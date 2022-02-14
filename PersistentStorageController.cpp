//
// Created by paracelsus on 2/14/2022.
//

#include "PersistentStorageController.h"

void PersistentStorageController::write(PersistentStorage &persistentStorage, unsigned int startingBlock, char *src,
                                        unsigned int length) {
    unsigned blockSize = persistentStorage.blockSize();
    unsigned numberOfBlocks = length / blockSize + (int)(length % blockSize != 0);

    for (unsigned i = startingBlock; i != startingBlock + numberOfBlocks; ++i) {
        persistentStorage.write(i, src);
        src += persistentStorage.blockSize();
    }
}

void PersistentStorageController::read(PersistentStorage &persistentStorage, unsigned int startingBlock, char *dst,
                                       unsigned int length) {
    unsigned blockSize = persistentStorage.blockSize();
    unsigned numberOfBlocks = length / blockSize + (int)(length % blockSize != 0);

    for (unsigned i = startingBlock; i != startingBlock + numberOfBlocks; ++i) {
        persistentStorage.read(i, dst);
        dst += persistentStorage.blockSize();
    }
}
