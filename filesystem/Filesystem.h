//
// Created by paracelsus on 2/13/2022.
//

#ifndef FILESYSTEM_SIMULATOR_FILESYSTEM_H
#define FILESYSTEM_SIMULATOR_FILESYSTEM_H

#include "../persistent-storage/PersistentStorage.h"
#include "../continuous-filesystem/OpenedContinuousFileDescriptor.h"
#include <string>
#include <map>

class Filesystem {
protected:
    PersistentStorage& persistentStorage;
    std::map<unsigned, OpenedContinuousFileDescriptor> openedFiles;
public:
    explicit Filesystem(PersistentStorage &persistentStorage);
    virtual int open(std::string path) = 0;
    virtual void close(unsigned index) = 0;
    virtual void create(std::string path, bool isDirectory, unsigned blocksReserved) = 0;
    virtual void read(unsigned index, char *dst, unsigned size) = 0;
    virtual void write(unsigned index, char *src, unsigned size) = 0;
    virtual void seek(unsigned index, unsigned position) = 0;
    virtual void remove(std::string path) = 0;

    virtual void serialize(std::string path) = 0;

    virtual void listContentsAt(std::string path) = 0;
    virtual void printState();
};


#endif //FILESYSTEM_SIMULATOR_FILESYSTEM_H
