//
// Created by paracelsus on 3/18/2022.
//

#include "ScatteredFilesystem.h"
#include "../filesystem/PathTransform.h"
#include "../persistent-storage/PersistentStorageController.h"

std::list<Slice> ScatteredFilesystem::getFirstReadSlices(unsigned block, unsigned position, unsigned int readSize) {
    unsigned blockSize = persistentStorage.blockSize();
    auto tableBlockSize = blockSize / sizeof(unsigned);
    auto startOffset = position % blockSize;
    auto blocksToRead = (startOffset + readSize) / blockSize +
                        (((startOffset + readSize) % blockSize != 0) ? 1 : 0);
    auto firstBlock = position / blockSize;
    bool dataExcedesThisBlock = firstBlock + blocksToRead > tableBlockSize;
    auto blockNumsToRead = dataExcedesThisBlock ? tableBlockSize - firstBlock : blocksToRead;
    auto bytesInLastBlock = blockSize - (blockSize * blocksToRead - readSize - position % blockSize);

    unsigned blockNums[blockSize];
    PersistentStorageController::read(persistentStorage, block, firstBlock * sizeof(unsigned), (char *) blockNums,
                                      blockNumsToRead * sizeof(unsigned));

    std::list<Slice> retVal;
    if (blockNumsToRead == 1) {
        retVal.push_back(
                {.block = blockNums[0], .start = startOffset, .length = dataExcedesThisBlock ? blockSize - startOffset
                                                                                             : bytesInLastBlock});
    } else {
        retVal.push_back({.block = blockNums[0], .start = startOffset, .length = blockSize - startOffset});
        for (int i = 1; i < blockNumsToRead - 1; ++i) {
            retVal.push_back({.block = blockNums[i], .start = 0, .length = blockSize});
        }
        retVal.push_back(
                {.block = blockNums[blockNumsToRead - 1], .start = 0, .length = dataExcedesThisBlock ? blockSize
                                                                                                     : bytesInLastBlock});
    }
    return retVal;
}

void ScatteredFilesystem::appendSecondReadSlices(std::list<Slice> &slices, unsigned block, unsigned position,
                                                 unsigned int readSize) {

}

void ScatteredFilesystem::appendThirdReadSlices(std::list<Slice> &slices, unsigned block, unsigned position,
                                                unsigned int readSize) {


}

std::list<Slice> ScatteredFilesystem::getReadSlices(OpenedScatteredFileDescriptor &descriptor,
                                                    unsigned int readSize) {
    auto slices = getFirstReadSlices(descriptor.getBlock(0), descriptor.getPosition(), readSize);
    appendSecondReadSlices(slices, descriptor.getBlock(1), descriptor.getPosition(), readSize);
    appendThirdReadSlices(slices, descriptor.getBlock(2), descriptor.getPosition(), readSize);
    return slices;
}

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
    if (!openedFiles.contains(index)) throw std::invalid_argument("Invalid index.");
    descriptorManager.updateDescriptor(index, openedFiles.at(index));
    openedFiles.erase(index);
}

void ScatteredFilesystem::create(std::string path, bool isDirectory, unsigned int blocksReserved) {
    auto fileDescriptor = ScatteredFileDescriptor(isDirectory);
    unsigned fileIndex = descriptorManager.addDescriptor(fileDescriptor);
    auto directories = PathTransform::filePathDirectories(path);
    auto fileName = PathTransform::filePathFile(path);
    addToDirectory(directories, fileName, fileIndex);
}

void ScatteredFilesystem::read(unsigned int index, char *dst, unsigned int size) {
    if (!openedFiles.contains(index)) throw std::invalid_argument("Invalid index.");
    auto &descriptor = openedFiles.at(index);
    readRaw(descriptor, dst, size);
    descriptor.setPosition(descriptor.getPosition() + size);
}

void ScatteredFilesystem::write(unsigned int index, char *src, unsigned int size) {
    if (!openedFiles.contains(index)) throw std::invalid_argument("Invalid index.");
    auto &descriptor = openedFiles.at(index);
    writeRaw(descriptor, src, size);
    descriptor.setPosition(descriptor.getPosition() + size);
    descriptor.setSize(std::max(descriptor.getSize(), descriptor.getPosition()));
}

void ScatteredFilesystem::seek(unsigned int index, unsigned int position) {
    if (!openedFiles.contains(index)) throw std::invalid_argument("Invalid index.");
    auto &descriptor = openedFiles.at(index);
    if (position > descriptor.getSize())
        throw std::invalid_argument("Position surpasses file size.");
    descriptor.setPosition(position);
}

void ScatteredFilesystem::removeFromRecord(std::string &path, int index) {

}

void ScatteredFilesystem::remove(std::string path) {
    auto index = open(path);
    removeFromRecord(path, index);
    auto directories = PathTransform::filePathDirectories(path);
    auto fileName = PathTransform::filePathFile(path);
    removeFromDirectory(directories, fileName);
    openedFiles.erase(index);
}

void ScatteredFilesystem::readRaw(OpenedScatteredFileDescriptor &descriptor, char *dst, unsigned int size) {
    if (descriptor.getPosition() + size > descriptor.getSize()) throw std::runtime_error("Reading out of bounds.");
    auto readSlices = getReadSlices(descriptor, size);
}

void ScatteredFilesystem::writeRaw(OpenedScatteredFileDescriptor &descriptor, char *src, unsigned int size) {
    if (descriptor.getPosition() + size > descriptor.getSize()) throw std::runtime_error("Out of reserved memory.");
}

void ScatteredFilesystem::printState() {
    Filesystem::printState();
}

void ScatteredFilesystem::persistMetadata() {
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

Directory ScatteredFilesystem::getDirectory(unsigned directoryIndex) {
    auto descriptor = descriptorManager.getDescriptor(directoryIndex);

    if (!descriptor.isDirectory()) throw std::invalid_argument("File is not a directory.");

    unsigned size = descriptor.getSize();

    char directoryContent[size];
    OpenedScatteredFileDescriptor openedDescriptor(descriptor);
    readRaw(openedDescriptor, directoryContent, size);
    return Directory(directoryContent, size);
}

void ScatteredFilesystem::saveDirectory(Directory directory, unsigned directoryIndex) {
    auto descriptor = descriptorManager.getDescriptor(directoryIndex);

    char directoryContent[directory.size()];
    directory.serialize(directoryContent);
    OpenedScatteredFileDescriptor openedDescriptor(descriptor);
    writeRaw(openedDescriptor, directoryContent, directory.size());

    descriptor.setSize(directory.size());
    descriptorManager.updateDescriptor(directoryIndex, descriptor);
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
    if (persistentStorage.blockSize() % sizeof(unsigned) != 0)
        throw std::runtime_error("Block size must be divisible by sizeof(unsigned)");
    auto rootDirectoryDescriptor = ScatteredFileDescriptor(true);
    auto rootIndex = descriptorManager.addDescriptor(rootDirectoryDescriptor);
    if (rootIndex != 0) throw std::runtime_error("Error in formatting.");
}

ScatteredFilesystem::ScatteredFilesystem(PersistentStorage &persistentStorage,
                                         ScatteredFilesystem::MountType mountType) : Filesystem(persistentStorage),
                                                                                     occupationMap(),
                                                                                     descriptorManager(
                                                                                             persistentStorage) {
    if (persistentStorage.blockSize() % sizeof(unsigned) != 0)
        throw std::runtime_error("Block size must be divisible by sizeof(unsigned)");
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




