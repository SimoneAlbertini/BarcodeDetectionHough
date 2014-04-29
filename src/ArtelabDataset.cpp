
#include <boost/foreach.hpp>
#include "DirectoryInfo.hpp"
#include "utils.hpp"
#include "ArtelabDataset.hpp"

#define foreach BOOST_FOREACH

using std::string;
using std::vector;

namespace artelab
{
    
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
        const string detection_suffix = ".png";
        DirectoryInfo content_dir = _base_dir.directoryCombine("Original");
        DirectoryInfo detection_dir = _base_dir.directoryCombine("Detection");

        vector<FileInfo> files;
        content_dir.getFiles("png", files);
        foreach(FileInfo file, files)
        {
            string name = artelab::split(file.getNameWithoutExtension(), '.')[0];

            std::ostringstream original_file, detection_file;
            original_file << name << orig_suffix;
            detection_file << name << detection_suffix;

            barcode_image data;
            data.original = content_dir.fileCombine(original_file.str());
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

    std::map<std::string, ArtelabDataset::barcode_image> ArtelabDataset::get_barcodes()
    {
        return _data;
    }

}