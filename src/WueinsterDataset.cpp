/* 
 * File:   WueinsterDataset.cpp
 * Author: simone
 * 
 * Created on 5 giugno 2013, 9.57
 */

#include <boost/foreach.hpp>
#include <iostream>
#include <sstream>
#define foreach BOOST_FOREACH
#include "WueinsterDataset.hpp"

using std::string;
using std::vector;

WueinsterDataset::WueinsterDataset(DirectoryInfo dir) 
{
    _base_dir = dir;
}

WueinsterDataset::WueinsterDataset(const WueinsterDataset& orig) 
{
    *this = orig;
}

WueinsterDataset& WueinsterDataset::operator =(const WueinsterDataset& other)
{
    this->_base_dir = other._base_dir;
    this->_data = other._data;
    return *this;
}

WueinsterDataset::~WueinsterDataset() {}

int WueinsterDataset::count()
{
    return _data.size();
}

void WueinsterDataset::load_dataset()
{
    const string orig_suffix = ".jpg";
    const string detection_suffix = ".png";
    DirectoryInfo content_dir = _base_dir.directoryCombine("Original");
    DirectoryInfo detection_dir = _base_dir.directoryCombine("Detection");
    
    vector<FileInfo> files;
    detection_dir.getFiles("png", files);
    foreach(FileInfo file, files)
    {
        string name = artelab::split(file.getNameWithoutExtension(), '.')[0];
        
        barcode_images data;
        if(_data.count(name) > 0)
        {
            data = _data[name];
        }
        
        std::ostringstream original_file, detection_file;
        original_file << name << orig_suffix;
        detection_file << name << detection_suffix;
        
        data.original = content_dir.fileCombine(original_file.str());
        data.detection_gt = detection_dir.fileCombine(detection_file.str());
        _data[name] = data;
    }
    
}

std::map<std::string, WueinsterDataset::barcode_images> WueinsterDataset::get_barcodes()
{
    return _data;
}