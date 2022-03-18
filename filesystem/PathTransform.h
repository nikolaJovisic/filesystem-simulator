//
// Created by paracelsus on 2/14/2022.
//

#ifndef FILESYSTEM_SIMULATOR_PATHTRANSFORM_H
#define FILESYSTEM_SIMULATOR_PATHTRANSFORM_H


#include <deque>
#include <string>

class PathTransform {
public:
    static std::deque<std::string> filePathDirectories(std::string path);
    static std::deque<std::string> directoryPathDirectories(std::string path);
    static std::string filePathFile(std::string path);
private:
    static std::deque<std::string> split(std::string path);
};


#endif //FILESYSTEM_SIMULATOR_PATHTRANSFORM_H
