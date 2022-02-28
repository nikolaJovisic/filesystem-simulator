//
// Created by paracelsus on 2/13/2022.
//

#include "FileDescriptor.h"

FileDescriptor::FileDescriptor(bool directory, unsigned startingBlock, unsigned blocksReserved, unsigned usedSpace) : directory(directory),
                                                                                                 startingBlock(startingBlock),
                                                                                                 blocksReserved(blocksReserved),
                                                                                                 usedSpace(usedSpace),
                                                                                                 deleted(false){}

bool FileDescriptor::isDirectory() const {
    return directory;
}

unsigned int FileDescriptor::getStartingBlock() const {
    return startingBlock;
}

unsigned int FileDescriptor::getBlocksReserved() const {
    return blocksReserved;
}

unsigned int FileDescriptor::getUsedSpace() const {
    return usedSpace;
}

void FileDescriptor::setUsedSpace(unsigned int usedSpace) {
    FileDescriptor::usedSpace = usedSpace;
}

std::ostream &operator<<(std::ostream &os, const FileDescriptor &descriptor) {
    os << "startingBlock: " << descriptor.startingBlock << " blocksReserved: " << descriptor.blocksReserved
       << " usedSpace: " << descriptor.usedSpace << (descriptor.directory ? " - is a directory" : " - is not a directory") << (descriptor.deleted ? " - is deleted" : "");
    return os;
}

void FileDescriptor::setBlocksReserved(unsigned int blocksReserved) {
    FileDescriptor::blocksReserved = blocksReserved;
}

void FileDescriptor::markDeleted() {
    FileDescriptor::deleted = true;
}





