
#include <iostream>
#include <set>
#include <algorithm>
#include <limits>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/trim.hpp>

#include "TimeCounter.hpp"
#include "HoughTransform.hpp"
#include "hough_histogram.hpp"
#include "draw_and_show.hpp"
#include "stats.hpp"
#include "make_patterns.hpp"
#include "MLP.hpp"
#include "ArtelabDataset.hpp"
#include "detection.hpp"
#include "accuracy.hpp"
#include "WueinsterDataset.hpp"

#define TO_RAD(degree) degree*M_PI/180
#define foreach BOOST_FOREACH

using std::string;
using std::cout;
using std::flush;
using std::endl;
using std::vector;

typedef HoughTransform::points houghpoints;
//typedef WueinsterDataset::barcode_images barcodeimages;
typedef ArtelabDataset::barcode_images barcodeimages;

bool display = true;

void show_image(std::string name, cv::Mat img)
{
    if(display)
    {
        cv::namedWindow(name);
        cv::imshow(name, img);
    }
}

cv::Mat rotate_image(cv::Mat img, int angle, int size_factor=1)
{
    cv::Mat out;
    cv::Point2i center = cv::Point2i(img.cols / 2, img.rows / 2);
    cv::Size size(img.cols* size_factor, img.rows*size_factor);
    
    cv::Mat rotation_mat = cv::getRotationMatrix2D(center, angle, 1);
    cv::warpAffine(img, out, rotation_mat, size);
    return out;
}

void draw_lines_at_angle(double angle, vector<cv::Vec4i> lines, cv::Mat& image, int tolerance=2)
{
    cv::Scalar color = image.channels() == 1? cv::Scalar(255) : cv::Scalar(0,0,255);
    
    foreach(cv::Vec4i l, lines)
    {
        double delta = double(l[3] - l[1]) / double(l[2] - l[0]);
        double act_angle = atan(delta)*180/CV_PI;
        act_angle += act_angle < 0? 180 : 0;
        double diff = fabs(act_angle - angle);
        if(diff < tolerance)
        {
            int thick = int(diff) > 3? 1 : 4-int(diff);
            cv::line(image, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), color, thick, 8);
        }
    }
}

void smooth_histogram(cv::Mat& hist, int kernel_size, int iterations)
{
    CV_Assert(hist.cols == 1);
    
    int hist_type = hist.type();
    bool cvt_back = hist_type != CV_32F;
    if(cvt_back) hist.convertTo(hist, CV_32F);
    
    cv::Mat kernel = cv::Mat::ones(kernel_size, 1, CV_32F);
    kernel = kernel / float(kernel_size);
    
    for(int i=0; i < iterations; i++)
    {
        cv::filter2D(hist, hist, -1, kernel);
    }
    
    if(cvt_back) hist.convertTo(hist, hist_type);
}

void histograms_from_hough_lines(cv::Mat feature, cv::Mat& row_hist, cv::Mat& col_hist, bool smooth_and_thresh=true)
{
    row_hist = get_histogram(feature, HIST_ROW, CV_32F);
    col_hist = get_histogram(feature, HIST_COL, CV_32F);
    
    if(smooth_and_thresh)
    {
        smooth_histogram(row_hist, 5, 200);
        smooth_histogram(col_hist, 5, 200);

        double row_hist_mean = cv::sum(row_hist)[0] / double(row_hist.rows);
        double col_hist_mean = cv::sum(col_hist)[0] / double(col_hist.rows);
        cv::threshold(row_hist, row_hist, row_hist_mean, 0, cv::THRESH_TOZERO);
        cv::threshold(col_hist, col_hist, col_hist_mean, 0, cv::THRESH_TOZERO);
    }
}

cv::Mat project_histograms(cv::Mat row_hist, cv::Mat col_hist)
{
    CV_Assert(row_hist.type() == CV_32F);
    CV_Assert(col_hist.type() == CV_32F);
    
    cv::Mat out = row_hist * col_hist.t();
    
    double min, max;
    cv::minMaxLoc(out, &min, &max);
    out = (out - min) / max * 255;
    out.convertTo(out, CV_8U);
    
    return out;
}

