//
// Created by paracelsus on 2/13/2022.
//

#ifndef FILESYSTEM_SIMULATOR_CONTINUOUSFILESYSTEM_H
#define FILESYSTEM_SIMULATOR_CONTINUOUSFILESYSTEM_H

#include <vector>
#include "FileManagementLayer.h"
#include "OccupationMap.h"
#include "DescriptorMemoryManager.h"

class ContinuousFileSystem : FileManagementLayer {
private:
    OccupationMap occupationMap;
    DescriptorMemoryManager descriptorMemoryManager;
public:
    ContinuousFileSystem(const PersistentStorage &persistentStorage);

    unsigned int open(std::string path) override;

    bool close(unsigned int index) override;

    bool read(unsigned int index, char *dst, unsigned int size) override;

    bool write(unsigned int index, char *src, unsigned int size) override;

    bool seek(unsigned int index, unsigned int position) override;
};


#endif //FILESYSTEM_SIMULATOR_CONTINUOUSFILESYSTEM_H
