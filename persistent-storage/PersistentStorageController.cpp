//
// Created by paracelsus on 2/14/2022.
//

#include "PersistentStorageController.h"
#include <cstring>

PersistentStorage *PersistentStorageController::persistentStorage = nullptr;

void
PersistentStorageController::write(PersistentStorage &persistentStorage, unsigned int startingBlock, unsigned position,
                                   char *src,
                                   unsigned int length) {
    if (length == 0) return;

    unsigned blockSize = persistentStorage.blockSize();
    unsigned firstBlock = startingBlock + position / blockSize;
    unsigned lastBlock = firstBlock + (position % blockSize + length) / blockSize;
    unsigned skipInFirst = position % blockSize;
    unsigned processInLast = (position % blockSize + length) % blockSize;

    char tmp[blockSize];

    if (firstBlock == lastBlock) {
        persistentStorage.read(firstBlock, tmp);
        std::memcpy(tmp + skipInFirst, src, length);
        persistentStorage.write(firstBlock, tmp);
        return;
    }

    //first block
    persistentStorage.read(firstBlock, tmp);
    std::memcpy(tmp + skipInFirst, src, blockSize - skipInFirst);
    persistentStorage.write(firstBlock, tmp);
    src += blockSize - skipInFirst;

    //middle blocks
    for (unsigned i = firstBlock + 1; i != lastBlock; ++i) {
        persistentStorage.write(i, src);
        src += blockSize;
    }

    //last block
    persistentStorage.read(lastBlock, tmp);
    std::memcpy(tmp, src, processInLast);
    persistentStorage.write(lastBlock, tmp);
}

void
PersistentStorageController::read(PersistentStorage &persistentStorage, unsigned int startingBlock, unsigned position,
                                  char *dst,
                                  unsigned int length) {
    if (length == 0) return;

    unsigned blockSize = persistentStorage.blockSize();
    unsigned firstBlock = startingBlock + position / blockSize;
    unsigned lastBlock = firstBlock + (position % blockSize + length) / blockSize;
    unsigned skipInFirst = position % blockSize;
    unsigned processInLast = (position % blockSize + length) % blockSize;

    char tmp[blockSize];

    if (firstBlock == lastBlock) {
        persistentStorage.read(firstBlock, tmp);
        std::memcpy(dst, tmp + skipInFirst, length);
        return;
    }

    //first block
    persistentStorage.read(firstBlock, tmp);
    std::memcpy(dst, tmp + skipInFirst, blockSize - skipInFirst);
    dst += blockSize - skipInFirst;

    //middle blocks
    for (unsigned i = firstBlock + 1; i != lastBlock; ++i) {
        persistentStorage.read(i, dst);
        dst += blockSize;
    }

    //last block
    persistentStorage.read(lastBlock, tmp);
    std::memcpy(dst, tmp, processInLast);
}


void PersistentStorageController::setDefaultPersistentStorage(PersistentStorage *persistentStorage) {
    PersistentStorageController::persistentStorage = persistentStorage;
}

void
PersistentStorageController::write(unsigned int startingBlock, unsigned int position, char *src, unsigned int length) {
    if (persistentStorage == nullptr) throw std::runtime_error("Default persistent storage not set!");
    write(*persistentStorage, startingBlock, position, src, length);
}

void
PersistentStorageController::read(unsigned int startingBlock, unsigned int position, char *dst, unsigned int length) {
    if (persistentStorage == nullptr) throw std::runtime_error("Default persistent storage not set!");
    read(*persistentStorage, startingBlock, position, dst, length);
}
