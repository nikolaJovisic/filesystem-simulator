//
// Created by paracelsus on 2/13/2022.
//

#ifndef FILESYSTEM_SIMULATOR_DESCRIPTORMANAGER_H
#define FILESYSTEM_SIMULATOR_DESCRIPTORMANAGER_H

#include "../persistent-storage/PersistentStorage.h"
#include "../continuous-filesystem/ContinuousFileDescriptor.h"

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
    unsigned addDescriptor(ContinuousFileDescriptor fileDescriptor);
    ContinuousFileDescriptor getDescriptor(unsigned index);
    void updateDescriptor(unsigned index, ContinuousFileDescriptor fileDescriptor);
    void printAllDescriptors();
    void serializeInMemory(char* dst);
};


#endif //FILESYSTEM_SIMULATOR_DESCRIPTORMANAGER_H
