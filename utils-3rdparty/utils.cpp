#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "utils.hpp"

namespace artelab 
{
    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) 
    {
        std::stringstream ss(s);
        std::string item;
        while(std::getline(ss, item, delim)) 
        {
            if (item != "")
                elems.push_back(item);
        }
        return elems;
    }


    std::vector<std::string> split(const std::string &s, char delim) 
    {
        std::vector<std::string> elems;
        return split(s, delim, elems);
    }
    
    bool file_exists(std::string filename)
    {
      std::ifstream ifile(filename.c_str());
      return ifile;
    }
}