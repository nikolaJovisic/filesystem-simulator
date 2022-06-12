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
    int block;
    unsigned size;
public:
    explicit ScatteredFileDescriptor(bool directory, unsigned block);

    friend std::ostream &operator<<(std::ostream &os, const ScatteredFileDescriptor &descriptor);

    unsigned int getSize() const;

    void setSize(unsigned int size);

    unsigned getBlock();

};


#endif //FILESYSTEM_SIMULATOR_SCATTEREDFILEDESCRIPTOR_H
