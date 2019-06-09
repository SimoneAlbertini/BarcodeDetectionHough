#include <boost/algorithm/string/trim.hpp>
#include <boost/foreach.hpp>
#include <boost/program_options.hpp>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "ImageProcessor.hpp"

#define foreach BOOST_FOREACH

using namespace artelab;
using std::cout;
using std::endl;
using std::flush;
using std::string;
using std::vector;
namespace po = boost::program_options;

string netfile = "net61x3.net";
string outdir = "output";
string video_stream_file = "0";

int main(int argc, char **argv) {
  po::options_description desc("Usage");
  desc.add_options()("help", "print help")(
      "netfile,n", po::value<string>()->default_value(netfile),
      "trained network file")(
      "video_stream,v", po::value<string>()->default_value(video_stream_file),
      "path to the video file or camera index")(
      "output,o", po::value<string>()->default_value(outdir),
      "if specified, intermediate images are saved there");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    cout << desc << endl;
    return EXIT_SUCCESS;
  }

  netfile = vm["netfile"].as<string>();
  outdir = vm["output"].as<string>();
  video_stream_file = vm["video_stream"].as<string>();

  cv::Size win_size(61, 3);

  ImageProcessor pr(netfile, win_size, outdir);
  cv::VideoCapture video_stream = cv::VideoCapture(video_stream_file);
  cv::Mat frame, processed_frame, img_hist_projection;
  double angle;
  std::vector<cv::Rect> rects;

  char c = '0';
  cv::namedWindow("detected");
  while (c != 'q') {
    if (!video_stream.read(frame)) {
      break;
    }
    img_hist_projection = pr.processSimple(frame, angle);
    frame.copyTo(processed_frame);
    rects = pr.getRectangles(img_hist_projection, angle);
    foreach (cv::Rect r, rects) {
      pr.drawRectangles(processed_frame, r, angle);
    }
    cv::imshow("detected",processed_frame);
    c = cv::waitKey(10);
  }

  return EXIT_SUCCESS;
}
