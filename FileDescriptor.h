//
// Created by paracelsus on 2/13/2022.
//

#ifndef FILESYSTEM_SIMULATOR_FILEDESCRIPTOR_H
#define FILESYSTEM_SIMULATOR_FILEDESCRIPTOR_H


class FileDescriptor {
private:
    bool directory;
    unsigned startingBlock;
    unsigned blocksReserved;
    unsigned usedSpace;
public:
    FileDescriptor(bool directory, unsigned startingBlock, unsigned blocksReserved, unsigned usedSpace);

    [[nodiscard]] bool isDirectory() const;

    [[nodiscard]] unsigned int getStartingBlock() const;

    [[nodiscard]] unsigned int getBlocksReserved() const;

    [[nodiscard]] unsigned int getUsedSpace() const;

    void setUsedSpace(unsigned int usedSpace);
};


#endif //FILESYSTEM_SIMULATOR_FILEDESCRIPTOR_H
