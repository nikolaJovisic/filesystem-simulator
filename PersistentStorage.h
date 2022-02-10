//
// Created by paracelsus on 2/10/2022.
//

#ifndef FILESYSTEM_SIMULATOR_PERSISTENTSTORAGE_H
#define FILESYSTEM_SIMULATOR_PERSISTENTSTORAGE_H


class PersistentStorage {
private:
    char *storage;
    unsigned numberOfBlocks;
    unsigned bytesPerBlock;
public:
    PersistentStorage(unsigned numberOfBlocks, unsigned bytesPerBlock);
    void write(unsigned blockNumber, char *src);
    void read(unsigned blockNumber, char *dst);
    ~PersistentStorage();
};


#endif //FILESYSTEM_SIMULATOR_PERSISTENTSTORAGE_H
