//
// Created by paracelsus on 2/13/2022.
//

#ifndef FILESYSTEM_SIMULATOR_CONSTANTS_H
#define FILESYSTEM_SIMULATOR_CONSTANTS_H

#include <string>

//FS
const char DELIMITER = '/';
const std::string DELIMITER_STRING = std::string(1, DELIMITER);
const float DESCRIPTORS_MEMORY_SHARE = 0.1;
const unsigned MIN_NUMBER_OF_BLOCKS = 100;
const unsigned MIN_NUMBER_OF_BYTES_PER_BLOCK = 32;
const unsigned NUMBER_OF_BLOCKS_RESERVED_FOR_FILESYSTEM_METADATA = 10;

//CFS
const unsigned NUMBER_OF_BLOCKS_RESERVED_FOR_ROOT_DIRECTORY = 1;

//SFS (constants not flexible for changing)
const unsigned DIRECT_SLICES = 2;
const unsigned FIRST_INDIRECTION = 1;
const unsigned SECOND_INDIRECTION = 1;
const unsigned TABLE_SLICES_SIZE = DIRECT_SLICES + FIRST_INDIRECTION + SECOND_INDIRECTION;

#endif //FILESYSTEM_SIMULATOR_CONSTANTS_H
