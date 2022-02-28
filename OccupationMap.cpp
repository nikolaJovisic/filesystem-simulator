//
// Created by paracelsus on 2/13/2022.
//

#include "OccupationMap.h"
#include "Constants.h"
#include <iostream>

OccupationMap::OccupationMap(unsigned startBlock, unsigned endBlock) {
    map = std::set<std::pair<unsigned, unsigned>>();
    map.emplace(startBlock, endBlock);
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

OccupationMap::OccupationMap() {
}


