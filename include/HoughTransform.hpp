
#ifndef HOUGHTRANSFORM_HPP
#define	HOUGHTRANSFORM_HPP

#include <opencv2/core/core.hpp>
#include <map>

namespace artelab
{

    class HoughTransform 
    {
    public:

        typedef std::vector<cv::Point2i> houghpoints;

        HoughTransform();
        HoughTransform(cv::Mat image, bool keep_locations=false);
        virtual ~HoughTransform();
        HoughTransform(const HoughTransform& orig);

        HoughTransform& operator=(const HoughTransform& other);

        void compute_hough(cv::Mat image, bool keep_locations=false);

        cv::Mat get_hough_image(int threshold=0);
        cv::Size get_hough_space_size();
        uint get(int theta, int rho);
        houghpoints get_points_at(int theta, int rho);

        cv::Mat hough_space;

    private:

        struct CmpPoint2i
        {
            bool operator()(cv::Point2i const& left, cv::Point2i const& right) const;
        };

        typedef std::map<cv::Point2i, houghpoints, CmpPoint2i> locationsmap;

        bool _keep_loc;
        cv::Size _size;
        locationsmap _locations;

        int _max_theta;
        double _theta_step;

        double* sin_cache;
        double* cos_cache;

        void init();
        void add_point(int, int);
        void store_location(int,int,int,int);
    };

}

#endif	/* HOUGHTRANSFORM_HPP */

