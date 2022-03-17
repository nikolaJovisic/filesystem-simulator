//
// Created by paracelsus on 2/13/2022.
//

#include "ContinuousFilesystem.h"
#include "Directory.h"
#include "Constants.h"
#include "PathTransform.h"
#include "PersistentStorageController.h"

#include <stdexcept>
#include <iostream>

int ContinuousFilesystem::open(std::string path) {
    if (path == std::string(1, DELIMITER)) {
        openedFiles.emplace(0, OpenedFileDescriptor(descriptorManager.getDescriptor(0)));
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
    openedFiles.emplace(fileIndex, OpenedFileDescriptor(fileDescriptor));

    return fileIndex;
}


void ContinuousFilesystem::close(unsigned int index) {
    if (!openedFiles.contains(index)) throw std::invalid_argument("Invalid index.");
    descriptorManager.updateDescriptor(index, openedFiles.at(index));
    openedFiles.erase(index);
}


void ContinuousFilesystem::create(std::string path, bool isDirectory, unsigned int blocksReserved) {
    unsigned startingBlock = occupationMap.occupy(blocksReserved);
    FileDescriptor fileDescriptor = FileDescriptor(isDirectory, startingBlock, blocksReserved, 0);
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
    auto& descriptor = openedFiles.at(index);
    if (descriptor.isDirectory()) {
        auto directory =  getDirectory(index);
        for(const auto& filename: directory.getAllFilenames()) {
            auto filePath = path + DELIMITER + filename;
            removeFromRecord(filePath, open(filePath));
        }
    }
    occupationMap.free(descriptor.getStartingBlock(), descriptor.getBlocksReserved());
    descriptor.markDeleted();
    descriptor.setUsedSpace(0);
    descriptorManager.updateDescriptor(index, descriptor);
}

void ContinuousFilesystem::read(unsigned int index, char *dst, unsigned int size) {
    if (!openedFiles.contains(index)) throw std::invalid_argument("Invalid index.");
    auto& descriptor = openedFiles.at(index);
    if (descriptor.getPosition() + size > descriptor.getUsedSpace()) throw std::runtime_error("Reading out of bounds.");
    PersistentStorageController::read(persistentStorage, descriptor.getStartingBlock(), descriptor.getPosition(), dst, size);
    descriptor.setPosition(descriptor.getPosition() + size);
}

void ContinuousFilesystem::write(unsigned int index, char *src, unsigned int size) {
    if (!openedFiles.contains(index)) throw std::invalid_argument("Invalid index.");
    auto& descriptor = openedFiles.at(index);
    if (descriptor.getPosition() + size > descriptor.getBlocksReserved() * persistentStorage.blockSize()) throw std::runtime_error("Out of reserved memory.");
    PersistentStorageController::write(persistentStorage, descriptor.getStartingBlock(), descriptor.getPosition(), src, size);
    descriptor.setPosition(descriptor.getPosition() + size);
    descriptor.setUsedSpace(std::max(descriptor.getUsedSpace(), descriptor.getPosition()));
}

void ContinuousFilesystem::seek(unsigned int index, unsigned int position) {
    if (!openedFiles.contains(index)) throw std::invalid_argument("Invalid index.");
    auto& descriptor = openedFiles.at(index);
    if (position > descriptor.getBlocksReserved() * persistentStorage.blockSize()) throw std::invalid_argument("Position surpasses file size.");
    descriptor.setPosition(position);
}

ContinuousFilesystem::ContinuousFilesystem(PersistentStorage &persistentStorage) : Filesystem(
        persistentStorage),
                                                                                   occupationMap(
                                                                                           persistentStorage.getNumberOfBlocks() *
                                                                                           DESCRIPTORS_MEMORY_SHARE,
                                                                                           persistentStorage.getNumberOfBlocks() - NUMBER_OF_BLOCKS_RESERVED_FOR_FILESYSTEM_METADATA),
                                                                                   descriptorManager(persistentStorage,
                                                                                                     persistentStorage.getNumberOfBlocks() *
                                                                                                     DESCRIPTORS_MEMORY_SHARE) {

    auto rootStartingBlock = occupationMap.occupy(NUMBER_OF_BLOCKS_RESERVED_FOR_ROOT_DIRECTORY);
    FileDescriptor rootDirectoryDescriptor = FileDescriptor(true, rootStartingBlock,
                                                            NUMBER_OF_BLOCKS_RESERVED_FOR_ROOT_DIRECTORY, 0);
    descriptorManager.addDescriptor(rootDirectoryDescriptor);
}

ContinuousFilesystem::ContinuousFilesystem(PersistentStorage &persistentStorage, MountType mountType): Filesystem(
        persistentStorage), occupationMap(), descriptorManager(persistentStorage){
    unsigned metadataLength = NUMBER_OF_BLOCKS_RESERVED_FOR_FILESYSTEM_METADATA * persistentStorage.blockSize();
    unsigned metadataStartingBlock = persistentStorage.getNumberOfBlocks() - NUMBER_OF_BLOCKS_RESERVED_FOR_FILESYSTEM_METADATA;
    char metadata[metadataLength];
    PersistentStorageController::read(persistentStorage, metadataStartingBlock, 0, metadata, metadataLength);
    char* readingPointer = metadata;
    occupationMap.loadFrom(readingPointer);
    readingPointer += occupationMap.serializationSize();
    descriptorManager.loadFrom(readingPointer);
}

void ContinuousFilesystem::addToDirectory(std::deque<std::string>& directories, std::string fileName,
                                          unsigned int fileIndex) {
    auto directoryIndex = getLastDirectoryIndex(0, directories);
    auto directory = getDirectory(directoryIndex);
    directory.addFile(fileName, fileIndex);
    saveDirectory(directory, directoryIndex);
}

void ContinuousFilesystem::removeFromDirectory(std::deque<std::string>& directories, std::string fileName) {
    auto directoryIndex = getLastDirectoryIndex(0, directories);
    auto directory = getDirectory(directoryIndex);
    directory.removeFile(fileName);
    saveDirectory(directory, directoryIndex);
}

unsigned ContinuousFilesystem::getLastDirectoryIndex(unsigned startingDirectoryIndex, std::deque<std::string> &directories) {
    if (directories.empty()) return startingDirectoryIndex;

    Directory directory = getDirectory(startingDirectoryIndex);

    auto index = directory.getIndex(directories.front());
    directories.pop_front();
    getLastDirectoryIndex(index, directories);

    return getLastDirectoryIndex(index, directories);
}

Directory ContinuousFilesystem::getDirectory(unsigned directoryIndex) {
    FileDescriptor directoryDescriptor = descriptorManager.getDescriptor(directoryIndex);

    if (!directoryDescriptor.isDirectory()) throw std::invalid_argument("File is not a directory.");

    unsigned startingBlock = directoryDescriptor.getStartingBlock();
    unsigned directoryLength = directoryDescriptor.getUsedSpace();

    char directoryContent[directoryDescriptor.getBlocksReserved() * persistentStorage.blockSize()];
    PersistentStorageController::read(persistentStorage, startingBlock, 0, directoryContent, directoryLength);
    return Directory(directoryContent, directoryLength);
}

void ContinuousFilesystem::saveDirectory(Directory directory, unsigned directoryIndex) {
    FileDescriptor directoryDescriptor = descriptorManager.getDescriptor(directoryIndex);

    if (directory.size() > directoryDescriptor.getBlocksReserved() * persistentStorage.blockSize()) throw std::runtime_error("Directory size surpasses maximum.");

    char directoryContent[directory.size()];
    directory.serialize(directoryContent);
    PersistentStorageController::write(persistentStorage, directoryDescriptor.getStartingBlock(), 0, directoryContent, directory.size());

    directoryDescriptor.setUsedSpace(directory.size());
    descriptorManager.updateDescriptor(directoryIndex, directoryDescriptor);
}

void ContinuousFilesystem::printState() {
    Filesystem::printState();
    std::cout<<"FREE BLOCKS:"<<std::endl;
    occupationMap.print();
    std::cout<<"ALL DESCRIPTORS:"<<std::endl;
    descriptorManager.printAllDescriptors();
}

void ContinuousFilesystem::listContentsAt(std::string path) {
    std::cout<<"-------------------"<<std::endl;
    auto index = open(path);
    auto filenames = getDirectory(index).getAllFilenames();
    close(index);
    for(auto i: filenames) {
        std::cout<<i<<std::endl;
    }
}

void ContinuousFilesystem::serialize(std::string filename) {
    unsigned metadataLength = NUMBER_OF_BLOCKS_RESERVED_FOR_FILESYSTEM_METADATA * persistentStorage.blockSize();
    unsigned metadataStartingBlock = persistentStorage.getNumberOfBlocks() - NUMBER_OF_BLOCKS_RESERVED_FOR_FILESYSTEM_METADATA;
    char metadata[metadataLength];
    char* writingPointer = metadata;
    occupationMap.serialize(writingPointer);
    writingPointer += occupationMap.serializationSize();
    descriptorManager.serialize(writingPointer);
    PersistentStorageController::write(persistentStorage, metadataStartingBlock, 0, metadata, metadataLength);
    persistentStorage.serialize(filename);
}













