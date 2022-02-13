//
// Created by paracelsus on 2/13/2022.
//

#include "Directory.h"

std::vector<std::string> Directory::getAllFilenames() {
    std::vector<std::string> filenames;
    for (auto const& element : content) {
        filenames.push_back(element.first);
    }
    return filenames;
}

unsigned Directory::getIndex(std::string filename) {
    return content[filename];
}

void Directory::addFile(std::string filename, unsigned int index) {
    content[filename] = index; //overwrite default
}

void Directory::removeFile(std::string filename) {
    content.erase(filename);
}

Directory::Directory() {
    content = std::map<std::string, unsigned>();
}
