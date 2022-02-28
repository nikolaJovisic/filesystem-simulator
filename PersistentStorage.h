//
// Created by paracelsus on 2/10/2022.
//

#ifndef FILESYSTEM_SIMULATOR_PERSISTENTSTORAGE_H
#define FILESYSTEM_SIMULATOR_PERSISTENTSTORAGE_H


#include <ostream>

class PersistentStorage {
private:
    char *storage;
    unsigned numberOfBlocks;
    unsigned bytesPerBlock;
public:
    PersistentStorage();
    PersistentStorage(unsigned numberOfBlocks, unsigned bytesPerBlock);
    void write(unsigned block, char *src);
    void read(unsigned block, char *dst);
    ~PersistentStorage();

    [[nodiscard]] unsigned int getNumberOfBlocks() const;

    [[nodiscard]] unsigned int blockSize() const;

    friend std::ostream &operator<<(std::ostream &os, const PersistentStorage &storage);
};


#endif //FILESYSTEM_SIMULATOR_PERSISTENTSTORAGE_H
