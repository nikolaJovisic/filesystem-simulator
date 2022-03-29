//
// Created by paracelsus on 3/18/2022.
//

#include "ScatteredFilesystem.h"

int ScatteredFilesystem::open(std::string path) {
    return 0;
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
                                         ScatteredFilesystem::FormatType formatType) : Filesystem<OpenedScatteredFileDescriptor>(persistentStorage),
                                                                                       descriptorManager(persistentStorage) {

}

ScatteredFilesystem::ScatteredFilesystem(PersistentStorage &persistentStorage,
                                         ScatteredFilesystem::MountType mountType) : Filesystem<OpenedScatteredFileDescriptor>(persistentStorage),
                                                                                     descriptorManager(persistentStorage) {

}

void ScatteredFilesystem::readRaw(OpenedScatteredFileDescriptor &descriptor, char *dst, unsigned int size) {

}

void ScatteredFilesystem::writeRaw(OpenedScatteredFileDescriptor &descriptor, char *src, unsigned int size) {

}
