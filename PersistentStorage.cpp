//
// Created by paracelsus on 2/10/2022.
//

#include "PersistentStorage.h"
#include <iostream>
#include <cstring>
#include <exception>

PersistentStorage::PersistentStorage(unsigned int numberOfBlocks, unsigned int bytesPerBlock) {
    this->numberOfBlocks = numberOfBlocks;
    this->bytesPerBlock = bytesPerBlock;
    storage = (char*) malloc(numberOfBlocks * bytesPerBlock);
}

void PersistentStorage::write(unsigned int blockNumber, char *src) {
    if (blockNumber > numberOfBlocks) throw new std::invalid_argument("Block number greater than storage maximum!");
    memcpy(storage + blockNumber * bytesPerBlock, src, bytesPerBlock);
}

void PersistentStorage::read(unsigned int blockNumber, char *dst) {
    if (blockNumber > numberOfBlocks) throw new std::invalid_argument("Block number greater than storage maximum!");
    memcpy(dst, storage + blockNumber * bytesPerBlock, bytesPerBlock);
}

PersistentStorage::~PersistentStorage() {
    free(storage);
}
