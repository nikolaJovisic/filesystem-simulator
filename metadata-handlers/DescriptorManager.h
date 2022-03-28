//
// Created by paracelsus on 2/13/2022.
//

#ifndef FILESYSTEM_SIMULATOR_DESCRIPTORMANAGER_H
#define FILESYSTEM_SIMULATOR_DESCRIPTORMANAGER_H

#include "../persistent-storage/PersistentStorage.h"
#include <stdexcept>
#include <iostream>
#include <cstring>

template<class FileDescriptorType>
class DescriptorManager {
private:
    PersistentStorage &persistentStorage;
    unsigned descriptorCount;
    unsigned blocksAvailable;
    unsigned descriptorsPerBlock;
public:
    DescriptorManager(PersistentStorage &persistentStorage, unsigned int blocksAvailable) : persistentStorage(persistentStorage), blocksAvailable(blocksAvailable){
        descriptorsPerBlock = persistentStorage.blockSize() / sizeof(FileDescriptorType);
        descriptorCount = 0;
    }

    DescriptorManager(PersistentStorage &persistentStorage) : persistentStorage(persistentStorage) {
    }

    unsigned addDescriptor(FileDescriptorType fileDescriptor) {
        if (descriptorCount == blocksAvailable * descriptorsPerBlock) {
            throw std::runtime_error("Out of descriptor dedicated memory.");
        }
        auto block = descriptorCount / descriptorsPerBlock;
        char content[persistentStorage.blockSize()];
        persistentStorage.read(block, content);
        auto positionInBlock = descriptorCount % descriptorsPerBlock;
        std::memcpy(content + positionInBlock * sizeof(FileDescriptorType), &fileDescriptor, sizeof(FileDescriptorType));
        persistentStorage.write(block, content);
        return descriptorCount++;
    }

    FileDescriptorType getDescriptor(unsigned int index) {
        if (index >= descriptorCount) {
            throw std::runtime_error("Index out of descriptor bounds.");
        }
        auto block = index / descriptorsPerBlock;
        char content[persistentStorage.blockSize()];
        persistentStorage.read(block, content);
        auto positionInBlock = index % descriptorsPerBlock;
        return *reinterpret_cast<FileDescriptorType *>(content + positionInBlock * sizeof(FileDescriptorType));
    }

    void updateDescriptor(unsigned int index, FileDescriptorType fileDescriptor) {
        if (index >= descriptorCount) {
            throw std::runtime_error("Index out of descriptor bounds.");
        }
        auto block = index / descriptorsPerBlock;
        char content[persistentStorage.blockSize()];
        persistentStorage.read(block, content);
        auto positionInBlock = index % descriptorsPerBlock;
        std::memcpy(content + positionInBlock * sizeof(FileDescriptorType), &fileDescriptor, sizeof(FileDescriptorType));
        persistentStorage.write(block, content);
    }

    void printAllDescriptors() {
        for (int i = 0; i < descriptorCount; ++i) {
            std::cout<<i<<": "<<getDescriptor(i)<<std::endl;
        }
    }

    void serializeInMemory(char *dst) {
        std::memcpy(dst, (char *)&blocksAvailable, sizeof(unsigned));
        dst += sizeof(unsigned);
        std::memcpy(dst, (char *)&descriptorCount, sizeof(unsigned));
        dst += sizeof(unsigned);
        std::memcpy(dst, (char *)&descriptorsPerBlock, sizeof(unsigned));
    }

    void loadFrom(char *src) {
        std::memcpy((char *)&blocksAvailable, src, sizeof(unsigned));
        src += sizeof(unsigned);
        std::memcpy((char *)&descriptorCount, src,sizeof(unsigned));
        src += sizeof(unsigned);
        std::memcpy((char *)&descriptorsPerBlock, src,sizeof(unsigned));
    }
};


#endif //FILESYSTEM_SIMULATOR_DESCRIPTORMANAGER_H
