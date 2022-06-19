//
// Created by paracelsus on 3/28/2022.
//

#ifndef FILESYSTEM_SIMULATOR_FILEDESCRIPTOR_H
#define FILESYSTEM_SIMULATOR_FILEDESCRIPTOR_H


#include <ostream>

class FileDescriptor {
protected:
    bool deleted;
    bool directory;
    unsigned usedSpace;

    explicit FileDescriptor(bool directory, unsigned usedSpace);

public:
    unsigned int getUsedSpace() const;

    void setUsedSpace(unsigned int usedSpace);

    void markDeleted();

    [[nodiscard]] bool isDeleted() const;

    friend std::ostream &operator<<(std::ostream &os, const FileDescriptor &descriptor);

    [[nodiscard]] bool isDirectory() const;
};


#endif //FILESYSTEM_SIMULATOR_FILEDESCRIPTOR_H
