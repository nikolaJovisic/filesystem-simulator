//
// Created by paracelsus on 3/28/2022.
//

#ifndef FILESYSTEM_SIMULATOR_OPENEDSCATTEREDFILEDESCRIPTOR_H
#define FILESYSTEM_SIMULATOR_OPENEDSCATTEREDFILEDESCRIPTOR_H


#include "ScatteredFileDescriptor.h"
#include <ostream>

class OpenedScatteredFileDescriptor : public ScatteredFileDescriptor {
private:
    unsigned position;
public:
    explicit OpenedScatteredFileDescriptor(ScatteredFileDescriptor);

    [[nodiscard]] unsigned int getPosition() const;

    void setPosition(unsigned int position);

    friend std::ostream &operator<<(std::ostream &os, const OpenedScatteredFileDescriptor &descriptor);

};


#endif //FILESYSTEM_SIMULATOR_OPENEDSCATTEREDFILEDESCRIPTOR_H
