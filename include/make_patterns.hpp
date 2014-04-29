
#ifndef MAKE_PATTERNS_HPP
#define	MAKE_PATTERNS_HPP

#include <opencv2/core/core.hpp>
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

#include "MLP.hpp"
#include "HoughTransform.hpp"

cv::Mat make_patterns_given_neural(HoughTransform& hough, int angle, cv::Size img_size, cv::Mat mask)
{
    cv::Size h_size = hough.get_hough_space_size();
    
    CV_Assert(mask.size() == h_size);
    CV_Assert(mask.type() == CV_8U);
    
    cv::Mat patterns(0, 6, CV_32F);
    for(int rho=0; rho < h_size.width; rho++)
    {
        HoughTransform::houghpoints points = hough.get_points_at(angle, rho);
        foreach(cv::Point2i p, points)
        if(mask.at<uchar>(angle, rho) > 0)
        {
            cv::Mat pattern(1, 3, CV_32F);
            pattern.at<float>(0, 0) = float(p.x) / img_size.width;
            pattern.at<float>(0, 1) = float(p.y) / img_size.height;
            pattern.at<float>(0, 2) = float(rho) / h_size.width;
            patterns.push_back(pattern);
        }
    }
    return patterns;
}

void imageToPattern(cv::Mat& roi, cv::Mat& roiToPattern) 
{
    roiToPattern.create(1, roi.rows * roi.cols, CV_32F);
    float* dstRow = roiToPattern.ptr<float>(0);
    for (int n = 0, r = 0; r < roi.rows; r++) 
    {
        float* srcRow = roi.ptr<float>(r);
        for (int c = 0; c < roi.cols; c++) 
        {
            dstRow[n++] = srcRow[c];
        }
    }
}
void read_normalized_patch(const cv::Mat& imageHoughPadded, cv::Size win_size, cv::Mat& roiNorm, cv::Point2i p) 
{
    cv::Mat roi(imageHoughPadded,
            cv::Range(p.y, p.y + win_size.height),
            cv::Range(p.x, p.x + win_size.width));
    
    roi.copyTo(roiNorm);
    roiNorm.convertTo(roiNorm, CV_32F);
    
    roiNorm = roiNorm * (2.0f / 255.0f) - 1; // [-1,1]
}

void read_patch_as_row_vector(const cv::Mat& imageHoughPadded, cv::Size win_size, cv::Mat& roiAsVector, cv::Point2i p) 
{
    cv::Mat roiNorm;
    read_normalized_patch(imageHoughPadded, win_size, roiNorm, p);
    imageToPattern(roiNorm, roiAsVector);
}

cv::Mat zero_padding(const cv::Mat& src, const int x_padding, const int y_padding)
{
    cv::Mat out = cv::Mat::zeros(src.rows + 2*y_padding, src.cols + 2*x_padding, src.type());
    cv::Mat m = cv::Mat(out, 
                       cv::Range(y_padding, out.rows-y_padding),
                       cv::Range(x_padding, out.cols-x_padding));
    src.copyTo(m);
    return out;
}

cv::Mat threshold_mlp(MLP& nnetwork, cv::Size win_size, HoughTransform& hough) 
{
    int halfWinInH = win_size.height / 2;
    int halfWinInW = win_size.width / 2;
    int halfWinOutH = win_size.height / 2;
    int halfWinOutW = win_size.width / 2;

    cv::Mat imageHough = hough.get_hough_image();

    cv::Mat outputImage = cv::Mat::zeros(imageHough.rows, imageHough.cols, CV_8U);
    cv::Mat imageHoughPadded = zero_padding(imageHough, halfWinInW, halfWinInH);

    cv::Mat mlp_out;

    cv::Mat window(1, win_size.height*win_size.width, CV_32F);
    
    for (int row = 0; row < imageHough.rows; row += win_size.height) 
    {
        for (int col = 0; col < imageHough.cols; col += win_size.width) 
        {
            cv::Point2i p(col, row);

            read_patch_as_row_vector(imageHoughPadded, win_size, window, p);
            
            nnetwork.predict(window, mlp_out);

            float* netOutput = mlp_out.ptr<float>(0);

            for (int r = -halfWinOutH; r <= halfWinOutH; r++) 
            {
                if ((row + r) < 0 || (row + r) >= outputImage.rows) 
                    continue;
                uchar* dstRow = outputImage.ptr<uchar>(row + r);
                for (int c = -halfWinOutW; c <= halfWinOutW; c++) 
                {
                    if ((col + c) < 0 || (col + c) >= imageHough.cols)
                        continue;
                    float out_value = (1 + netOutput[(r + halfWinOutH) * win_size.width + (c + halfWinOutW)]) * 127.5;
                    out_value = MAX(0, out_value);
                    out_value = MIN(255, out_value);
                    dstRow[col + c] = cv::saturate_cast<uchar>(out_value);
                }
            }

        }
    }
    
    return outputImage;
}

#endif	/* MAKE_PATTERNS_HPP */

