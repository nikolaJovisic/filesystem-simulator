//
// Created by paracelsus on 3/28/2022.
//

#include "ScatteredFileDescriptor.h"

ScatteredFileDescriptor::ScatteredFileDescriptor(bool directory, unsigned block) : FileDescriptor(directory, 0) {
    size = 0;
    this->block = block;
}

std::ostream &operator<<(std::ostream &os, const ScatteredFileDescriptor &descriptor) {
    os << (descriptor.deleted ? " block (historically): " : " block: ") << descriptor.block;
    if (!descriptor.deleted) os << " size: " << descriptor.size;
    os << (descriptor.directory ? " - is a directory" : " - is not a directory")
       << (descriptor.deleted ? " - is deleted" : "");
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


