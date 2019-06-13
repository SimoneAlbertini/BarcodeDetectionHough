
#include "hough_histogram.hpp"

namespace artelab
{

    cv::Mat get_histogram(cv::Mat image, int hist_type, int type)
    {
        CV_Assert(image.type() == CV_8U);
        cv::Mat hist;

        if(hist_type == HIST_ROW)
        {
            hist = cv::Mat::zeros(image.rows, 1, CV_32S);
            for(int t=0; t < image.rows; t++)
            {
                cv::Scalar sum = cv::sum(image.row(t));
                hist.at<uint>(t, 0) = sum[0];
            }
        }
        else if(hist_type == HIST_COL)
        {
            hist = cv::Mat::zeros(image.cols, 1, CV_32S);
            for(int c=0; c < image.cols; c++)
            {
                cv::Scalar sum = cv::sum(image.col(c));
                hist.at<uint>(c, 0) = sum[0];
            }
        }

        if(type != CV_32S)
            hist.convertTo(hist, type);

        return hist;
    }

    int max_angle_hist(cv::Mat hist)
    {
        CV_Assert(hist.cols == 1);
        CV_Assert(hist.type() == CV_32S);

        int angle = 0;
        int max = -1;
        for(int r=0; r < hist.rows; r++)
        {
            int v = hist.at<int>(r, 0);
            if(v > max)
            {
                max = v;
                angle = r;
            }
        }
        return angle;
    }

}
