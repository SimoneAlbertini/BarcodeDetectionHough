#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/program_options.hpp>

#include "ImageProcessor.hpp"

#define foreach BOOST_FOREACH

using namespace artelab;
using std::string;
using std::cout;
using std::flush;
using std::endl;
using std::vector;
namespace po = boost::program_options;

string netfile = "net61x3.net";
string outdir = "output";

int main(int argc, char** argv)
{
  po::options_description desc("Usage");
  desc.add_options()
    ("help", "print help")
    ("netfile,n",       po::value<string>()->default_value(netfile),         "trained network file")
    ("output,o",        po::value<string>()->default_value(outdir),          "if specified, intermediate images are saved there")
    ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help"))
    {
      cout << desc << endl;
      return EXIT_SUCCESS;
    }

  netfile = vm["netfile"].as<string>();
  outdir = vm["output"].as<string>();

  cv::Size win_size(61, 3);

  ImageProcessor pr(netfile, win_size, outdir);

  return EXIT_SUCCESS;
}
