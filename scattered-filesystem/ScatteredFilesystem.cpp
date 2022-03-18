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


void ScatteredFilesystem::listContentsAt(std::string path) {

}

void ScatteredFilesystem::printState() {
    Filesystem::printState();
}

void ScatteredFilesystem::persistMetadata() {

}
