//
// Created by paracelsus on 3/28/2022.
//

#include "ScatteredFileDescriptor.h"

ScatteredFileDescriptor::ScatteredFileDescriptor(bool directory) : FileDescriptor(directory) {
    size = 0;
}

std::ostream &operator<<(std::ostream &os, const ScatteredFileDescriptor &descriptor) {
    os << static_cast<const FileDescriptor &>(descriptor) << " tableSlices: " << descriptor.tableSlices << " size: "
       << descriptor.size;
    return os;
}

unsigned int ScatteredFileDescriptor::getSize() const {
    return size;
}

void ScatteredFileDescriptor::setSize(unsigned int size) {
    ScatteredFileDescriptor::size = size;
}
