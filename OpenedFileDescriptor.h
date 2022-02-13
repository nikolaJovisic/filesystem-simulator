//
// Created by paracelsus on 2/13/2022.
//

#ifndef FILESYSTEM_SIMULATOR_OPENEDFILEDESCRIPTOR_H
#define FILESYSTEM_SIMULATOR_OPENEDFILEDESCRIPTOR_H

#include "FileDescriptor.h"

class OpenedFileDescriptor : FileDescriptor {
private:
    unsigned position;
public:
    OpenedFileDescriptor(FileDescriptor); // NOLINT(google-explicit-constructor)
};


#endif //FILESYSTEM_SIMULATOR_OPENEDFILEDESCRIPTOR_H
