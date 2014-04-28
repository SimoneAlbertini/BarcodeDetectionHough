/* 
 * File:   BarcodeDataset.cpp
 * Author: simone
 * 
 * Created on 27 maggio 2013, 14.33
 */

#include <Artelibs/DirectoryInfo.h>
#include <boost/foreach.hpp>
#include <Artelibs/utils.h>
#define foreach BOOST_FOREACH

#include "ArtelabDataset.hpp"

typedef ArtelabDataset::barcode_images image;

using std::string;
using std::vector;

ArtelabDataset::ArtelabDataset(DirectoryInfo base) 
{
    _base_dir = base;
}

ArtelabDataset::ArtelabDataset(const ArtelabDataset& orig) 
{
    *this = orig;
}

ArtelabDataset& ArtelabDataset::operator =(const ArtelabDataset& other)
{
    this->_base_dir = other._base_dir;
    this->_data = other._data;
}

void ArtelabDataset::load_dataset()
{
    const string orig_suffix = ".jpgbarcodeOrig.png";
    const string canny_suffix = ".jpgbarcodeCanny.png";
    const string detection_suffix = ".png";
    DirectoryInfo content_dir = _base_dir.directoryCombine("Original");
    DirectoryInfo detection_dir = _base_dir.directoryCombine("Detection");
    
    vector<FileInfo> files;
    content_dir.getFiles("png", files);
    foreach(FileInfo file, files)
    {
        string name = artelab::split(file.getNameWithoutExtension(), '.')[0];
        
        std::ostringstream original_file, canny_file, detection_file;
        original_file << name << orig_suffix;
        canny_file << name << canny_suffix;
        detection_file << name << detection_suffix;
        
        barcode_images data;
        data.original = content_dir.fileCombine(original_file.str());
        data.canny = content_dir.fileCombine(canny_file.str());
        data.detection_gt = detection_dir.fileCombine(detection_file.str());
        
        if(!artelab::file_exists(data.original.fullName()) || !artelab::file_exists(data.detection_gt.fullName()))
            continue;
        
        if(_data.count(name) > 0)
        {
            data = _data[name];
        }
        _data[name] = data;
    }
    
}

int ArtelabDataset::count()
{
    return _data.size();
}

std::map<std::string, ArtelabDataset::barcode_images> ArtelabDataset::get_barcodes()
{
    return _data;
}