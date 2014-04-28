/* 
 * File:   BarcodeDataset.hpp
 * Author: simone
 *
 * Created on 27 maggio 2013, 14.33
 */

#ifndef BARCODEDATASET_HPP
#define	BARCODEDATASET_HPP

#include <opencv2/core/core.hpp>
#include <Artelibs/DirectoryInfo.h>

class ArtelabDataset 
{
public:
    
    typedef struct 
    {
        FileInfo original;
        FileInfo canny;
        FileInfo detection_gt;
    } barcode_images;
    
    ArtelabDataset(DirectoryInfo);
    ArtelabDataset(const ArtelabDataset& orig);
    virtual ~ArtelabDataset() {}
    
    ArtelabDataset& operator=(const ArtelabDataset& other);
    
    void load_dataset();
    int count();
    
    std::map<std::string, barcode_images> get_barcodes();
    
    private:
        std::map<std::string, barcode_images> _data;
        DirectoryInfo _base_dir;
};

#endif	/* BARCODEDATASET_HPP */

