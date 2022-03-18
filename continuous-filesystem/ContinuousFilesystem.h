//
// Created by paracelsus on 2/13/2022.
//

#ifndef FILESYSTEM_SIMULATOR_CONTINUOUSFILESYSTEM_H
#define FILESYSTEM_SIMULATOR_CONTINUOUSFILESYSTEM_H

#include "../filesystem/Filesystem.h"
#include "../metadata-handlers/OccupationMap.h"
#include "../metadata-handlers/DescriptorManager.h"
#include "../filesystem/Directory.h"

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

    void printState() override;

private:
    void persistMetadata() override;

    Directory getDirectory(unsigned directoryIndex) override;

    void addToDirectory(std::deque<std::string>& directories, std::string fileName, unsigned fileIndex);

    void removeFromDirectory(std::deque<std::string> &directories, std::string fileName);

    unsigned getLastDirectoryIndex(unsigned startingDirectoryIndex, std::deque<std::string> &directories);

    void saveDirectory(Directory directory, unsigned directoryIndex) override;

    void removeFromRecord(const std::string &path, int index);
};


#endif //FILESYSTEM_SIMULATOR_CONTINUOUSFILESYSTEM_H
