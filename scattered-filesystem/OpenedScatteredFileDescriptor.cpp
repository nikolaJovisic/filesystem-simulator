//
// Created by paracelsus on 3/28/2022.
//

#include "OpenedScatteredFileDescriptor.h"

OpenedScatteredFileDescriptor::OpenedScatteredFileDescriptor(ScatteredFileDescriptor fileDescriptor)
        : ScatteredFileDescriptor(fileDescriptor) {
    position = 0;
    indirectionBlock.load(fileDescriptor.getBlock());
}

unsigned int OpenedScatteredFileDescriptor::getPosition() const {
    return position;
}

void OpenedScatteredFileDescriptor::setPosition(unsigned int position) {
    OpenedScatteredFileDescriptor::position = position;
}

std::ostream &operator<<(std::ostream &os, const OpenedScatteredFileDescriptor &descriptor) {
    os << static_cast<const ScatteredFileDescriptor &>(descriptor) << " position: " << descriptor.position;
    return os;
}
