/* 
 * File:   FileInfo.h
 * Author: ignazio
 *
 * Created on May 16, 2012, 2:28 PM
 */

#ifndef FILEINFO_H
#define	FILEINFO_H

#include <string>

class FileInfo 
{
public:
    
    FileInfo(){}
    FileInfo(std::string fullName);
    FileInfo(const FileInfo& orig);
    
    virtual ~FileInfo();
    
    std::string fullName() const { return _fullName; }
    
    std::string getName();
    std::string getNameWithoutExtension();
    std::string getExtension();
    std::string getBaseDir();
    
    void setFullName(std::string fullName);
        
    bool operator<(const FileInfo &other)const {return (_name < other._name);}
    bool operator ==(const FileInfo& other) const {return _fullName == other._fullName;}
    FileInfo& operator=(const FileInfo& other);
    bool operator !=(const FileInfo& other) const {return _fullName != other._fullName;}
    
private:
    std::string _fullName;
    
    std::string _name;
    std::string _name_without_extension;
    std::string _extension;
};

#endif	/* FILEINFO_H */

