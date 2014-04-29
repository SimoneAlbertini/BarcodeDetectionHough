
#include <vector>
#include <algorithm>
#include "FileInfo.hpp"

using namespace std;

FileInfo::FileInfo(string fullName) 
{
    setFullName(fullName);
}



FileInfo::FileInfo(const FileInfo& orig) 
{
    *this = orig;
}

FileInfo& FileInfo::operator=(const FileInfo& other) 
{
    _fullName = other._fullName;
    _name = other._name;
    _name_without_extension = other._name_without_extension;
    _extension = other._extension;
    return *this;
}

FileInfo::~FileInfo() { }

void FileInfo::setFullName(string fullName) 
{
    _fullName = fullName;
    getNameWithoutExtension();
    getExtension();
}

string FileInfo::getName()
{

    if (_name.empty()) 
    {
        size_t found;   
        // Searches the string from the end for any of the characters / or 
        found=_fullName.find_last_of("/\\");
        _name = _fullName.substr(found+1);
    }
    return _name;
}



string FileInfo::getNameWithoutExtension()
{
    if(_name_without_extension.empty())
    { 
        size_t found;   
        getName();
        // Searches the string from the end for any of the characters '.' 
        found=_name.find_last_of(".");
        _name_without_extension = _name.substr(0,found);
    }
    return _name_without_extension;
}

string FileInfo::getExtension()
{
    if(_extension.empty())
    { 
        _extension = "";
        size_t found = 0;   
        getName();
        // Searches the string from the end for any of the characters '.' 
        found=_name.find_last_of(".");
        if (found < _name.length()){
            _extension = _name.substr(found+1);
            transform(_extension.begin(), _extension.end(),
                    _extension.begin(), ::tolower );
        }
    }
    return _extension;
}

string FileInfo::getBaseDir()
{
    string baseDir = _fullName.substr(0, _fullName.length()- _name.length());
    return baseDir == ""? "." : baseDir;
}
