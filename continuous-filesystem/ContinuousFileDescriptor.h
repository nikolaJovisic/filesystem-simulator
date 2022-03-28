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
    unsigned usedSpace;
    bool directory;

public:
    ContinuousFileDescriptor(bool directory, unsigned startingBlock, unsigned blocksReserved, unsigned usedSpace);

    [[nodiscard]] bool isDirectory() const;

    [[nodiscard]] unsigned int getStartingBlock() const;

    [[nodiscard]] unsigned int getBlocksReserved() const;

    [[nodiscard]] unsigned int getUsedSpace() const;

    void setUsedSpace(unsigned int usedSpace);

    void setBlocksReserved(unsigned int blocksReserved);

    friend std::ostream &operator<<(std::ostream &os, const ContinuousFileDescriptor &descriptor);

};




#endif //FILESYSTEM_SIMULATOR_CONTINUOUSFILEDESCRIPTOR_H
