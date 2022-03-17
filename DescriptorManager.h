//
// Created by paracelsus on 2/13/2022.
//

#ifndef FILESYSTEM_SIMULATOR_DESCRIPTORMANAGER_H
#define FILESYSTEM_SIMULATOR_DESCRIPTORMANAGER_H

#include "PersistentStorage.h"
#include "FileDescriptor.h"

class DescriptorManager {
private:
    PersistentStorage &persistentStorage;
    unsigned descriptorCount;
    unsigned blocksAvailable;
    unsigned descriptorsPerBlock;
public:
    explicit DescriptorManager(PersistentStorage &persistentStorage);
    DescriptorManager(PersistentStorage &persistentStorage, unsigned blocksAvailable);
    void loadFrom(char* src);
    unsigned addDescriptor(FileDescriptor fileDescriptor);
    FileDescriptor getDescriptor(unsigned index);
    void updateDescriptor(unsigned index, FileDescriptor fileDescriptor);
    void printAllDescriptors();
    void serialize(char* dst);
};


#endif //FILESYSTEM_SIMULATOR_DESCRIPTORMANAGER_H
