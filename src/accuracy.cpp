
#include "accuracy.hpp"


namespace artelab
{
    
    double jaccard_overlap(cv::Mat mask, cv::Mat truth)
    {
        CV_Assert(mask.type() == CV_8U);
        CV_Assert(truth.type() == CV_8U);

        cv::Mat intersection, un;
        cv::bitwise_and(mask, truth, intersection);
        cv::bitwise_or(mask, truth, un);

        double union_area = cv::sum(un)[0];
        if(union_area == 0) return 1;
        double intersection_area = cv::sum(intersection)[0];

        return intersection_area / union_area;
    }

    bool is_true_positive(cv::Mat mask, cv::Mat truth, double thresh)
    {
        CV_Assert(mask.type() == CV_8U);
        CV_Assert(truth.type() == CV_8U);

        cv::Mat intersection;
        cv::bitwise_and(mask, truth, intersection);
        double sum_inter = cv::sum(intersection)[0];
        double sum_truth = cv::sum(truth)[0];

        return (sum_inter / sum_truth) > thresh;
    }

    results measure_results(cv::Mat mask, cv::Mat truth, TimeCounter tc)
    {
        results res;
        res.jaccard_hist = cv::Mat(10, 1, CV_32F);
        res.jaccard = jaccard_overlap(mask, truth);
        res.time = tc.get_seconds();
        for(int i=0; i < 10; i++)
            res.jaccard_hist.at<float>(i, 0) = res.jaccard >= (i+1)/10.0? 1 : 0;

        return res;
    }
    
    
}
