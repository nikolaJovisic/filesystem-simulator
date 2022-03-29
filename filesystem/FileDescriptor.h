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
    explicit FileDescriptor(bool directory);
public:

    void markDeleted();

    [[nodiscard]] bool isDeleted() const;

    [[nodiscard]] bool isDirectory() const;

    friend std::ostream &operator<<(std::ostream &os, const FileDescriptor &descriptor);
};


#endif //FILESYSTEM_SIMULATOR_FILEDESCRIPTOR_H
