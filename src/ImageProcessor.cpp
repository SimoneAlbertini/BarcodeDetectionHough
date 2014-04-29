
#include <boost/foreach.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iomanip>

#include "HoughTransform.hpp"
#include "mlp_threshold.hpp"
#include "hough_histogram.hpp"
#include "ImageProcessor.hpp"
#include "utils.hpp"
#include "draw_hist.hpp"
#include "detection.hpp"

#define foreach BOOST_FOREACH

namespace artelab
{

    ImageProcessor::ImageProcessor(std::string mlp_file, cv::Size win_size, std::string outdir, bool quiet, bool show) :
        _show(show),
        _quiet(quiet),
        _winsize(win_size)
    {
        _mlp.load(mlp_file);
        _output = DirectoryInfo(outdir);
    }

    ImageProcessor::~ImageProcessor() 
    { }

    ImageProcessor& ImageProcessor::show(bool b)
    {
        _show = b;
        return *this;
    }

    ImageProcessor& ImageProcessor::set_output(std::string outdir)
    {
        _output = DirectoryInfo(outdir);
        return *this;
    }

    
    void ImageProcessor::show_image(std::string name, cv::Mat img)
    {
        if(_show)
        {
            cv::namedWindow(name);
            cv::imshow(name, img);
        }
    }
    
    void draw_lines_at_angle(double angle, std::vector<cv::Vec4i> lines, cv::Mat& image, int tolerance=2)
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
    
    results ImageProcessor::process(std::string name, ArtelabDataset::barcode_image bcimage)
    {
        cv::Mat img_orig = cv::imread(bcimage.original.fullName(), CV_LOAD_IMAGE_COLOR);
        cv::Mat img_truth = cv::imread(bcimage.detection_gt.fullName(), CV_LOAD_IMAGE_GRAYSCALE);

        TimeCounter tc;
        tc.start();

        // Apply canny
        cv::Mat img_canny;
        cv::GaussianBlur(img_orig, img_canny, cv::Size(17,17), 2);
        cv::Canny(img_canny, img_canny, 60, 100, 3);

        // Get hough transform
        HoughTransform hough(img_canny);

        // Threshold with MLP
        cv::Mat img_neural = threshold_mlp(_mlp, _winsize, hough);

        // find angle
        double angle = max_angle_hist(get_histogram(img_neural, HIST_ROW));

        // lines from canny using probabilistc hough
        const int tolerance = 3;
        std::vector<cv::Vec4i> lines;
        cv::HoughLinesP(img_canny, lines, 1, CV_PI/180, 50, 20, 1);
        cv::Mat line_image;
        if(_show)
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

        // crop image with projected mask and threshold
        cv::Mat bb_mask;
        bb_mask = artelab::rotate_image(img_hist_projection, -angle);
        double min, max;
        cv::minMaxLoc(bb_mask, &min, &max);
        cv::threshold(bb_mask, bb_mask, int(0.3*max), 1, cv::THRESH_BINARY);
        cv::Mat img_cropped;
        if(_show) img_orig.copyTo(img_cropped, bb_mask);

        // draw bounding boxes
        std::vector<cv::Rect> rects = object_rectangles(img_hist_projection, int(0.3*max));
        cv::Mat img_bb; 
        img_orig.copyTo(img_bb);
        cv::Mat img_detection_mask = cv::Mat::zeros(img_orig.size(), CV_8U);
        foreach(cv::Rect r, rects)
        {
            // BB for impression on original image
            cv::Mat bb = cv::Mat::zeros(img_bb.size(), CV_8U);
            cv::rectangle(bb, r, cv::Scalar(255), 2);
            bb = artelab::rotate_image(bb, -angle);
            img_bb.setTo(cv::Scalar(0,0,255), bb);

            // BB for measuring accuracy
            cv::Mat bb_fill = cv::Mat::zeros(img_orig.size(), CV_8U);
            cv::rectangle(bb_fill, r, cv::Scalar(255), -1);
            bb_fill = artelab::rotate_image(bb_fill, -angle);
            img_detection_mask.setTo(cv::Scalar(255), bb_fill);
        }

        tc.stop();

        // Measuring accuracy
        results res = measure_results(img_detection_mask, img_truth, tc);
        
        // verbose output
        if(!_quiet)
        {
            std::cout << std::setw(15) << std::left << name;
            std::cout << std::setw(11) << std::left << ("Angle " + tostring(angle));
            std::cout << std::setw(20) << std::left << ("Accuracy: " + tostring(res.jaccard));
            std::cout << std::setw(6) << std::left << ("Time: " + tostring(res.time)) << std::endl;
        }
        
        // show results
        show_image("Prob Hough", line_image);
        show_image("histograms smooth", feature_with_hist_smooth);
        show_image("histograms projection", img_hist_projection);
        show_image("Cropped", img_cropped);
        show_image("Boundig Boxes", img_bb);
        show_image("Original", img_orig);
        show_image("Feature", feature_image);
        show_image("Canny", img_canny);

        // saving intermediate images
        if(_output.fullPath() != "")
        {
            std::ostringstream ss;

            // Original
            ss << name << "_1_original.png";
            cv::imwrite(_output.fileCombine(ss.str()).fullName(), img_orig);

            // Hough accumulator
            ss.str(""); ss.clear(); ss << name << "_2_accumulator.png";
            cv::imwrite(_output.fileCombine(ss.str()).fullName(), hough.get_hough_image());

            // Hough MLP threshold
            img_neural = draw_histogram_on_image(get_histogram(img_neural, HIST_ROW, CV_32F), img_neural, cv::Scalar(0,0,255), HIST_ROW);
            ss.str(""); ss.clear();; ss << name << "_3_thresh_mlp.png";
            cv::imwrite(_output.fileCombine(ss.str()).fullName(), img_neural);

            // Feature with histograms not processed
            histograms_from_hough_lines(feature_image, row_hist, col_hist, false);
            cv::Mat feature_with_hist = draw_histogram_on_image(row_hist, feature_image, cv::Scalar(0,0,255), HIST_ROW);
            feature_with_hist = draw_histogram_on_image(col_hist, feature_with_hist, cv::Scalar(0,255,0), HIST_COL);
            ss.str(""); ss.clear();; ss << name << "_4_hist.png";
            cv::imwrite(_output.fileCombine(ss.str()).fullName(), feature_with_hist);

            // Feature with processed histograms
            ss.str(""); ss.clear();; ss << name << "_5_hist_smooth_thresh.png";
            cv::imwrite(_output.fileCombine(ss.str()).fullName(), feature_with_hist_smooth);

            // Histogram projection
            ss.str(""); ss.clear();; ss << name << "_6_hist_projection.png";
            cv::imwrite(_output.fileCombine(ss.str()).fullName(), img_hist_projection);

            // Bounding boxes
            ss.str(""); ss.clear();; ss << name << "_7_boxes.png";
            cv::imwrite(_output.fileCombine(ss.str()).fullName(), img_bb);

            // Canny
            ss.str(""); ss.clear(); ss << name << "_canny.png";
            cv::imwrite(_output.fileCombine(ss.str()).fullName(), img_canny);
        }
        
        return res;
    }

}