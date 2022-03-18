//
// Created by paracelsus on 2/13/2022.
//

#ifndef FILESYSTEM_SIMULATOR_DIRECTORY_H
#define FILESYSTEM_SIMULATOR_DIRECTORY_H

#include <map>
#include <string>
#include <vector>

class Directory {
    struct Element{
        unsigned filenameLength;
        char* filename;
        unsigned index;
    };
private:
    std::map<std::string, unsigned> content;
public:
    Directory();
    Directory(char *src, unsigned length);
    std::vector<std::string> getAllFilenames();
    int getIndex(std::string filename);
    void addFile(std::string filename, unsigned index);
    void removeFile(std::string filename);
    unsigned size();
    void serialize(char *dst);
};


#endif //FILESYSTEM_SIMULATOR_DIRECTORY_H
