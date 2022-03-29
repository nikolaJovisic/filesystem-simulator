//
// Created by paracelsus on 2/13/2022.
//

#include "ContinuousFilesystem.h"
#include "../filesystem/Directory.h"
#include "../filesystem/Constants.h"
#include "../filesystem/PathTransform.h"
#include "../persistent-storage/PersistentStorageController.h"

#include <stdexcept>
#include <iostream>

int ContinuousFilesystem::open(std::string path) {
    if (path == DELIMITER_STRING) {
        openedFiles.emplace(0, OpenedContinuousFileDescriptor(descriptorManager.getDescriptor(0)));
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
    openedFiles.emplace(fileIndex, OpenedContinuousFileDescriptor(fileDescriptor));

    return fileIndex;
}


void ContinuousFilesystem::close(unsigned int index) {
    if (!openedFiles.contains(index)) throw std::invalid_argument("Invalid index.");
    descriptorManager.updateDescriptor(index, openedFiles.at(index));
    openedFiles.erase(index);
}


void ContinuousFilesystem::create(std::string path, bool isDirectory, unsigned int blocksReserved) {
    unsigned startingBlock = occupationMap.occupy(blocksReserved);
    ContinuousFileDescriptor fileDescriptor = ContinuousFileDescriptor(isDirectory, startingBlock, blocksReserved, 0);
    unsigned fileIndex = descriptorManager.addDescriptor(fileDescriptor);
    auto directories = PathTransform::filePathDirectories(path);
    auto fileName = PathTransform::filePathFile(path);
    addToDirectory(directories, fileName, fileIndex);
}

void ContinuousFilesystem::remove(std::string path) {
    auto index = open(path);
    removeFromRecord(path, index);
    auto directories = PathTransform::filePathDirectories(path);
    auto fileName = PathTransform::filePathFile(path);
    removeFromDirectory(directories, fileName);
    openedFiles.erase(index);
}

void ContinuousFilesystem::removeFromRecord(const std::string &path, int index) {
    auto &descriptor = openedFiles.at(index);
    if (descriptor.isDirectory()) {
        auto directory = getDirectory(index);
        for (const auto &filename: directory.getAllFilenames()) {
            auto filePath = path + DELIMITER + filename;
            removeFromRecord(filePath, open(filePath));
        }
    }
    occupationMap.free(descriptor.getStartingBlock(), descriptor.getBlocksReserved());
    descriptor.markDeleted();
    descriptor.setUsedSpace(0);
    descriptorManager.updateDescriptor(index, descriptor);
}

void ContinuousFilesystem::readRaw(OpenedContinuousFileDescriptor &descriptor, char *dst, unsigned int size) {
    if (descriptor.getPosition() + size > descriptor.getUsedSpace()) throw std::runtime_error("Reading out of bounds.");
    PersistentStorageController::read(persistentStorage, descriptor.getStartingBlock(), descriptor.getPosition(), dst,
                                      size);
}

void ContinuousFilesystem::writeRaw(OpenedContinuousFileDescriptor &descriptor, char *src, unsigned int size) {
    if (descriptor.getPosition() + size > descriptor.getBlocksReserved() * persistentStorage.blockSize())
        throw std::runtime_error("Out of reserved memory.");
    PersistentStorageController::write(persistentStorage, descriptor.getStartingBlock(), descriptor.getPosition(), src,
                                       size);
}

void ContinuousFilesystem::read(unsigned int index, char *dst, unsigned int size) {
    if (!openedFiles.contains(index)) throw std::invalid_argument("Invalid index.");
    auto &descriptor = openedFiles.at(index);
    readRaw(descriptor, dst, size);
    descriptor.setPosition(descriptor.getPosition() + size);
}

void ContinuousFilesystem::write(unsigned int index, char *src, unsigned int size) {
    if (!openedFiles.contains(index)) throw std::invalid_argument("Invalid index.");
    auto &descriptor = openedFiles.at(index);
    writeRaw(descriptor, src, size);
    descriptor.setPosition(descriptor.getPosition() + size);
    descriptor.setUsedSpace(std::max(descriptor.getUsedSpace(), descriptor.getPosition()));
}

void ContinuousFilesystem::seek(unsigned int index, unsigned int position) {
    if (!openedFiles.contains(index)) throw std::invalid_argument("Invalid index.");
    auto &descriptor = openedFiles.at(index);
    if (position > descriptor.getBlocksReserved() * persistentStorage.blockSize())
        throw std::invalid_argument("Position surpasses file size.");
    descriptor.setPosition(position);
}

ContinuousFilesystem::ContinuousFilesystem(PersistentStorage &persistentStorage, FormatType formatType) : Filesystem(
        persistentStorage),
                                                                                                          occupationMap(
                                                                                                                  persistentStorage.getNumberOfBlocks() *
                                                                                                                  DESCRIPTORS_MEMORY_SHARE,
                                                                                                                  persistentStorage.getNumberOfBlocks() -
                                                                                                                  NUMBER_OF_BLOCKS_RESERVED_FOR_FILESYSTEM_METADATA),
                                                                                                          descriptorManager(
                                                                                                                  persistentStorage,
                                                                                                                  persistentStorage.getNumberOfBlocks() *
                                                                                                                  DESCRIPTORS_MEMORY_SHARE) {

    auto rootStartingBlock = occupationMap.occupy(NUMBER_OF_BLOCKS_RESERVED_FOR_ROOT_DIRECTORY);
    ContinuousFileDescriptor rootDirectoryDescriptor = ContinuousFileDescriptor(true, rootStartingBlock,
                                                                                NUMBER_OF_BLOCKS_RESERVED_FOR_ROOT_DIRECTORY,
                                                                                0);
    auto rootIndex = descriptorManager.addDescriptor(rootDirectoryDescriptor);
    if (rootIndex != 0) throw std::runtime_error("Error in formatting.");
}

ContinuousFilesystem::ContinuousFilesystem(PersistentStorage &persistentStorage, MountType mountType) : Filesystem(
        persistentStorage), occupationMap(), descriptorManager(persistentStorage) {
    unsigned metadataLength = NUMBER_OF_BLOCKS_RESERVED_FOR_FILESYSTEM_METADATA * persistentStorage.blockSize();
    unsigned metadataStartingBlock =
            persistentStorage.getNumberOfBlocks() - NUMBER_OF_BLOCKS_RESERVED_FOR_FILESYSTEM_METADATA;
    char metadata[metadataLength];
    PersistentStorageController::read(persistentStorage, metadataStartingBlock, 0, metadata, metadataLength);
    char *readingPointer = metadata;
    occupationMap.loadFrom(readingPointer);
    readingPointer += occupationMap.serializationSize();
    descriptorManager.loadFrom(readingPointer);
}

Directory ContinuousFilesystem::getDirectory(unsigned directoryIndex) {
    ContinuousFileDescriptor descriptor = descriptorManager.getDescriptor(directoryIndex);

    if (!descriptor.isDirectory()) throw std::invalid_argument("File is not a directory.");

    unsigned size = descriptor.getUsedSpace();

    char directoryContent[descriptor.getBlocksReserved() * persistentStorage.blockSize()];
    OpenedContinuousFileDescriptor openedDescriptor(descriptor);
    readRaw(openedDescriptor, directoryContent, size);
    return Directory(directoryContent, size);
}

void ContinuousFilesystem::saveDirectory(Directory directory, unsigned directoryIndex) {
    ContinuousFileDescriptor descriptor = descriptorManager.getDescriptor(directoryIndex);

    char directoryContent[directory.size()];
    directory.serialize(directoryContent);
    OpenedContinuousFileDescriptor openedDescriptor(descriptor);
    writeRaw(openedDescriptor, directoryContent, directory.size());
    PersistentStorageController::write(persistentStorage, descriptor.getStartingBlock(), 0, directoryContent,
                                       directory.size());

    descriptor.setUsedSpace(directory.size());
    descriptorManager.updateDescriptor(directoryIndex, descriptor);
}

void ContinuousFilesystem::printState() {
    Filesystem::printState();
    std::cout << "FREE BLOCKS:" << std::endl;
    occupationMap.print();
    std::cout << "ALL DESCRIPTORS:" << std::endl;
    descriptorManager.printAllDescriptors();
}


void ContinuousFilesystem::persistMetadata() {
    unsigned metadataLength = NUMBER_OF_BLOCKS_RESERVED_FOR_FILESYSTEM_METADATA * persistentStorage.blockSize();
    unsigned metadataStartingBlock =
            persistentStorage.getNumberOfBlocks() - NUMBER_OF_BLOCKS_RESERVED_FOR_FILESYSTEM_METADATA;
    char metadata[metadataLength];
    char *writingPointer = metadata;
    occupationMap.serializeInMemory(writingPointer);
    writingPointer += occupationMap.serializationSize();
    descriptorManager.serializeInMemory(writingPointer);
    PersistentStorageController::write(persistentStorage, metadataStartingBlock, 0, metadata, metadataLength);
}















