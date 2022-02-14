//
// Created by paracelsus on 2/13/2022.
//

#include "FileDescriptor.h"

FileDescriptor::FileDescriptor(bool directory, unsigned startingBlock, unsigned blocksReserved, unsigned usedSpace) : directory(directory),
                                                                                                 startingBlock(startingBlock),
                                                                                                 blocksReserved(blocksReserved),
                                                                                                 usedSpace(usedSpace){}

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



