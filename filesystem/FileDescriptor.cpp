//
// Created by paracelsus on 3/28/2022.
//

#include "FileDescriptor.h"
FileDescriptor::FileDescriptor(bool directory) {
    deleted = false;
    this->directory = directory;
}
void FileDescriptor::markDeleted() {
    deleted = true;
}

bool FileDescriptor::isDeleted() const {
    return deleted;
}

bool FileDescriptor::isDirectory() const {
    return directory;
}