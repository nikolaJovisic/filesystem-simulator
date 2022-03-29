//
// Created by paracelsus on 3/28/2022.
//

#ifndef FILESYSTEM_SIMULATOR_SCATTEREDFILEDESCRIPTOR_H
#define FILESYSTEM_SIMULATOR_SCATTEREDFILEDESCRIPTOR_H


#include <ostream>
#include "../filesystem/FileDescriptor.h"
#include "../filesystem/Constants.h"

class ScatteredFileDescriptor : public FileDescriptor {
private:
    int tableSlices[TABLE_SLICES_SIZE];
    unsigned size;
public:
    explicit ScatteredFileDescriptor(bool directory);

    friend std::ostream &operator<<(std::ostream &os, const ScatteredFileDescriptor &descriptor);

    unsigned int getSize() const;

    void setSize(unsigned int size);
};


#endif //FILESYSTEM_SIMULATOR_SCATTEREDFILEDESCRIPTOR_H
