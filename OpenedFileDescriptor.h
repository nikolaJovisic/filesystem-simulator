//
// Created by paracelsus on 2/13/2022.
//

#ifndef FILESYSTEM_SIMULATOR_OPENEDFILEDESCRIPTOR_H
#define FILESYSTEM_SIMULATOR_OPENEDFILEDESCRIPTOR_H

#include <ostream>
#include "ContinuousFileDescriptor.h"

class OpenedFileDescriptor : public ContinuousFileDescriptor {
private:
    unsigned position;
public:
    explicit OpenedFileDescriptor(ContinuousFileDescriptor);

    [[nodiscard]] unsigned int getPosition() const;

    void setPosition(unsigned int position);

    friend std::ostream &operator<<(std::ostream &os, const OpenedFileDescriptor &descriptor);
};


#endif //FILESYSTEM_SIMULATOR_OPENEDFILEDESCRIPTOR_H
