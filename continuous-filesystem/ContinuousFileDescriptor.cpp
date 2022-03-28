//
// Created by paracelsus on 2/13/2022.
//

#include "ContinuousFileDescriptor.h"

ContinuousFileDescriptor::ContinuousFileDescriptor(bool directory, unsigned startingBlock, unsigned blocksReserved, unsigned usedSpace) : directory(directory),
                                                                                                                                          startingBlock(startingBlock),
                                                                                                                                          blocksReserved(blocksReserved),
                                                                                                                                          usedSpace(usedSpace){}

bool ContinuousFileDescriptor::isDirectory() const {
    return directory;
}

unsigned int ContinuousFileDescriptor::getStartingBlock() const {
    return startingBlock;
}

unsigned int ContinuousFileDescriptor::getBlocksReserved() const {
    return blocksReserved;
}

unsigned int ContinuousFileDescriptor::getUsedSpace() const {
    return usedSpace;
}

void ContinuousFileDescriptor::setUsedSpace(unsigned int usedSpace) {
    ContinuousFileDescriptor::usedSpace = usedSpace;
}

std::ostream &operator<<(std::ostream &os, const ContinuousFileDescriptor &descriptor) {
    os << (descriptor.deleted ? " startingBlock (historically): " : " startingBlock: ") << descriptor.startingBlock << (descriptor.deleted ? " blocksReserved (historically): " :" blocksReserved: ") << descriptor.blocksReserved;
    if (!descriptor.deleted) os << " usedSpace: " << descriptor.usedSpace;
    os << (descriptor.directory ? " - is a directory" : " - is not a directory") << (descriptor.deleted ? " - is deleted" : "");
    return os;
}

void ContinuousFileDescriptor::setBlocksReserved(unsigned int blocksReserved) {
    ContinuousFileDescriptor::blocksReserved = blocksReserved;
}






