//
// Created by paracelsus on 2/13/2022.
//

#include "OccupationMap.h"
#include "Constants.h"
#include <iostream>
#include <cstring>

OccupationMap::OccupationMap() {
}

OccupationMap::OccupationMap(unsigned startBlock, unsigned endBlock) {
    map = std::set<std::pair<unsigned, unsigned>>();
    map.emplace(startBlock, endBlock);
}

void OccupationMap::loadFrom(char *src) {
    map = std::set<std::pair<unsigned, unsigned>>();
    unsigned mapSize;
    unsigned first;
    unsigned second;
    std::memcpy((char *)&mapSize, src, sizeof(unsigned));
    src += sizeof(unsigned);
    for (int i = 0; i < mapSize; ++i) {
        std::memcpy((char *)&(first), src, sizeof(unsigned));
        src += sizeof(unsigned);
        std::memcpy((char *)&(second), src, sizeof(unsigned));
        src += sizeof(unsigned);
        map.emplace(first, second);
    }
}

unsigned OccupationMap::occupy(unsigned int numberOfBlocks) {
    auto bestCandidate = map.begin();
    auto bestSpan = -1;
    for(auto it = map.begin(); it != map.end() ; ++it) {
        auto& segment = *it;
        auto span = segment.second - segment.first;
        if(span < numberOfBlocks) {
            continue;
        }
        if(bestSpan == -1 || span < bestSpan) {
            bestCandidate = it;
            bestSpan = span;
        }
    }
    if (bestSpan == -1) throw std::runtime_error("Out of memory.");
    auto retVal = bestCandidate->first;
    auto newFirst = (bestCandidate->first) + numberOfBlocks;
    auto newSecond = bestCandidate->second;
    map.erase(bestCandidate);
    if (newFirst != newSecond) {
        map.emplace(newFirst, newSecond);
    }
    return retVal;
}

void OccupationMap::free(unsigned int startBlock, unsigned int numberOfBlocks) {
    unsigned first = startBlock;
    unsigned second = startBlock + numberOfBlocks;
    std::_Rb_tree_const_iterator<std::pair<unsigned int, unsigned int>> it;

    for(it = map.begin(); it != map.end() ;) {
        if (second > it->first && startBlock < it->second) throw std::invalid_argument("Segment overlap.");
        if (first != it->second && second != it->first) {
            ++it;
        }
        if (first == it->second) {
            first = it->first;
            auto toRemove = it;
            ++it;
            map.erase(toRemove);
        }
        if (second == it->first) {
            second = it->second;
            auto toRemove = it;
            ++it;
            map.erase(toRemove);
        }
    }
    map.emplace(first, second);
}

void OccupationMap::print() {
    for(auto segment: map) {
        std::cout<<segment.first<<" - "<<segment.second<<std::endl;
    }
}

void OccupationMap::serialize(char *dst) {
    unsigned mapSize = map.size();
    std::memcpy(dst, (char *)&mapSize, sizeof(unsigned));
    dst += sizeof(unsigned);
    for (auto i = map.begin(); i != map.end(); ++i) {
        std::memcpy(dst, (char *)&(i->first), sizeof(unsigned));
        dst += sizeof(unsigned);
        std::memcpy(dst, (char *)&(i->second), sizeof(unsigned));
        dst += sizeof(unsigned);
    }
}

unsigned OccupationMap::serializationSize() {
    return sizeof(unsigned) + map.size() * (2 * sizeof(unsigned));
}






