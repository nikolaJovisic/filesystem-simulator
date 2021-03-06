//
// Created by paracelsus on 3/18/2022.
//

#ifndef FILESYSTEM_SIMULATOR_SCATTEREDFILESYSTEM_H
#define FILESYSTEM_SIMULATOR_SCATTEREDFILESYSTEM_H


#include <list>
#include "../filesystem/Filesystem.h"
#include "../metadata-handlers/OccupationMap.h"
#include "../metadata-handlers/DescriptorManager.h"
#include "ScatteredFileDescriptor.h"
#include "OpenedScatteredFileDescriptor.h"
#include "Slice.h"

class ScatteredFilesystem : public Filesystem<OpenedScatteredFileDescriptor> {
private:
    OccupationMap occupationMap;
    DescriptorManager<ScatteredFileDescriptor> descriptorManager;

    void readRaw(OpenedScatteredFileDescriptor &descriptor, char *dst, unsigned int size) override;

    void writeRaw(OpenedScatteredFileDescriptor &descriptor, char *src, unsigned int size) override;

    Directory getDirectory(unsigned int directoryIndex) override;

    void saveDirectory(Directory directory, unsigned int directoryIndex) override;

    void removeFromRecord(std::string &path, int index);

    std::list<Slice> indirectionSlices(unsigned *blocks, unsigned position, unsigned int size);

    std::list<Slice> getSlices(OpenedScatteredFileDescriptor &descriptor, unsigned int size);

    unsigned maxFilesize();

    void expandFile(OpenedScatteredFileDescriptor &descriptor, unsigned int size);

    void
    expandIndirectionBlock(IndirectionBlock &indirectionBlock, unsigned int blocksUsed, unsigned int blocksRequired);

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

    void create(std::string path, bool isDirectory, unsigned int blocksReserved = 0) override;

    void read(unsigned int index, char *dst, unsigned int size) override;

    void write(unsigned int index, char *src, unsigned int size) override;

    void seek(unsigned int index, unsigned int position) override;

    void remove(std::string path) override;

    void shorten(unsigned int index, unsigned int bytesToTrim);

    void persistMetadata() override;

    void printState() override;
};


#endif //FILESYSTEM_SIMULATOR_SCATTEREDFILESYSTEM_H
