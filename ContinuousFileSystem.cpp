//
// Created by paracelsus on 2/13/2022.
//

#include "ContinuousFileSystem.h"
#include "Directory.h"
#include "Constants.h"

unsigned int ContinuousFileSystem::open(std::string path) {
    return 0;
}

bool ContinuousFileSystem::close(unsigned int index) {
    return false;
}

bool ContinuousFileSystem::read(unsigned int index, char *dst, unsigned int size) {
    return false;
}

bool ContinuousFileSystem::write(unsigned int index, char *src, unsigned int size) {
    return false;
}

bool ContinuousFileSystem::seek(unsigned int index, unsigned int position) {
    return false;
}

ContinuousFileSystem::ContinuousFileSystem(const PersistentStorage &persistentStorage) : FileManagementLayer(persistentStorage){

    unsigned totalNumberOfBlocks = persistentStorage.getNumberOfBlocks();
    unsigned dedicateToDescriptorZone = totalNumberOfBlocks * DESCRIPTORS_MEMORY_SHARE;

    occupationMap = OccupationMap(dedicateToDescriptorZone, totalNumberOfBlocks);
    descriptorMemoryManager = DescriptorMemoryManager(persistentStorage, dedicateToDescriptorZone);

    
}


