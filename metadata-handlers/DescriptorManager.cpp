//
// Created by paracelsus on 2/13/2022.
//

#include "DescriptorManager.h"
#include <stdexcept>
#include <iostream>
#include <cstring>


DescriptorManager::DescriptorManager(PersistentStorage &persistentStorage, unsigned int blocksAvailable) : persistentStorage(persistentStorage), blocksAvailable(blocksAvailable){
    descriptorsPerBlock = persistentStorage.blockSize() / sizeof(ContinuousFileDescriptor);
    descriptorCount = 0;
}
DescriptorManager::DescriptorManager(PersistentStorage &persistentStorage) : persistentStorage(persistentStorage) {
}


unsigned DescriptorManager::addDescriptor(ContinuousFileDescriptor fileDescriptor) {
    if (descriptorCount == blocksAvailable * descriptorsPerBlock) {
        throw std::runtime_error("Out of descriptor dedicated memory.");
    }
    auto block = descriptorCount / descriptorsPerBlock;
    char content[persistentStorage.blockSize()];
    persistentStorage.read(block, content);
    auto positionInBlock = descriptorCount % descriptorsPerBlock;
    std::memcpy(content + positionInBlock * sizeof(ContinuousFileDescriptor), &fileDescriptor, sizeof(ContinuousFileDescriptor));
    persistentStorage.write(block, content);
    return descriptorCount++;
}

ContinuousFileDescriptor DescriptorManager::getDescriptor(unsigned int index) {
    if (index >= descriptorCount) {
        throw std::runtime_error("Index out of descriptor bounds.");
    }
    auto block = index / descriptorsPerBlock;
    char content[persistentStorage.blockSize()];
    persistentStorage.read(block, content);
    auto positionInBlock = index % descriptorsPerBlock;
    return *reinterpret_cast<ContinuousFileDescriptor *>(content + positionInBlock * sizeof(ContinuousFileDescriptor));
}

void DescriptorManager::updateDescriptor(unsigned int index, ContinuousFileDescriptor fileDescriptor) {
    if (index >= descriptorCount) {
        throw std::runtime_error("Index out of descriptor bounds.");
    }
    auto block = index / descriptorsPerBlock;
    char content[persistentStorage.blockSize()];
    persistentStorage.read(block, content);
    auto positionInBlock = index % descriptorsPerBlock;
    std::memcpy(content + positionInBlock * sizeof(ContinuousFileDescriptor), &fileDescriptor, sizeof(ContinuousFileDescriptor));
    persistentStorage.write(block, content);
}

void DescriptorManager::printAllDescriptors() {
    for (int i = 0; i < descriptorCount; ++i) {
        std::cout<<i<<": "<<getDescriptor(i)<<std::endl;
    }
}

void DescriptorManager::serializeInMemory(char *dst) {
    std::memcpy(dst, (char *)&blocksAvailable, sizeof(unsigned));
    dst += sizeof(unsigned);
    std::memcpy(dst, (char *)&descriptorCount, sizeof(unsigned));
    dst += sizeof(unsigned);
    std::memcpy(dst, (char *)&descriptorsPerBlock, sizeof(unsigned));
}

void DescriptorManager::loadFrom(char *src) {
    std::memcpy((char *)&blocksAvailable, src, sizeof(unsigned));
    src += sizeof(unsigned);
    std::memcpy((char *)&descriptorCount, src,sizeof(unsigned));
    src += sizeof(unsigned);
    std::memcpy((char *)&descriptorsPerBlock, src,sizeof(unsigned));
}






