//
// Created by paracelsus on 3/28/2022.
//

#ifndef FILESYSTEM_SIMULATOR_FILEDESCRIPTOR_H
#define FILESYSTEM_SIMULATOR_FILEDESCRIPTOR_H


class FileDescriptor {
protected:
    bool deleted;
public:
    FileDescriptor() {
        deleted = false;
    }
    virtual void markDeleted() {
        deleted = true;
    }
};


#endif //FILESYSTEM_SIMULATOR_FILEDESCRIPTOR_H
