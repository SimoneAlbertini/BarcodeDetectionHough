
#ifndef MAKE_PATTERNS_HPP
#define	MAKE_PATTERNS_HPP

#include <opencv2/core/core.hpp>

#include "MLP.hpp"
#include "HoughTransform.hpp"

namespace artelab
{

    cv::Mat threshold_mlp(MLP& nnetwork, cv::Size win_size, HoughTransform& hough);

}

#endif	/* MAKE_PATTERNS_HPP */
