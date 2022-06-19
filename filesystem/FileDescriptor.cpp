//
// Created by paracelsus on 3/28/2022.
//

#include "FileDescriptor.h"

FileDescriptor::FileDescriptor(bool directory, unsigned usedSpace) {
    deleted = false;
    this->directory = directory;
    this->usedSpace = usedSpace;
}

void FileDescriptor::markDeleted() {
    deleted = true;
}

bool FileDescriptor::isDeleted() const {
    return deleted;
}

std::ostream &operator<<(std::ostream &os, const FileDescriptor &descriptor) {
    os << "deleted: " << descriptor.deleted << " directory: " << descriptor.directory;
    return os;
}

bool FileDescriptor::isDirectory() const {
    return directory;
}

unsigned int FileDescriptor::getUsedSpace() const {
    return usedSpace;
}

void FileDescriptor::setUsedSpace(unsigned int usedSpace) {
    FileDescriptor::usedSpace = usedSpace;
}
