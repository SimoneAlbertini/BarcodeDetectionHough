
#include <opencv2/imgproc/imgproc.hpp>

#include "hough_histogram.hpp"
#include "draw_hist.hpp"

namespace artelab
{

    cv::Mat draw_histogram_on_image(cv::Mat hist, cv::Mat img, cv::Scalar color, int direction, int thick, int hist_height)
    {
        cv::Mat image;
        img.copyTo(image);
        if(img.type() == CV_8U)
            cv::cvtColor(image, image, CV_GRAY2BGR);

        double min, max;
        cv::minMaxLoc(hist, &min, &max);
        hist = hist / max * hist_height;

        if(direction == HIST_ROW)
        {
            CV_Assert(hist.rows == img.rows);
            for(int b=0; b < hist.rows; b++)
            {
                uint v = uint(hist.at<float>(b,0));
                cv::line(image, cv::Point(0, b), cv::Point(v-1, b), color, thick, 4);
            }
        }
        else if (direction == HIST_COL)
        {
            CV_Assert(hist.rows == img.cols);
            for(int b=0; b < hist.rows; b++)
            {
                uint v = uint(hist.at<float>(b,0));
                cv::line(image, cv::Point(b, 0), cv::Point(b, v-1), color, thick, 4);
            }
        }
        return image;
    }


}
