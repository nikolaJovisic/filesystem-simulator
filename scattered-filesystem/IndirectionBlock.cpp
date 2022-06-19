//
// Created by paracelsus on 6/12/2022.
//

#include "IndirectionBlock.h"

void IndirectionBlock::load(unsigned int number) {
    PersistentStorageController::read(number, 0, (char *) blocks, NUMBER_OF_BYTES_PER_BLOCK);
}

void IndirectionBlock::save(unsigned int number) {
    PersistentStorageController::write(number, 0, (char *) blocks, NUMBER_OF_BYTES_PER_BLOCK);
}


unsigned IndirectionBlock::getBlock(unsigned int index) {
    return blocks[index];
}

void IndirectionBlock::setBlock(unsigned int index, unsigned int value) {
    blocks[index] = value;
}
