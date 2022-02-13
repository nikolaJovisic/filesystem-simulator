//
// Created by paracelsus on 2/13/2022.
//

#include "FileDescriptor.h"

FileDescriptor::FileDescriptor(bool directory, unsigned startingBlock, unsigned length) : directory(directory),
                                                                               startingBlock(startingBlock),
                                                                               length(length){}

bool FileDescriptor::isDirectory() const {
    return directory;
}

unsigned int FileDescriptor::getStartingBlock() const {
    return startingBlock;
}

unsigned int FileDescriptor::getLength() const {
    return length;
}

