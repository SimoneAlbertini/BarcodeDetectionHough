
#ifndef DETECTION_HPP
#define	DETECTION_HPP

#include <opencv2/core/core.hpp>

namespace artelab
{    

    std::vector<cv::Rect> object_rectangles(cv::Mat feature_image, int thresh=70);

}

#endif	/* DETECTION_HPP */

