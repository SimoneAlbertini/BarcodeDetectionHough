/* 
 * File:   HoughTransform.cpp
 * Author: simone
 * 
 * Created on 22 maggio 2013, 12.38
 */

#include <map>

#include "HoughTransform.hpp"


bool HoughTransform::CmpPoint2i::operator()(const cv::Point2i& left, const cv::Point2i& right) const
{
    if(left.y < right.y)
        return true;
    if(left.y > right.y)
        return false;
    if(left.x < right.x)
        return true;
    return false;
}

void HoughTransform::init()
{
    _keep_loc = false;
    _size = cv::Size();
    
    _max_theta = 180;
    _theta_step = M_PI / _max_theta;
    
    // cache the values of sin and cos for faster processing
    
    sin_cache = new double[_max_theta];
    cos_cache = new double[_max_theta];
    for(int t=0; t < _max_theta; t++)
    {
        sin_cache[t] = sin(t * _theta_step);
        cos_cache[t] = cos(t * _theta_step);
    }
}

HoughTransform::HoughTransform()
{
    init();
} 

HoughTransform::HoughTransform(cv::Mat image, bool keep_loc)
{
    init();
    compute_hough(image, keep_loc);
}

HoughTransform::~HoughTransform()
{
    delete [] sin_cache;
    delete [] cos_cache;
}

HoughTransform::HoughTransform(const HoughTransform& orig) 
{
    *this = orig;
}

HoughTransform& HoughTransform::operator =(const HoughTransform& other)
{
    this->_size = other._size;
    this->_keep_loc = other._keep_loc;
    other.hough_space.copyTo(this->hough_space);
    this->_locations = other._locations;
    this->_max_theta = other._max_theta;
    this->_theta_step = other._theta_step;
    return *this;
}

void HoughTransform::store_location(int t, int r, int x, int y)
{
    cv::Point2i h_point(r,t), img_point(x,y);
    if(_locations.count(h_point) > 0)
    {
        _locations[h_point].push_back(img_point);
    }
    else
    {
        houghpoints s;
        s.push_back(img_point);
        _locations[h_point] = s;
    }
}

void HoughTransform::add_point(int x, int y)
{
    double center_x = _size.width / 2;
    double center_y = _size.height / 2;
    
    for (int t = 0; t < _max_theta; t++)
    {
        //Work out the r values for each theta step
        int r = (int) (((x - center_x) * cos_cache[t]) + ((y - center_y) * sin_cache[t]));
        
        // this copes with negative values of r
        int hough_height = hough_space.cols / 2;
        r += hough_height;

        if (r < 0 || r >= hough_height*2) 
            continue;
        
        hough_space.at<uint>(t, r)++;
        
        if (_keep_loc)
            store_location(t, r, x, y);
    }
}

void HoughTransform::compute_hough(cv::Mat image, bool keep_loc)
{
    CV_Assert(image.type() == CV_8U);
    
    _size = image.size();
    _keep_loc = keep_loc;
    _locations.clear();
    
    int hough_height = (int) (sqrt(2) * MAX(_size.height, _size.width)) / 2;
    // doubled for negative r values
    hough_space = cv::Mat::zeros(_max_theta, hough_height*2, CV_32S);
    
    for(int x=0; x < image.cols; x++)
        for(int y=0; y < image.rows; y++)
            if(image.at<uchar>(y,x) > 0)
                add_point(x, y);
}

uint HoughTransform::get(int theta, int rho)
{
    return hough_space.at<uint>(theta, rho);
}

cv::Size HoughTransform::get_hough_space_size()
{
    return hough_space.size();
}

HoughTransform::houghpoints HoughTransform::get_points_at(int theta, int rho)
{
    return _locations[cv::Point2i(rho, theta)];
}

cv::Mat HoughTransform::get_hough_image(int threshold)
{
    cv::Mat out_image = cv::Mat::zeros(hough_space.rows, hough_space.cols, CV_8U);
    
    double max, min;
    cv::minMaxLoc(hough_space, &min, &max);
    
    for(int r=0; r < hough_space.rows; r++)
    {
        uint* row = hough_space.ptr<uint>(r);
        for(int c=0; c < hough_space.cols; c++)
        {
            uchar value = cv::saturate_cast<uchar>(255 * (double(row[c]) / max));
            if(value >= threshold)
                out_image.at<uchar>(r,c) = value;
        }
    }
    
    return out_image;
}