//
// Created by paracelsus on 2/13/2022.
//

#include "DescriptorMemoryManager.h"
#include <stdexcept>
#include <iostream>

DescriptorMemoryManager::DescriptorMemoryManager() {
    blocksAvailable = 0;
    descriptorCount = 0;
    descriptorsPerBlock = 0;
}

DescriptorMemoryManager::DescriptorMemoryManager(const PersistentStorage &persistentStorage, unsigned int blocksAvailable) {
    this->persistentStorage = persistentStorage;
    this->blocksAvailable = blocksAvailable;
    descriptorsPerBlock = persistentStorage.getBytesPerBlock() / sizeof(FileDescriptor);
    descriptorCount = 0;

}

void DescriptorMemoryManager::addDescriptor(FileDescriptor fileDescriptor) {
    if (descriptorCount == blocksAvailable * descriptorsPerBlock) {
        throw std::runtime_error("Out of descriptor dedicated memory.");
    }
    auto block = descriptorCount / descriptorsPerBlock;
    char content[persistentStorage.getBytesPerBlock()];
    persistentStorage.read(block, content);
    auto positionInBlock = descriptorCount % descriptorsPerBlock;
    memcpy(content + positionInBlock * sizeof(FileDescriptor), &fileDescriptor, sizeof(FileDescriptor));
    persistentStorage.write(block, content);
    ++descriptorCount;
}

FileDescriptor DescriptorMemoryManager::getDescriptor(unsigned int index) {
    if (index >= descriptorCount) {
        throw std::runtime_error("Index out of descriptor bounds.");
    }
    auto block = index / descriptorsPerBlock;
    char content[persistentStorage.getBytesPerBlock()];
    persistentStorage.read(block, content);
    auto positionInBlock = index % descriptorsPerBlock;
    return *reinterpret_cast<FileDescriptor *>(content + positionInBlock * sizeof(FileDescriptor));
}

void DescriptorMemoryManager::printAllDescriptors() {
    for (int i = 0; i < descriptorCount; ++i) {
        std::cout<<i<<": starting block = "<<getDescriptor(i).getStartingBlock()<<std::endl;
    }
}