results process_image(MLP& mlp, cv::Size win_size, barcodeimages bcimages)
{
    cv::Mat img_orig = cv::imread(bcimages.original.fullName(), CV_LOAD_IMAGE_COLOR);
    cv::Mat img_truth = cv::imread(bcimages.detection_gt.fullName(), CV_LOAD_IMAGE_GRAYSCALE);
    
    TimeCounter tc;
    tc.start();
    
    // Apply canny
    cv::Mat img_canny;
    cv::GaussianBlur(img_orig, img_canny, cv::Size(17,17), 2);
    cv::Canny(img_canny, img_canny, 60, 100, 3);
    
    // Get hough transform
    HoughTransform hough(img_canny);
    
    // Threshold with MLP
    cv::Mat img_neural = threshold_mlp(mlp, win_size, hough);
    
    // find angle
    double angle = max_angle_hist(get_histogram(img_neural, HIST_ROW));
    
    cout << "Angle " << angle << " "; cout.flush();
   
    // lines from canny using probabilistc hough
    const int tolerance = 3;
    vector<cv::Vec4i> lines;
    cv::HoughLinesP(img_canny, lines, 1, CV_PI/180, 50, 20, 1);
    cv::Mat line_image;
    if(display)
    {
        // impress lines of given angle on the canny image
        cv::cvtColor(img_canny, line_image, CV_GRAY2BGR);
        draw_lines_at_angle((int(angle+0.5) + 90) % 180, lines, line_image, tolerance);
    }
    
    // obtain a feature image with only the lines. It is rectified.
    cv::Mat feature_image;
    feature_image = cv::Mat::zeros(img_canny.size(), CV_8U);
    draw_lines_at_angle((int(angle+0.5) + 90) % 180, lines, feature_image, tolerance);
    feature_image = rotate_image(feature_image, angle);
    
    //Histograms for detection
    cv::Mat row_hist, col_hist,feature_with_hist_smooth;
    histograms_from_hough_lines(feature_image, row_hist, col_hist);
    feature_with_hist_smooth = draw_histogram_on_image(row_hist, feature_image, cv::Scalar(0,0,255), HIST_ROW);
    feature_with_hist_smooth = draw_histogram_on_image(col_hist, feature_with_hist_smooth, cv::Scalar(0,255,0), HIST_COL);  
    
    // project histograms
    cv::Mat img_hist_projection = project_histograms(row_hist, col_hist);
    
    // crop image with projected mask and magic threshold
    cv::Mat bb_mask;
    bb_mask = rotate_image(img_hist_projection, -angle);
    double min, max;
    cv::minMaxLoc(bb_mask, &min, &max);
    cv::threshold(bb_mask, bb_mask, int(0.3*max), 1, cv::THRESH_BINARY);
    cv::Mat img_cropped;
    if(display) img_orig.copyTo(img_cropped, bb_mask);
    
    // draw bounding boxes
    vector<cv::Rect> rects = object_rectangles(img_hist_projection, int(0.3*max));
    cv::Mat img_bb; 
    img_orig.copyTo(img_bb);
    cv::Mat img_detection_mask = cv::Mat::zeros(img_orig.size(), CV_8U);
    foreach(cv::Rect r, rects)
    {
        // BB for impression on original image
        cv::Mat bb = cv::Mat::zeros(img_bb.size(), CV_8U);
        cv::rectangle(bb, r, cv::Scalar(255), 2);
        bb = rotate_image(bb, -angle);
        img_bb.setTo(cv::Scalar(0,0,255), bb);
        
        // BB for measuring accuracy
        cv::Mat bb_fill = cv::Mat::zeros(img_orig.size(), CV_8U);
        cv::rectangle(bb_fill, r, cv::Scalar(255), -1);
        bb_fill = rotate_image(bb_fill, -angle);
        img_detection_mask.setTo(cv::Scalar(255), bb_fill);
    }
    
    tc.stop();
    
    // Measuring accuracy
    results res = measure_results(img_detection_mask, img_truth, tc);
    cout << " Accuracy: " << res.jaccard;
    cout << " Time: " << res.time; cout.flush();
    
    /* show results */
    
    show_image("Prob Hough", line_image);
    show_image("histograms smooth", feature_with_hist_smooth);
    show_image("histograms projection", img_hist_projection);
    show_image("Cropped", img_cropped);
    show_image("Boundig Boxes", img_bb);
    show_image("Original", img_orig);
    show_image("Feature", feature_image);
    show_image("Canny", img_canny);
    
//    DirectoryInfo outdir("/home/simone/Desktop/barcodes");
//    string name = bcimages.original.getName();
//    std::ostringstream ss;
////     
////    // 1. Original
//    ss << name << "_1_original.png";
//    cv::imwrite(outdir.fileCombine(ss.str()).fullName(), img_orig);
////    
////    // 2. Hough accumulator
//    ss.str(""); ss.clear(); ss << name << "_2_accumulator.png";
//    cv::imwrite(outdir.fileCombine(ss.str()).fullName(), hough.get_hough_image());
////    
////    // 3. Hough MLP threshold
//    img_neural = draw_histogram_on_image(get_histogram(img_neural, HIST_ROW, CV_32F), img_neural, cv::Scalar(0,0,255), HIST_ROW);
//    ss.str(""); ss.clear();; ss << name << "_3_thresh_mlp.png";
//    cv::imwrite(outdir.fileCombine(ss.str()).fullName(), img_neural);
////    
////    // 4. feature with histograms not processed
//    histograms_from_hough_lines(feature_image, row_hist, col_hist, false);
//    cv::Mat feature_with_hist = draw_histogram_on_image(row_hist, feature_image, cv::Scalar(0,0,255), HIST_ROW);
//    feature_with_hist = draw_histogram_on_image(col_hist, feature_with_hist, cv::Scalar(0,255,0), HIST_COL);
//    ss.str(""); ss.clear();; ss << name << "_4_hist.png";
//    cv::imwrite(outdir.fileCombine(ss.str()).fullName(), feature_with_hist);
////    
////    // 5. feature with processed histograms
//    ss.str(""); ss.clear();; ss << name << "_5_hist_smooth_thresh.png";
//    cv::imwrite(outdir.fileCombine(ss.str()).fullName(), feature_with_hist_smooth);
////    
////    // 6. histogram projection
//    ss.str(""); ss.clear();; ss << name << "_6_hist_projection.png";
//    cv::imwrite(outdir.fileCombine(ss.str()).fullName(), img_hist_projection);
////    
////    // 7. bounding boxes
//    ss.str(""); ss.clear();; ss << name << "_7_boxes.png";
//    cv::imwrite(outdir.fileCombine(ss.str()).fullName(), img_bb);
////    
////    // Canny
//    ss.str(""); ss.clear(); ss << name << "_canny.png";
//    cv::imwrite(outdir.fileCombine(ss.str()).fullName(), img_canny);
    
    return res;
}

