/* 
 * File:   draw_and_show.hpp
 * Author: simone
 *
 * Created on 24 maggio 2013, 12.55
 */

#ifndef DRAW_AND_SHOW_HPP
#define	DRAW_AND_SHOW_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

cv::Mat draw_histogram_on_image(cv::Mat hist, cv::Mat img, cv::Scalar color, int direction, int thick=1, int hist_height=50)
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

cv::Vec3b random_color(cv::RNG& rng)
{
    return cv::Vec3b(rng.uniform(100, 256), rng.uniform(100, 256), rng.uniform(100, 256));
}

typedef std::map<float, cv::Vec3b> colorsmap;

colorsmap color_map(cv::Mat labels, cv::RNG& rng)
{
    colorsmap colors;
    for(int r=0; r < labels.rows; r++)
    {
        float label = labels.at<float>(r,0);
        if(colors.count(label) == 0)
            colors[label] = random_color(rng);
    }
    return colors;
}

cv::Mat draw_clusters(cv::Mat patterns, cv::Mat labels, cv::Size size)
{
    cv::RNG rng;
    cv::Mat img(size, CV_8UC3);
    
    colorsmap colors = color_map(labels, rng);
    
    for(int r=0; r < patterns.rows; r++)
    {
        float* row = patterns.ptr<float>(r);
        int x = int(row[0] * size.width + 0.5);
        int y = int(row[1] * size.height + 0.5);
        float l = labels.at<float>(r, 0);
        img.at<cv::Vec3b>(y, x) = colors[l];
    }
    return img;
}

cv::Mat draw_row_clusters(cv::Mat_<float> row_patterns, cv::Mat_<float> labels)
{
    cv::RNG rng;
    cv::Mat img(row_patterns.size(), CV_8UC3);
    
    colorsmap colors = color_map(labels, rng);
    for(int r=0; r < row_patterns.rows; r++)
    {
        float* row = row_patterns.ptr<float>(r);
        for(int c=0; c < row_patterns.cols; c++)
        {
            img.at<cv::Vec3b>(r,c) = colors[labels.at<float>(r, 0)];
        }
    }
    return img;
}

#endif	/* DRAW_AND_SHOW_HPP */

