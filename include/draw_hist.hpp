
#ifndef DRAW_AND_SHOW_HPP
#define	DRAW_AND_SHOW_HPP

#include <opencv2/core/core.hpp>

namespace artelab
{

    cv::Mat draw_histogram_on_image(cv::Mat hist, cv::Mat img, cv::Scalar color, int direction, int thick=1, int hist_height=50);

}

#endif	/* DRAW_AND_SHOW_HPP */

