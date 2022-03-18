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

