//
// Created by paracelsus on 2/14/2022.
//

#include "PersistentStorageController.h"

void PersistentStorageController::write(PersistentStorage &persistentStorage, unsigned int startingBlock, unsigned position, char *src,
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
        memcpy(tmp + skipInFirst, src, length);
        persistentStorage.write(firstBlock, tmp);
        return;
    }

    //first block
    persistentStorage.read(firstBlock, tmp);
    memcpy(tmp + skipInFirst, src, blockSize - skipInFirst);
    persistentStorage.write(firstBlock, tmp);
    src += blockSize - skipInFirst;

    //middle blocks
    for (unsigned i = firstBlock + 1; i != lastBlock; ++i) {
        persistentStorage.write(i, src);
        src += blockSize;
    }

    //last block
    persistentStorage.read(lastBlock, tmp);
    memcpy(tmp, src, processInLast);
    persistentStorage.write(lastBlock, tmp);
}

void PersistentStorageController::read(PersistentStorage &persistentStorage, unsigned int startingBlock, unsigned position, char *dst,
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
        memcpy(dst, tmp + skipInFirst, length);
        return;
    }

    //first block
    persistentStorage.read(firstBlock, tmp);
    memcpy(dst, tmp + skipInFirst, blockSize - skipInFirst);
    dst += blockSize - skipInFirst;

    //middle blocks
    for (unsigned i = firstBlock + 1; i != lastBlock; ++i) {
        persistentStorage.read(i, dst);
        dst += blockSize;
    }

    //last block
    persistentStorage.read(lastBlock, tmp);
    memcpy(dst, tmp, processInLast);
}
