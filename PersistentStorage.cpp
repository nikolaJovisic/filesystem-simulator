//
// Created by paracelsus on 2/10/2022.
//

#include "PersistentStorage.h"
#include <iostream>
#include <cstring>

PersistentStorage::PersistentStorage() {
    numberOfBlocks = 0;
    bytesPerBlock = 0;
    storage = nullptr;
}

PersistentStorage::PersistentStorage(unsigned int numberOfBlocks, unsigned int bytesPerBlock) {
    this->numberOfBlocks = numberOfBlocks;
    this->bytesPerBlock = bytesPerBlock;
    storage = (char*) malloc(numberOfBlocks * bytesPerBlock);
}

void PersistentStorage::write(unsigned int block, char *src) {
    if (block > numberOfBlocks) throw std::invalid_argument("Block number greater than storage maximum!");
    memcpy(storage + block * bytesPerBlock, src, bytesPerBlock);
}

void PersistentStorage::read(unsigned int block, char *dst) {
    if (block > numberOfBlocks) throw std::invalid_argument("Block number greater than storage maximum!");
    memcpy(dst, storage + block * bytesPerBlock, bytesPerBlock);
}

PersistentStorage::~PersistentStorage() {
    free(storage);
}

unsigned int PersistentStorage::getNumberOfBlocks() const {
    return numberOfBlocks;
}

unsigned int PersistentStorage::blockSize() const {
    return bytesPerBlock;
}

std::ostream &operator<<(std::ostream &os, const PersistentStorage &storage) {
    os << "numberOfBlocks: " << storage.numberOfBlocks << " bytesPerBlock: "
       << storage.bytesPerBlock;
    return os;
}


