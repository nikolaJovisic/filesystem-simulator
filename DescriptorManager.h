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
    DescriptorManager(PersistentStorage &persistentStorage, unsigned blocksAvailable);
    unsigned addDescriptor(FileDescriptor fileDescriptor);
    FileDescriptor getDescriptor(unsigned index);
    void updateDescriptor(unsigned index, FileDescriptor fileDescriptor);
    void printAllDescriptors();
};


#endif //FILESYSTEM_SIMULATOR_DESCRIPTORMANAGER_H
