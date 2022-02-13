//
// Created by paracelsus on 2/13/2022.
//

#ifndef FILESYSTEM_SIMULATOR_FILEDESCRIPTOR_H
#define FILESYSTEM_SIMULATOR_FILEDESCRIPTOR_H


class FileDescriptor {
private:
    bool directory;
    unsigned startingBlock;
    unsigned length;
public:
    FileDescriptor(bool directory, unsigned startingBlock, unsigned length);

    [[nodiscard]] bool isDirectory() const;

    [[nodiscard]] unsigned int getStartingBlock() const;

    [[nodiscard]] unsigned int getLength() const;
};


#endif //FILESYSTEM_SIMULATOR_FILEDESCRIPTOR_H
