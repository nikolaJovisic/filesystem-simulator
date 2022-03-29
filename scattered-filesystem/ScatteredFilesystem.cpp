//
// Created by paracelsus on 3/18/2022.
//

#include "ScatteredFilesystem.h"
#include "../filesystem/PathTransform.h"

int ScatteredFilesystem::open(std::string path) {

    if (path == DELIMITER_STRING) {
        openedFiles.emplace(0, OpenedScatteredFileDescriptor(descriptorManager.getDescriptor(0)));
        return 0;
    }

    auto directories = PathTransform::filePathDirectories(path);
    auto filename = PathTransform::filePathFile(path);

    auto directoryIndex = getLastDirectoryIndex(0, directories);
    auto directory = getDirectory(directoryIndex);

    auto fileIndex = directory.getIndex(filename);

    if (fileIndex == -1) throw std::invalid_argument("Invalid path.");
    if (openedFiles.contains(fileIndex)) throw std::invalid_argument("File already opened.");

    auto fileDescriptor = descriptorManager.getDescriptor(fileIndex);
    openedFiles.emplace(fileIndex, OpenedScatteredFileDescriptor(fileDescriptor));

    return fileIndex;
}

void ScatteredFilesystem::close(unsigned int index) {

}

void ScatteredFilesystem::create(std::string path, bool isDirectory, unsigned int blocksReserved) {

}

void ScatteredFilesystem::read(unsigned int index, char *dst, unsigned int size) {

}

void ScatteredFilesystem::write(unsigned int index, char *src, unsigned int size) {

}

void ScatteredFilesystem::seek(unsigned int index, unsigned int position) {

}

void ScatteredFilesystem::remove(std::string path) {

}

void ScatteredFilesystem::printState() {
    Filesystem::printState();
}

void ScatteredFilesystem::persistMetadata() {

}

Directory ScatteredFilesystem::getDirectory(unsigned int directoryIndex) {
    return Directory();
}

void ScatteredFilesystem::saveDirectory(Directory directory, unsigned int directoryIndex) {

}

ScatteredFilesystem::ScatteredFilesystem(PersistentStorage &persistentStorage,
                                         ScatteredFilesystem::FormatType formatType) : Filesystem(persistentStorage),
                                                                                       occupationMap(
                                                                                               persistentStorage.getNumberOfBlocks() *
                                                                                               DESCRIPTORS_MEMORY_SHARE,
                                                                                               persistentStorage.getNumberOfBlocks() -
                                                                                               NUMBER_OF_BLOCKS_RESERVED_FOR_FILESYSTEM_METADATA),
                                                                                       descriptorManager(
                                                                                               persistentStorage,
                                                                                               persistentStorage.getNumberOfBlocks() *
                                                                                               DESCRIPTORS_MEMORY_SHARE) {
    ScatteredFileDescriptor rootDirectoryDescriptor = ScatteredFileDescriptor(true);
    descriptorManager.addDescriptor(rootDirectoryDescriptor);
}

ScatteredFilesystem::ScatteredFilesystem(PersistentStorage &persistentStorage,
                                         ScatteredFilesystem::MountType mountType) : Filesystem(persistentStorage),
                                                                                     descriptorManager(
                                                                                             persistentStorage) {

}

void ScatteredFilesystem::readRaw(OpenedScatteredFileDescriptor &descriptor, char *dst, unsigned int size) {

}

void ScatteredFilesystem::writeRaw(OpenedScatteredFileDescriptor &descriptor, char *src, unsigned int size) {

}
