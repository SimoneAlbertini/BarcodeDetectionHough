/* 
 * File:   WueinsterDataset.hpp
 * Author: simone
 *
 * Created on 5 giugno 2013, 9.57
 */

#ifndef WUEINSTERDATASET_HPP
#define	WUEINSTERDATASET_HPP

#include <Artelibs/DirectoryInfo.h>
#include <map>

class WueinsterDataset {
public:
    
    typedef struct 
    {
        FileInfo original;
        FileInfo canny;
        FileInfo detection_gt;
    } barcode_images;
    
    WueinsterDataset(DirectoryInfo dir);
    WueinsterDataset(const WueinsterDataset& orig);
    virtual ~WueinsterDataset();
    
    WueinsterDataset& operator=(const WueinsterDataset& other);
    
    void load_dataset();
    int count();
    std::map<std::string, barcode_images> get_barcodes();
    
private:
    std::map<std::string, barcode_images> _data;
    DirectoryInfo _base_dir;
};

#endif	/* WUEINSTERDATASET_HPP */

