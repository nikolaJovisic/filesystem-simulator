//
// Created by paracelsus on 2/10/2022.
//

#include "PersistentStorage.h"
#include "../filesystem/Constants.h"
#include <iostream>
#include <cstring>

PersistentStorage::PersistentStorage() {
    numberOfBlocks = 0;
    bytesPerBlock = 0;
    storage = nullptr;
}

PersistentStorage::PersistentStorage(unsigned int numberOfBlocks, unsigned int bytesPerBlock) {
    if(numberOfBlocks < MIN_NUMBER_OF_BLOCKS) throw std::invalid_argument("Number of blocks too small.");
    if(bytesPerBlock < MIN_NUMBER_OF_BYTES_PER_BLOCK) throw std::invalid_argument("Block serializationSize too small.");
    this->numberOfBlocks = numberOfBlocks;
    this->bytesPerBlock = bytesPerBlock;
    storage = (char*) malloc(numberOfBlocks * bytesPerBlock);
}

PersistentStorage::PersistentStorage(std::string filename) {
    std::ifstream rf(filename, std::ios::out | std::ios::binary);

    rf.read((char *)&(bytesPerBlock), sizeof(unsigned));
    rf.read((char *)&(numberOfBlocks), sizeof(unsigned));
    storage = (char*) malloc(numberOfBlocks * bytesPerBlock);
    rf.read(storage, bytesPerBlock * numberOfBlocks);

    rf.close();
    if(!rf.good()) throw std::runtime_error("Error in deserialization.");
}

void PersistentStorage::serializeInFile(std::string filename) {
    std::ofstream wf(filename, std::ios::out | std::ios::binary);
    if(!wf) throw ("Filename problem.");

    wf.write((char *)&(bytesPerBlock), sizeof(unsigned));
    wf.write((char *)&(numberOfBlocks), sizeof(unsigned));
    wf.write(storage, bytesPerBlock * numberOfBlocks);

    wf.close();
    if(!wf.good()) throw std::runtime_error("Error in serialization.");
}


void PersistentStorage::write(unsigned int block, char *src) {
    if (block > numberOfBlocks) throw std::invalid_argument("Block number greater than storage maximum!");
    std::memcpy(storage + block * bytesPerBlock, src, bytesPerBlock);
}

void PersistentStorage::read(unsigned int block, char *dst) {
    if (block > numberOfBlocks) throw std::invalid_argument("Block number greater than storage maximum!");
    std::memcpy(dst, storage + block * bytesPerBlock, bytesPerBlock);
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


