//
// Created by paracelsus on 2/13/2022.
//

#include "Directory.h"
#include <stdexcept>
#include <cstring>

Directory::Directory() : content() {}

Directory::Directory(char *src, unsigned length) : content() {
    auto end = src + length;
    while (src != end) {
        unsigned filenameLength;
        char* filename;
        unsigned index;
        filenameLength = *reinterpret_cast<unsigned *>(src);
        src += sizeof(unsigned);
        filename = reinterpret_cast<char *>(src);
        src += filenameLength * sizeof(char);
        index = *reinterpret_cast<unsigned *>(src);
        src += sizeof(unsigned);
        content.emplace(std::string(filename, filenameLength), index);
    }
}

unsigned Directory::size() {
    auto size = content.size() * (2 * sizeof(unsigned));
    for(const auto& element: content) {
        size += element.first.length() * sizeof(char);
    }
    return size;
}

void Directory::serialize(char *dst) {
    for(auto element: content) {
        unsigned filenameLength = element.first.length();
        unsigned index = element.second;
        std::memcpy(dst, &filenameLength, sizeof(unsigned));
        std::memcpy(dst += sizeof(unsigned), element.first.c_str(), sizeof(char) * filenameLength);
        std::memcpy(dst += sizeof(char) * filenameLength, &index, sizeof(unsigned));
        dst += sizeof(unsigned);
    }
}

std::vector<std::string> Directory::getAllFilenames() {
    std::vector<std::string> filenames;
    for (auto const& element : content) {
        filenames.push_back(element.first);
    }
    return filenames;
}

int Directory::getIndex(std::string filename) {
    auto it = content.find(filename);
    if (it == content.end()) return -1;
    return it->second;
}

void Directory::addFile(std::string filename, unsigned int index) {
    if (content.contains(filename)) throw std::invalid_argument("File with that name already exists.");
    content[filename] = index;
}

void Directory::removeFile(std::string filename) {
    content.erase(filename);
}





