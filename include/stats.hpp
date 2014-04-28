/* 
 * File:   stats.hpp
 * Author: simone
 *
 * Created on 24 maggio 2013, 12.56
 */

#ifndef STATS_HPP
#define	STATS_HPP

#include <opencv2/core/core.hpp>

typedef struct { double mean; double var; double min; double max;} stat;

stat get_stats(cv::Mat patterns, int col)
{
    stat st;
    cv::Mat values = patterns.col(col);
    cv::minMaxLoc(values, &st.min, &st.max);
    cv::Scalar sum = cv::sum(values);
    st.mean = sum[0] / double(values.rows);
    st.var = 0;
    for(int r=0; r < values.rows; r++)
    {
        float v = values.at<float>(r, 0);
        st.var += (v-st.mean)*(v-st.mean);
    }
    st.var /= values.rows-1;
    return st;
}

#endif	/* STATS_HPP */

