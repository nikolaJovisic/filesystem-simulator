//
// Created by paracelsus on 2/14/2022.
//

#include "PathTransform.h"

#include <utility>
#include <sstream>
#include "Constants.h"


std::deque<std::string> PathTransform::filePathFolders(std::string path) {
    auto deque = split(std::move(path));
    deque.pop_back();
    return deque;
}

std::deque<std::string> PathTransform::folderPathFolders(std::string path) {
    return split(std::move(path));
}

std::string PathTransform::filePathFile(std::string path) {
    return split(std::move(path)).back();
}

std::deque<std::string> PathTransform::split(std::string path) {
    std::deque<std::string> retVal;
    std::stringstream stringStream(path);
    std::string item;
    while (getline (stringStream, item, DELIMITER)) {
        retVal.push_back (item);
    }
    return retVal;
}
