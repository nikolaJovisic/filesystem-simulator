//
// Created by paracelsus on 6/12/2022.
//

#ifndef FILESYSTEM_SIMULATOR_INDIRECTIONBLOCK_H
#define FILESYSTEM_SIMULATOR_INDIRECTIONBLOCK_H

#include "../filesystem/Constants.h"
#include "../persistent-storage/PersistentStorageController.h"

class IndirectionBlock {
public:
    unsigned blocks[UNSIGNEDS_PER_BLOCK];

    void setBlock(unsigned index, unsigned value);

    unsigned getBlock(unsigned index);

    void load(unsigned number);

    void save(unsigned number);
};


#endif //FILESYSTEM_SIMULATOR_INDIRECTIONBLOCK_H
