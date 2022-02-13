//
// Created by paracelsus on 2/13/2022.
//

#ifndef FILESYSTEM_SIMULATOR_FILEMANAGEMENTLAYER_H
#define FILESYSTEM_SIMULATOR_FILEMANAGEMENTLAYER_H

#include "PersistentStorage.h"
#include "OpenedFileDescriptor.h"
#include <string>
#include <map>

class FileManagementLayer {
private:
    PersistentStorage persistentStorage;
    std::map<unsigned, OpenedFileDescriptor> openedFiles;
public:
    explicit FileManagementLayer(const PersistentStorage &persistentStorage);
    virtual unsigned open(std::string path) = 0;
    virtual bool close(unsigned index) = 0;
    virtual bool read(unsigned index, char *dst, unsigned size) = 0;
    virtual bool write(unsigned index, char *src, unsigned size) = 0;
    virtual bool seek(unsigned index, unsigned position) = 0;
};


#endif //FILESYSTEM_SIMULATOR_FILEMANAGEMENTLAYER_H
