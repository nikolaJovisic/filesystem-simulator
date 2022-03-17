//
// Created by paracelsus on 2/13/2022.
//

#ifndef FILESYSTEM_SIMULATOR_CONTINUOUSFILESYSTEM_H
#define FILESYSTEM_SIMULATOR_CONTINUOUSFILESYSTEM_H

#include "Filesystem.h"
#include "OccupationMap.h"
#include "DescriptorManager.h"
#include "Directory.h"

#include <deque>

class ContinuousFilesystem : public Filesystem {
private:
    OccupationMap occupationMap;
    DescriptorManager descriptorManager;
public:
    enum MountType {
        BACK
    };
    explicit ContinuousFilesystem(PersistentStorage &persistentStorage);

    explicit ContinuousFilesystem(PersistentStorage &persistentStorage, MountType mountType);

    int open(std::string path) override;

    void close(unsigned int index) override;

    void create(std::string path, bool isDirectory, unsigned blocksReserved) override;

    void read(unsigned int index, char *dst, unsigned int size) override;

    void write(unsigned int index, char *src, unsigned int size) override;

    void seek(unsigned int index, unsigned int position) override;

    void remove(std::string path) override;

    void serialize(std::string filename) override;

    void listContentsAt(std::string path) override;

    void printState() override;

private:
    Directory getDirectory(unsigned directoryIndex);

    void addToDirectory(std::deque<std::string>& directories, std::string fileName, unsigned fileIndex);

    void removeFromDirectory(std::deque<std::string> &directories, std::string fileName);

    unsigned getLastDirectoryIndex(unsigned startingDirectoryIndex, std::deque<std::string> &directories);

    void saveDirectory(Directory directory, unsigned directoryIndex);

    void removeFromRecord(const std::string &path, int index);
};


#endif //FILESYSTEM_SIMULATOR_CONTINUOUSFILESYSTEM_H
