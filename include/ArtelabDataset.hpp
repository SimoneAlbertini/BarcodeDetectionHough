
#ifndef BARCODEDATASET_HPP
#define	BARCODEDATASET_HPP

#include <opencv2/core/core.hpp>
#include "DirectoryInfo.hpp"

namespace artelab
{

    class ArtelabDataset 
    {
    public:

        typedef struct 
        {
            FileInfo original;
            FileInfo detection_gt;
        } barcode_image;

        ArtelabDataset(DirectoryInfo d);
        ArtelabDataset(const ArtelabDataset& orig);
        virtual ~ArtelabDataset() {}

        ArtelabDataset& operator=(const ArtelabDataset& other);

        void load_dataset();
        int count();

        std::map<std::string, barcode_image> get_barcodes();

        private:
            std::map<std::string, barcode_image> _data;
            DirectoryInfo _base_dir;
    };

}

#endif	/* BARCODEDATASET_HPP */

