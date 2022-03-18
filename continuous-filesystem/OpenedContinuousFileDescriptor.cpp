//
// Created by paracelsus on 2/13/2022.
//

#include "OpenedContinuousFileDescriptor.h"

OpenedContinuousFileDescriptor::OpenedContinuousFileDescriptor(ContinuousFileDescriptor fileDescriptor) : ContinuousFileDescriptor(fileDescriptor) {
    position = 0;
}

unsigned int OpenedContinuousFileDescriptor::getPosition() const {
    return position;
}

void OpenedContinuousFileDescriptor::setPosition(unsigned int position) {
    OpenedContinuousFileDescriptor::position = position;
}


std::ostream &operator<<(std::ostream &os, const OpenedContinuousFileDescriptor &descriptor) {
    os << "directory: " << descriptor.directory << " startingBlock: " << descriptor.startingBlock << " blocksReserved: "
       << descriptor.blocksReserved << " usedSpace: " << descriptor.usedSpace << " position: " << descriptor.position;
    return os;
}



