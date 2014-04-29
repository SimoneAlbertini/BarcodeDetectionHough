
#include <algorithm>
#include <sstream>
#include <boost/filesystem.hpp>
#include "DirectoryInfo.hpp"

using std::string;
using std::vector;

DirectoryInfo::DirectoryInfo(string fullPath) 
{
    if (fullPath[fullPath.length() -1] == '/')
    {
        fullPath = fullPath.substr(0, fullPath.length()-1);
    }
        
    _fullPath = fullPath;
}

DirectoryInfo::DirectoryInfo(const DirectoryInfo& other)
{
    *this = other;
}

DirectoryInfo& DirectoryInfo::operator=(const DirectoryInfo& other)
{
    _fullPath = other._fullPath;
    return *this;
}

bool DirectoryInfo::operator ==(const DirectoryInfo& other)
{
    return _fullPath == other._fullPath;
}

void DirectoryInfo::getFiles(vector<FileInfo>& outList)
{
    namespace fs = boost::filesystem;
    
    outList.clear();
    fs::path basePath(_fullPath);
    fs::directory_iterator end_it;
    
    if(fs::exists(basePath) && fs::is_directory(basePath))
    {
        for(fs::directory_iterator dir_it(basePath); dir_it != end_it; dir_it++)
        {
            if(fs::is_regular_file(*dir_it))
            {
                outList.push_back(FileInfo(dir_it->path().string()));
            }
        }
    }
}

void DirectoryInfo::getFiles(string extension, vector<FileInfo>& outList)
{
    namespace fs = boost::filesystem;
    
    if(extension.length() > 0 && extension[0] != '.')
    {
        std::ostringstream ss;
        ss << "." << extension;
        extension = ss.str();
    }
    
    outList.clear();
    fs::path basePath(_fullPath);
    fs::directory_iterator end_it;
    
    if(fs::exists(basePath) && fs::is_directory(basePath))
    {
        for(fs::directory_iterator dir_it(basePath); dir_it != end_it; dir_it++)
        {
            if(fs::is_regular_file(*dir_it) && dir_it->path().extension() == extension)
            {
                outList.push_back(FileInfo(dir_it->path().string()));
            }
        }
    }
}

DirectoryInfo DirectoryInfo::directoryCombine(string path)
{
    if (path[path.length()-1] == '/')
    {
        path = path.substr(0, path.length()-1);
    }
    
    if (path[0] != '/')
    {
        path = "/" + path;
    }
    
    path = _fullPath + path;
    return DirectoryInfo(path);
}

FileInfo DirectoryInfo::fileCombine(string filename)
{
    if(filename.length() > 0 && filename[filename.length()-1] == '/')
    {
        filename = filename.substr(0, filename.length()-1);
    }
    
    if(filename[0] != '/')
    {
        filename = "/" + filename;
    }
    
    string fullpath = _fullPath + filename;
    return FileInfo(fullpath);
}

void DirectoryInfo::getSubdirNames(vector<string>& outList)
{
    namespace fs = boost::filesystem;
    
    outList.clear();
    fs::path path(_fullPath);
    fs::directory_iterator end_it;
    
    if(fs::exists(path) && fs::is_directory(path))
    {
        for(fs::directory_iterator dir_it(path); dir_it != end_it; dir_it++)
        {
            if(fs::is_directory(*dir_it))
            {
                outList.push_back(string(dir_it->path().filename().c_str()));
            }
        }
    }
}