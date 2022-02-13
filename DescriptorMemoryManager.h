//
// Created by paracelsus on 2/13/2022.
//

#ifndef FILESYSTEM_SIMULATOR_DESCRIPTORMEMORYMANAGER_H
#define FILESYSTEM_SIMULATOR_DESCRIPTORMEMORYMANAGER_H

#include "PersistentStorage.h"
#include "FileDescriptor.h"

class DescriptorMemoryManager {
private:
    PersistentStorage persistentStorage;
    unsigned descriptorCount;
    unsigned blocksAvailable;
    unsigned descriptorsPerBlock;
public:
    DescriptorMemoryManager(const PersistentStorage &persistentStorage, unsigned blocksAvailable);
    DescriptorMemoryManager();
    void addDescriptor(FileDescriptor fileDescriptor);
    FileDescriptor getDescriptor(unsigned index);
    void printAllDescriptors();
};


#endif //FILESYSTEM_SIMULATOR_DESCRIPTORMEMORYMANAGER_H
