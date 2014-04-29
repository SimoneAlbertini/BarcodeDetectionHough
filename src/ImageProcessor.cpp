/* 
 * File:   ImageProcessor.cpp
 * Author: simone
 * 
 * Created on April 29, 2014, 10:13 AM
 */

#include "ImageProcessor.hpp"

ImageProcessor::ImageProcessor(std::string mlp_file, cv::Size win_size, std::string outdir, bool show)
{
    _mlp.load(mlp_file);
    _winsize = win_size;
    _output = DirectoryInfo(outdir);
    _show = show;
}


ImageProcessor::~ImageProcessor() 
{ }

ImageProcessor& ImageProcessor::show(bool b)
{
    _show = b;
    return *this;
}

ImageProcessor& ImageProcessor::set_output(std::string outdir)
{
    _output = DirectoryInfo(outdir);
    return *this;
}

results ImageProcessor::process(ArtelabDataset::barcode_image barcode)
{
    
}