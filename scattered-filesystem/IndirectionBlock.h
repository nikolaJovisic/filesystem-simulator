//
// Created by paracelsus on 6/12/2022.
//

#ifndef FILESYSTEM_SIMULATOR_INDIRECTIONBLOCK_H
#define FILESYSTEM_SIMULATOR_INDIRECTIONBLOCK_H


class IndirectionBlock {
    unsigned number;
    unsigned *blocks;
    unsigned numOfBlocks;

    void load();

    void save();


};


#endif //FILESYSTEM_SIMULATOR_INDIRECTIONBLOCK_H
