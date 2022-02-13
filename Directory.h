//
// Created by paracelsus on 2/13/2022.
//

#ifndef FILESYSTEM_SIMULATOR_DIRECTORY_H
#define FILESYSTEM_SIMULATOR_DIRECTORY_H

#include <map>
#include <string>
#include <vector>

class Directory {
private:
    std::map<std::string, unsigned> content;
public:
    Directory();
    std::vector<std::string> getAllFilenames();
    unsigned getIndex(std::string filename);
    void addFile(std::string filename, unsigned index); //overwrite default
    void removeFile(std::string filename);
};


#endif //FILESYSTEM_SIMULATOR_DIRECTORY_H
