//
// Created by paracelsus on 3/18/2022.
//

#include "ScatteredFilesystem.h"
#include "../filesystem/PathTransform.h"
#include "../persistent-storage/PersistentStorageController.h"

std::list<Slice>
ScatteredFilesystem::indirectionSlices(unsigned *blocks, unsigned position, unsigned int size) {
    if (size == 0) return {};
    unsigned blockSize = persistentStorage.blockSize();
    auto tableBlockSize = blockSize / sizeof(unsigned);
    auto startOffset = position % blockSize;
    auto blocksToRead = (startOffset + size) / blockSize +
                        (((startOffset + size) % blockSize != 0) ? 1 : 0);
    auto firstBlock = position / blockSize;
    if (firstBlock >= tableBlockSize) return {};
    bool dataExcedesThisBlock = firstBlock + blocksToRead > tableBlockSize;
    auto blockNumsToRead = dataExcedesThisBlock ? tableBlockSize - firstBlock : blocksToRead;
    auto bytesInLastBlock = blockSize - (blockSize * blocksToRead - size - position % blockSize);


    std::list<Slice> retVal;
    if (blockNumsToRead == 1) {
        retVal.push_back(
                {.block = blocks[0], .start = startOffset, .length = dataExcedesThisBlock ? blockSize - startOffset
                                                                                          : bytesInLastBlock});
    } else {
        retVal.push_back({.block = blocks[0], .start = startOffset, .length = blockSize - startOffset});
        for (int i = 1; i < blockNumsToRead - 1; ++i) {
            retVal.push_back({.block = blocks[i], .start = 0, .length = blockSize});
        }
        retVal.push_back(
                {.block = blocks[blockNumsToRead - 1], .start = 0, .length = dataExcedesThisBlock ? blockSize
                                                                                                  : bytesInLastBlock});
    }
    return retVal;
}

std::list<Slice> ScatteredFilesystem::getSlices(OpenedScatteredFileDescriptor &descriptor,
                                                unsigned int size) {
    return indirectionSlices(descriptor.indirectionBlock.blocks, descriptor.getPosition(), size);
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
    auto fileDescriptor = ScatteredFileDescriptor(isDirectory, occupationMap.occupy(1));
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
    descriptor.indirectionBlock.save(descriptor.getBlock());
}

void ScatteredFilesystem::seek(unsigned int index, unsigned int position) {
    if (!openedFiles.contains(index)) throw std::invalid_argument("Invalid index.");
    auto &descriptor = openedFiles.at(index);
    if (position > descriptor.getSize())
        throw std::invalid_argument("Position surpasses file size.");
    descriptor.setPosition(position);
}

void ScatteredFilesystem::shorten(unsigned int index, unsigned int bytesToTrim) {
    if (!openedFiles.contains(index)) throw std::invalid_argument("Invalid index.");
    auto &descriptor = openedFiles.at(index);
    if (descriptor.isDirectory()) throw std::invalid_argument("Cannot shorten directory!");
    auto blocksUsed = (descriptor.getSize() + NUMBER_OF_BYTES_PER_BLOCK - 1) / NUMBER_OF_BYTES_PER_BLOCK;
    auto blocksToTrim = bytesToTrim / NUMBER_OF_BYTES_PER_BLOCK;
    for (int i = blocksUsed - blocksToTrim; i < blocksUsed; ++i) {
        occupationMap.free(descriptor.indirectionBlock.getBlock(i), 1);
    }
    descriptor.setSize(descriptor.getSize() - bytesToTrim);
    descriptor.indirectionBlock.save(descriptor.getBlock());
    descriptorManager.updateDescriptor(index, descriptor);
}

void ScatteredFilesystem::removeFromRecord(std::string &path, int index) {
    auto &descriptor = openedFiles.at(index);
    if (descriptor.isDirectory()) {
        auto directory = getDirectory(index);
        for (const auto &filename: directory.getAllFilenames()) {
            auto filePath = path + DELIMITER + filename;
            removeFromRecord(filePath, open(filePath));
        }
    }
    occupationMap.free(descriptor.getBlock(), 1);
    auto blocksUsed = (descriptor.getSize() + NUMBER_OF_BYTES_PER_BLOCK - 1) / NUMBER_OF_BYTES_PER_BLOCK;
    for (int i = 0; i < blocksUsed; ++i) {
        occupationMap.free(descriptor.indirectionBlock.getBlock(i), 1);
    }
    descriptor.markDeleted();
    descriptor.setSize(0);
    descriptorManager.updateDescriptor(index, descriptor);
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
    auto readSlices = getSlices(descriptor, size);
    unsigned read = 0;
    for (auto slice: readSlices) {
        PersistentStorageController::read(slice.block, slice.start, dst + read, slice.length);
        read += slice.length;
    }
}

void ScatteredFilesystem::writeRaw(OpenedScatteredFileDescriptor &descriptor, char *src, unsigned int size) {
    if (descriptor.getPosition() + size > maxFilesize()) throw std::runtime_error("File size too large.");
    if (descriptor.getPosition() + size > descriptor.getSize()) {
        expandFile(descriptor, size);
    }
    auto writeSlices = getSlices(descriptor, size);
    unsigned written = 0;
    for (auto slice: writeSlices) {
        PersistentStorageController::write(slice.block, slice.start, src + written, slice.length);
        written += slice.length;
    }
}

void ScatteredFilesystem::printState() {
    Filesystem::printState();
    std::cout << "FREE BLOCKS:" << std::endl;
    occupationMap.print();
    std::cout << "ALL DESCRIPTORS:" << std::endl;
    descriptorManager.printAllDescriptors();
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

    openedDescriptor.indirectionBlock.save(descriptor.getBlock());
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
    auto block = occupationMap.occupy(1);
    auto rootDirectoryDescriptor = ScatteredFileDescriptor(true, block);
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

unsigned ScatteredFilesystem::maxFilesize() {
    return persistentStorage.blockSize() / sizeof(unsigned) * persistentStorage.blockSize();
}

void ScatteredFilesystem::expandFile(OpenedScatteredFileDescriptor &descriptor, unsigned int required) {
    auto size = descriptor.getSize();
    auto position = descriptor.getPosition();
    auto available = size - position;
    auto toExpand = required - available;
    auto blocksUsed = (size + NUMBER_OF_BYTES_PER_BLOCK - 1) / NUMBER_OF_BYTES_PER_BLOCK;
    auto blocksRequired = (toExpand + NUMBER_OF_BYTES_PER_BLOCK - 1) / NUMBER_OF_BYTES_PER_BLOCK;
    expandIndirectionBlock(descriptor.indirectionBlock, blocksUsed, blocksRequired);
    descriptor.setSize(size + toExpand);
}

void ScatteredFilesystem::expandIndirectionBlock(IndirectionBlock &indirectionBlock, unsigned int blocksUsed,
                                                 unsigned int blocksRequired) {
    for (int i = blocksUsed; i != blocksRequired; ++i) {
        auto newBlock = occupationMap.occupy(1);
        indirectionBlock.setBlock(i, newBlock);
    }
}






