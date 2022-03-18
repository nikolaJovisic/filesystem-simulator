//
// Created by paracelsus on 2/13/2022.
//

#include <iostream>
#include "Filesystem.h"

Filesystem::Filesystem(PersistentStorage &persistentStorage) : persistentStorage(
        persistentStorage) {}

void Filesystem::printState() {
    std::cout<<"-------------------"<<std::endl;
    std::cout<<"PERSISTENT STORAGE:"<<std::endl;
    std::cout<<persistentStorage<<std::endl;
    std::cout<<"OPENED FILES:"<<std::endl;
    for(auto& descriptor: openedFiles) {
        std::cout<<descriptor.first<<": ";
        std::cout<<descriptor.second<<std::endl;
    }
}

void Filesystem::serializeInFile(std::string filename) {
    persistMetadata();
    persistentStorage.serializeInFile(filename);
}

void Filesystem::listContentsAt(std::string path) {
    std::cout<<"-------------------"<<std::endl;
    auto index = open(path);
    auto filenames = getDirectory(index).getAllFilenames();
    close(index);
    for(auto i: filenames) {
        std::cout<<i<<std::endl;
    }
}

