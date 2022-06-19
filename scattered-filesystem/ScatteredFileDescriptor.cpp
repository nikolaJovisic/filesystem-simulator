//
// Created by paracelsus on 3/28/2022.
//

#include "ScatteredFileDescriptor.h"

ScatteredFileDescriptor::ScatteredFileDescriptor(bool directory, unsigned block) : FileDescriptor(directory, 0) {
    size = 0;
    this->block = block;
}

std::ostream &operator<<(std::ostream &os, const ScatteredFileDescriptor &descriptor) {
    os << static_cast<const FileDescriptor &>(descriptor) << " block: " << descriptor.block << " size: "
       << descriptor.size;
    return os;
}

unsigned int ScatteredFileDescriptor::getSize() const {
    return size;
}

void ScatteredFileDescriptor::setSize(unsigned int size) {
    ScatteredFileDescriptor::size = size;
}

unsigned ScatteredFileDescriptor::getBlock() {
    return block;
}


