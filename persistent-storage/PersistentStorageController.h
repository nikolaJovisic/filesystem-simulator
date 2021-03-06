//
// Created by paracelsus on 2/14/2022.
//

#ifndef FILESYSTEM_SIMULATOR_PERSISTENTSTORAGECONTROLLER_H
#define FILESYSTEM_SIMULATOR_PERSISTENTSTORAGECONTROLLER_H

#include "PersistentStorage.h"

class PersistentStorageController {
private:
    static PersistentStorage *persistentStorage;
public:
    static void setDefaultPersistentStorage(PersistentStorage *persistentStorage);

    static void
    write(PersistentStorage &persistentStorage, unsigned startingBlock, unsigned position, char *src, unsigned length);

    static void
    read(PersistentStorage &persistentStorage, unsigned startingBlock, unsigned position, char *dst, unsigned length);

    static void
    write(unsigned startingBlock, unsigned position, char *src, unsigned length);

    static void
    read(unsigned startingBlock, unsigned position, char *dst, unsigned length);
};


#endif //FILESYSTEM_SIMULATOR_PERSISTENTSTORAGECONTROLLER_H
