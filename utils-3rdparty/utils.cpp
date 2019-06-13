#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <sys/stat.h>
#include <opencv2/imgproc/imgproc.hpp>
#include "utils.hpp"

namespace artelab 
{
    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) 
    {
        std::stringstream ss(s);
        std::string item;
        while(std::getline(ss, item, delim)) 
        {
            if (item != "")
                elems.push_back(item);
        }
        return elems;
    }


    std::vector<std::string> split(const std::string &s, char delim) 
    {
        std::vector<std::string> elems;
        return split(s, delim, elems);
    }
    
    //Changed the file existance checking to use stat, as it is at least 2x faster
    bool file_exists(std::string filename)
    {
	struct stat buffer;
	return (stat(filename.c_str(), &buffer)==0);
    }
    
    cv::Mat rotate_image(cv::Mat img, int angle, int size_factor)
    {
        cv::Mat out;
        cv::Point2i center = cv::Point2i(img.cols / 2, img.rows / 2);
        cv::Size size(img.cols* size_factor, img.rows*size_factor);

        cv::Mat rotation_mat = cv::getRotationMatrix2D(center, angle, 1);
        cv::warpAffine(img, out, rotation_mat, size);
        return out;
    }
    
    std::string tostring(double d)
    {
        std::ostringstream ss;
        ss << d;
        return ss.str();
    }
}
