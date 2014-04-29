
#include <opencv2/imgproc/imgproc.hpp>
#include <boost/foreach.hpp>

#include "detection.hpp"

#define foreach BOOST_FOREACH

namespace artelab
{
    
    cv::Rect contour_bounding_box(std::vector<cv::Point> contour)
    {
        int top = std::numeric_limits<int>::max();
        int bot = -1;
        int left = top;
        int right = -1;
        foreach(cv::Point p, contour)
        {
            if(p.x < left) left = p.x;
            if(p.x > right) right = p.x;
            if(p.y < top) top = p.y;
            if(p.y > bot) bot = p.y;
        }
        return cv::Rect(left, top, right-left, bot-top);
    }
    
    std::vector<cv::Rect> object_rectangles(cv::Mat feature_image, int thresh)
    {
        using std::vector;

        vector<cv::Rect> rectangles;
        cv::Size imgsize = feature_image.size();
        const double smaller_side = 18;
        double scalefactor = (imgsize.width > imgsize.height? imgsize.height : imgsize.width) / smaller_side;

        int min_size_rect = int(scalefactor * smaller_side / 6 + 0.5);

        cv::Mat img_thresh;
        cv::threshold(feature_image, img_thresh, thresh, 255, cv::THRESH_BINARY);

        vector<vector<cv::Point> > contours;
        vector<cv::Vec4i> hierarchy;
        cv::findContours(img_thresh, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0,0));

        foreach(vector<cv::Point> cont, contours)
        {
            cv::Rect bb = contour_bounding_box(cont);

            if(bb.width > min_size_rect || bb.height > min_size_rect)
                rectangles.push_back(bb);
        }
        return rectangles;
    }
    
    
}