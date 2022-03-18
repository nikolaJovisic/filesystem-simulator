//
// Created by paracelsus on 2/14/2022.
//

#include "PathTransform.h"

#include <utility>
#include <sstream>
#include "Constants.h"


std::deque<std::string> PathTransform::filePathDirectories(std::string path) {
    auto deque = split(std::move(path));
    deque.pop_back();
    deque.pop_front();
    return deque;
}

std::deque<std::string> PathTransform::directoryPathDirectories(std::string path) {
    auto deque =  split(std::move(path));
    deque.pop_front();
    return deque;
}

std::string PathTransform::filePathFile(std::string path) {
    return split(std::move(path)).back();
}

std::deque<std::string> PathTransform::split(std::string path) {
    if (!path.starts_with(DELIMITER)) {
        path = DELIMITER + path;
    }
    std::deque<std::string> retVal;
    std::stringstream stringStream(path);
    std::string item;
    while (getline (stringStream, item, DELIMITER)) {
        retVal.push_back (item);
    }
    return retVal;
}
