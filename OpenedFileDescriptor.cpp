//
// Created by paracelsus on 2/13/2022.
//

#include "OpenedFileDescriptor.h"

OpenedFileDescriptor::OpenedFileDescriptor(ContinuousFileDescriptor fileDescriptor) : ContinuousFileDescriptor(fileDescriptor) {
    position = 0;
}

unsigned int OpenedFileDescriptor::getPosition() const {
    return position;
}

void OpenedFileDescriptor::setPosition(unsigned int position) {
    OpenedFileDescriptor::position = position;
}


std::ostream &operator<<(std::ostream &os, const OpenedFileDescriptor &descriptor) {
    os << "directory: " << descriptor.directory << " startingBlock: " << descriptor.startingBlock << " blocksReserved: "
       << descriptor.blocksReserved << " usedSpace: " << descriptor.usedSpace << " position: " << descriptor.position;
    return os;
}



