
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/program_options.hpp>

#include "ImageProcessor.hpp"

#define foreach BOOST_FOREACH
#define barcodeimage ArtelabDataset::barcode_image

using namespace artelab;
using std::string;
using std::cout;
using std::flush;
using std::endl;
using std::vector;
namespace po = boost::program_options;

bool display = false;
bool quiet = false;
string datasetdir = "dataset";
string netfile = "net61x3.net";
string outdir = "";
string whitelist = "";


vector<string> imagenames_to_process(FileInfo file)
{
    vector<string> names;
    std::ifstream infile;
    infile.open(file.fullName().c_str());
    
    while(!infile.eof())
    {
        string line;
        getline(infile, line);
        boost::trim(line);
        names.push_back(line);
    }
    infile.close();
    return names;
}

int main(int argc, char** argv) 
{   
    po::options_description desc("Usage");
    desc.add_options()
        ("help", "print help")
        ("dataset,d",       po::value<string>()->default_value(datasetdir),      "dataset directory")
        ("output,o",        po::value<string>()->default_value(outdir),          "if specified, intermediate images are saved there")
        ("whitelist,w",     po::value<string>()->default_value(whitelist),       "list of image names to process")
        ("show,s",                                                               "show intermediate images")
        ("quiet,q",                                                              "suppress verbose output")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);    

    if (vm.count("help"))
    {
        cout << desc << endl;
        return EXIT_SUCCESS;
    }
    
    datasetdir = vm["dataset"].as<string>();
    outdir = vm["output"].as<string>();
    whitelist = vm["whitelist"].as<string>();
    display = vm.count("show") > 0;
    quiet = vm.count("quiet") > 0;
    
    DirectoryInfo dataset_dir(datasetdir);
    
    ArtelabDataset dataset(dataset_dir);
    dataset.load_dataset();
    if(!quiet) cout << "Dataset Loaded: " << dataset.count() << " Images" << endl;
    
    MLP mlp; mlp.load(netfile);
    
    cv::Size win_size(61, 3);
    
    std::map<string, barcodeimage> images = dataset.get_barcodes();
    std::map<string, barcodeimage>::iterator it;
    
    vector<string> white_list;
    if(whitelist != "") 
        white_list = imagenames_to_process(FileInfo(whitelist));
    
    double accuracy = 0, time = 0;
    int count = 0;
    cv::Mat jaccard_hist = cv::Mat::zeros(10, 1, CV_32F);
    
    ImageProcessor pr(netfile, win_size, outdir, quiet, display);
    
    for(it = images.begin(); it != images.end(); it++)
    {
        if(white_list.size() > 0 && std::find(white_list.begin(), white_list.end(), it->first) == white_list.end())
        {
            continue;
        }
        
        results res = pr.process(it->first, it->second);
        
        accuracy += res.jaccard;
        time += res.time;
        jaccard_hist += res.jaccard_hist;
        count++;
        
        if(display)
        {
            char c;
            do
            {
                c = cv::waitKey();
            } while (c != 32);
        }
        
    }
    
    cout << endl 
         << "Number of images: " << count << endl
         << "TOTAL ACCURACY (jaccard): " << accuracy/count << endl
         << "TOTAL ACCURACY BY THRESHOLD (jaccard): " << jaccard_hist/count << endl
         << "Average time (sec): " << time/count << endl;
    
    return EXIT_SUCCESS;
}

