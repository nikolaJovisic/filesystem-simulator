//
// Created by paracelsus on 2/13/2022.
//

#ifndef FILESYSTEM_SIMULATOR_CONTINUOUSFILEDESCRIPTOR_H
#define FILESYSTEM_SIMULATOR_CONTINUOUSFILEDESCRIPTOR_H


#include <ostream>
#include "../filesystem/FileDescriptor.h"

class ContinuousFileDescriptor : public FileDescriptor {
protected:
    unsigned startingBlock;
    unsigned blocksReserved;

public:
    ContinuousFileDescriptor(bool directory, unsigned startingBlock, unsigned blocksReserved, unsigned usedSpace);

    [[nodiscard]] unsigned int getStartingBlock() const;

    [[nodiscard]] unsigned int getBlocksReserved() const;

    void setBlocksReserved(unsigned int blocksReserved);

    friend std::ostream &operator<<(std::ostream &os, const ContinuousFileDescriptor &descriptor);

};


#endif //FILESYSTEM_SIMULATOR_CONTINUOUSFILEDESCRIPTOR_H
