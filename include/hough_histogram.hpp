
#ifndef HOUGH_HISTOGRAM_HPP
#define	HOUGH_HISTOGRAM_HPP

#include <opencv2/core/core.hpp>

namespace artelab
{

    enum { HIST_ROW, HIST_COL };

    cv::Mat get_histogram(cv::Mat image, int hist_type=HIST_ROW, int type=CV_32S);

    int max_angle_hist(cv::Mat hist);

}

#endif	/* HOUGH_HISTOGRAM_HPP */

