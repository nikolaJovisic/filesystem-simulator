#include <iostream>
#include "PersistentStorage.h"
using namespace std;

int main() {
    const unsigned numberOfBlocks = 3;
    const unsigned bytesPerBlock = 16;
    PersistentStorage persistentStorage = PersistentStorage(numberOfBlocks, bytesPerBlock);

    char block[16];

    for (int i = 0; i < 16; ++i) {
        block[i] = '0';
    }

    block[7] = '1';

    for (int i = 0; i < 16; ++i) {
        std::cout << block[i];
    }
    std::cout<<std::endl;

    persistentStorage.write(0, block);

    block[8] = '1';

    for (int i = 0; i < 16; ++i) {
        std::cout << block[i];
    }
    std::cout<<std::endl;

    persistentStorage.write(1, block);

    persistentStorage.read(0, block);

    for (int i = 0; i < 16; ++i) {
        std::cout << block[i];
    }
    std::cout<<std::endl;

    persistentStorage.read(1, block);

    for (int i = 0; i < 16; ++i) {
        std::cout << block[i];
    }
    std::cout<<std::endl;
    
}
