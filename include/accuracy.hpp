
#ifndef ACCURACY_HPP
#define	ACCURACY_HPP

#include <opencv2/core/core.hpp>
#include "TimeCounter.hpp"

namespace artelab
{

    typedef struct
    {
        double jaccard;
        cv::Mat jaccard_hist;
        double time;
    } results;

    bool is_true_positive(cv::Mat mask, cv::Mat truth, double thresh=0.5);

    results measure_results(cv::Mat mask, cv::Mat truth, TimeCounter tc);

}

#endif	/* ACCURACY_HPP */
