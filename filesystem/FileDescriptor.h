//
// Created by paracelsus on 3/28/2022.
//

#ifndef FILESYSTEM_SIMULATOR_FILEDESCRIPTOR_H
#define FILESYSTEM_SIMULATOR_FILEDESCRIPTOR_H


class FileDescriptor {
protected:
    bool deleted;
    bool directory;
    FileDescriptor(bool directory);
public:

    void markDeleted();

    bool isDeleted() const;

    bool isDirectory() const;
};


#endif //FILESYSTEM_SIMULATOR_FILEDESCRIPTOR_H
