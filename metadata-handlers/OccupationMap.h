//
// Created by paracelsus on 2/13/2022.
//

#ifndef FILESYSTEM_SIMULATOR_OCCUPATIONMAP_H
#define FILESYSTEM_SIMULATOR_OCCUPATIONMAP_H

#include <set>
#include <fstream>

class OccupationMap {
private:
    std::set<std::pair<unsigned, unsigned>> map;
public:
    OccupationMap();
    explicit OccupationMap(unsigned startBlock, unsigned endBlock);
    unsigned occupy(unsigned numberOfBlocks);
    void free(unsigned startBlock, unsigned numberOfBlocks);
    void print();
    void serialize(char* dst);
    unsigned serializationSize();
    void loadFrom(char *src);
};


#endif //FILESYSTEM_SIMULATOR_OCCUPATIONMAP_H
