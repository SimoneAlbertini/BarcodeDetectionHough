
#ifndef IMAGEPROCESSOR_HPP
#define	IMAGEPROCESSOR_HPP

#include <opencv2/core/core.hpp>

#include "MLP.hpp"
#include "ArtelabDataset.hpp"
#include "accuracy.hpp"

namespace artelab
{
  class ImageProcessor
  {
  public:

    ImageProcessor(std::string mlp_file, cv::Size win_size, std::string outdir="", bool quiet=false, bool show=false);
    virtual ~ImageProcessor();

    ImageProcessor& show(bool b=true);
    ImageProcessor& set_output(std::string outdir);

    results process(std::string imagename, ArtelabDataset::barcode_image barcode);
    cv::Mat processSimple(cv::Mat img, double& angle);
    void drawRectangles(cv::Mat& img_bb, cv::Rect rect, double angle);
    std::vector<cv::Rect> getRectangles(cv::Mat img_hist_projection, double angle);


  private:

    MLP _mlp;
    cv::Size _winsize;
    DirectoryInfo _output;
    bool _show;
    bool _quiet;

    void show_image(std::string name, cv::Mat img);
  };

}

#endif	/* IMAGEPROCESSOR_HPP */
