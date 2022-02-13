//
// Created by paracelsus on 2/13/2022.
//

#include "OpenedFileDescriptor.h"

OpenedFileDescriptor::OpenedFileDescriptor(FileDescriptor fileDescriptor) : FileDescriptor(fileDescriptor) {
    position = 0;
}
