
#ifndef DIRECTORYINFO_H
#define	DIRECTORYINFO_H

#include <string>
#include <vector>
#include "FileInfo.hpp"

class DirectoryInfo 
{
public:
    
    DirectoryInfo() {};
    DirectoryInfo(std::string fullPath);
    DirectoryInfo(const DirectoryInfo& other);
    ~DirectoryInfo() {}
    
    DirectoryInfo directoryCombine(std::string path);
    FileInfo fileCombine(std::string filename);
    void getSubdirNames(std::vector<std::string>& outList);
    std::string fullPath() { return _fullPath; }
    void getFiles(const std::string extension, std::vector<FileInfo>& outList);
    void getFiles(std::vector<FileInfo>& outList);
    
    DirectoryInfo& operator=(const DirectoryInfo& other);
    bool operator==(const DirectoryInfo& other);
    
private:
    std::string _fullPath;
};

#endif	/* DIRECTORYINFO_H */

