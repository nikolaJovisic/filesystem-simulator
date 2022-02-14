//
// Created by paracelsus on 2/13/2022.
//

#include "DescriptorManager.h"
#include <stdexcept>
#include <iostream>

DescriptorManager::DescriptorManager() {
    blocksAvailable = 0;
    descriptorCount = 0;
    descriptorsPerBlock = 0;
}

DescriptorManager::DescriptorManager(const PersistentStorage &persistentStorage, unsigned int blocksAvailable) {
    this->persistentStorage = persistentStorage;
    this->blocksAvailable = blocksAvailable;
    descriptorsPerBlock = persistentStorage.blockSize() / sizeof(FileDescriptor);
    descriptorCount = 0;
}

unsigned DescriptorManager::addDescriptor(FileDescriptor fileDescriptor) {
    if (descriptorCount == blocksAvailable * descriptorsPerBlock) {
        throw std::runtime_error("Out of descriptor dedicated memory.");
    }
    auto block = descriptorCount / descriptorsPerBlock;
    char content[persistentStorage.blockSize()];
    persistentStorage.read(block, content);
    auto positionInBlock = descriptorCount % descriptorsPerBlock;
    memcpy(content + positionInBlock * sizeof(FileDescriptor), &fileDescriptor, sizeof(FileDescriptor));
    persistentStorage.write(block, content);
    return descriptorCount++;
}

FileDescriptor DescriptorManager::getDescriptor(unsigned int index) {
    if (index >= descriptorCount) {
        throw std::runtime_error("Index out of descriptor bounds.");
    }
    auto block = index / descriptorsPerBlock;
    char content[persistentStorage.blockSize()];
    persistentStorage.read(block, content);
    auto positionInBlock = index % descriptorsPerBlock;
    return *reinterpret_cast<FileDescriptor *>(content + positionInBlock * sizeof(FileDescriptor));
}

void DescriptorManager::updateDescriptor(unsigned int index, FileDescriptor fileDescriptor) {
    if (index >= descriptorCount) {
        throw std::runtime_error("Index out of descriptor bounds.");
    }
    auto block = index / descriptorsPerBlock;
    char content[persistentStorage.blockSize()];
    persistentStorage.read(block, content);
    auto positionInBlock = index % descriptorsPerBlock;
    memcpy(content + positionInBlock * sizeof(FileDescriptor), &fileDescriptor, sizeof(FileDescriptor));
    persistentStorage.write(block, content);
}

void DescriptorManager::printAllDescriptors() {
    for (int i = 0; i < descriptorCount; ++i) {
        std::cout<<i<<": starting block = "<<getDescriptor(i).getStartingBlock()<<std::endl;
    }
}


