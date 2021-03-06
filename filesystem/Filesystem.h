//
// Created by paracelsus on 2/13/2022.
//

#ifndef FILESYSTEM_SIMULATOR_FILESYSTEM_H
#define FILESYSTEM_SIMULATOR_FILESYSTEM_H

#include "../persistent-storage/PersistentStorage.h"
#include "../continuous-filesystem/OpenedContinuousFileDescriptor.h"
#include "Directory.h"
#include <string>
#include <map>
#include <iostream>
#include <deque>

template<class OpenedFileDescriptorType>
class Filesystem {
protected:
    PersistentStorage &persistentStorage;
    std::map<unsigned, OpenedFileDescriptorType> openedFiles;

    virtual void persistMetadata() = 0;

    virtual Directory getDirectory(unsigned directoryIndex) = 0;

    virtual void saveDirectory(Directory directory, unsigned directoryIndex) = 0;

    virtual void readRaw(OpenedFileDescriptorType &descriptor, char *dst, unsigned int size) = 0;

    virtual void writeRaw(OpenedFileDescriptorType &descriptor, char *src, unsigned int size) = 0;

public:
    virtual int open(std::string path) = 0;

    virtual void close(unsigned index) = 0;

    virtual void create(std::string path, bool isDirectory, unsigned blocksReserved = 0) = 0;

    virtual void read(unsigned index, char *dst, unsigned size) = 0;

    virtual void write(unsigned index, char *src, unsigned size) = 0;

    virtual void seek(unsigned index, unsigned position) = 0;

    virtual void remove(std::string path) = 0;

    explicit Filesystem(PersistentStorage &persistentStorage) : persistentStorage(
            persistentStorage) {}

    unsigned getLastDirectoryIndex(unsigned startingDirectoryIndex, std::deque<std::string> &directories) {
        if (directories.empty()) return startingDirectoryIndex;

        Directory directory = getDirectory(startingDirectoryIndex);

        auto index = directory.getIndex(directories.front());
        directories.pop_front();
        getLastDirectoryIndex(index, directories);

        return getLastDirectoryIndex(index, directories);
    }

    void addToDirectory(std::deque<std::string> &directories, std::string fileName,
                        unsigned int fileIndex) {
        auto directoryIndex = getLastDirectoryIndex(0, directories);
        auto directory = getDirectory(directoryIndex);
        directory.addFile(fileName, fileIndex);
        saveDirectory(directory, directoryIndex);
    }

    void removeFromDirectory(std::deque<std::string> &directories, std::string fileName) {
        auto directoryIndex = getLastDirectoryIndex(0, directories);
        auto directory = getDirectory(directoryIndex);
        directory.removeFile(fileName);
        saveDirectory(directory, directoryIndex);
    }

    virtual void printState() {
        std::cout << "-------------------" << std::endl;
        std::cout << "PERSISTENT STORAGE:" << std::endl;
        std::cout << persistentStorage << std::endl;
        std::cout << "OPENED FILES:" << std::endl;
        for (auto &descriptor: openedFiles) {
            std::cout << descriptor.first << ": ";
            std::cout << descriptor.second << std::endl;
        }
    }

    void serializeInFile(std::string filename) {
        persistMetadata();
        persistentStorage.serializeInFile(filename);
    }

    void listContentsAt(std::string path) {
        std::cout << "-------------------" << std::endl;
        auto index = open(path);
        auto filenames = getDirectory(index).getAllFilenames();
        close(index);
        for (auto i: filenames) {
            std::cout << i << std::endl;
        }
    }
};


#endif //FILESYSTEM_SIMULATOR_FILESYSTEM_H