vector<string> imagenames_to_process(FileInfo file)
{
    vector<string> names;
    std::ifstream infile;
    infile.open(file.fullName().c_str());
    
    while(!infile.eof())
    {
        string line;
        getline(infile, line);
        boost::trim(line);
        names.push_back(line);
    }
    infile.close();
    return names;
}

int main(int argc, char** argv) 
{   
    DirectoryInfo dataset_dir_arte("/home/simone/Dataset/Barcodes/barcode-hough");
    DirectoryInfo dataset_dir_wue("/home/simone/Dataset/Barcodes/Wueinster");
    FileInfo mlp_path("net61x3.net");
    
    //WueinsterDataset dataset(dataset_dir_wue);
    ArtelabDataset dataset(dataset_dir_arte);
    dataset.load_dataset();
    cout << "Dataset Loaded: " << dataset.count() << " Images" << endl;
    
    MLP mlp; mlp.load(mlp_path.fullName());
    
    cv::Size win_size(61, 3);
    
    std::map<string, barcodeimages> images = dataset.get_barcodes();
    std::map<string, barcodeimages>::iterator it;
    
    vector<string> white_list; // = imagenames_to_process(FileInfo("pucci.txt"));
    
//    vector<FileInfo> names;
//    names.push_back(FileInfo("2200458003693-01_N95-2592x1944_scaledTo640x480bilinear.png"));
//    names.push_back(FileInfo("3182550402538-01_N95-2592x1944_scaledTo640x480bilinear.png"));
//    names.push_back(FileInfo("4000286216979-01_N95-2592x1944_scaledTo640x480bilinear.png"));
//    names.push_back(FileInfo("4000445116300-01_N95-2592x1944_scaledTo640x480bilinear.png"));
//    names.push_back(FileInfo("4000680703266-02_N95-2592x1944_scaledTo640x480bilinear.png"));
//    
//    foreach(FileInfo ff, names)
//    {
//        barcodeimages imgs;
//        imgs.original = ff;
//        imgs.canny = FileInfo("nofile");
//        process_image(mlp, win_size, imgs);
//    }
//    
//    return 0;
    
    double accuracy = 0, time = 0;
    int count = 0;
    cv::Mat jaccard_hist = cv::Mat::zeros(10, 1, CV_32F);
    for(it = images.begin(); it != images.end(); it++)
    {
        if(white_list.size() > 0 && std::find(white_list.begin(), white_list.end(), it->first) == white_list.end())
        {
//            cout << it->first << " Skipped" << endl;
            continue;
        }
        
        
        cout << it->first << " "; cout.flush();
        results res = process_image(mlp, win_size, it->second);
        cout << endl;
        
        accuracy += res.jaccard;
        time += res.time;
        jaccard_hist += res.jaccard_hist;
        count++;
        
        char c;
        if(display)
        {
            do
            {
                c = cv::waitKey();
            } while (c != 32);
        }
        
    }
    
    cout << "Number images: " << count << endl;
    cout << endl << "TOTAL ACCURACY (jaccard): " << accuracy/count << endl;
    cout << endl << "TOTAL ACCURACY (jaccard): " << jaccard_hist/count << endl;
    cout << endl << "Average time (sec): " << time/count << endl;
    
    return EXIT_SUCCESS;
}

