//
// Created by paracelsus on 3/18/2022.
//

#ifndef FILESYSTEM_SIMULATOR_SCATTEREDFILESYSTEM_H
#define FILESYSTEM_SIMULATOR_SCATTEREDFILESYSTEM_H


#include "../filesystem/Filesystem.h"
#include "../metadata-handlers/OccupationMap.h"
#include "../metadata-handlers/DescriptorManager.h"
#include "ScatteredFileDescriptor.h"
#include "OpenedScatteredFileDescriptor.h"

class ScatteredFilesystem : public Filesystem<OpenedScatteredFileDescriptor> {
private:
    OccupationMap occupationMap;
    DescriptorManager<ScatteredFileDescriptor> descriptorManager;
public:
    enum MountType {
        STANDARD_MOUNT
    };

    enum FormatType {
        STANDARD_FORMAT
    };

    explicit ScatteredFilesystem(PersistentStorage &persistentStorage, FormatType formatType);

    explicit ScatteredFilesystem(PersistentStorage &persistentStorage, MountType mountType);

    int open(std::string path) override;

    void close(unsigned int index) override;

    void create(std::string path, bool isDirectory, unsigned int blocksReserved) override;

    void read(unsigned int index, char *dst, unsigned int size) override;

    void write(unsigned int index, char *src, unsigned int size) override;

    void seek(unsigned int index, unsigned int position) override;

    void remove(std::string path) override;

    void persistMetadata() override;

    void printState() override;

protected:
    Directory getDirectory(unsigned int directoryIndex) override;

    void saveDirectory(Directory directory, unsigned int directoryIndex) override;
};


#endif //FILESYSTEM_SIMULATOR_SCATTEREDFILESYSTEM_H
