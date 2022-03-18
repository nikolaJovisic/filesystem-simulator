//
// Created by paracelsus on 2/13/2022.
//

#ifndef FILESYSTEM_SIMULATOR_OPENEDCONTINUOUSFILEDESCRIPTOR_H
#define FILESYSTEM_SIMULATOR_OPENEDCONTINUOUSFILEDESCRIPTOR_H

#include <ostream>
#include "ContinuousFileDescriptor.h"

class OpenedContinuousFileDescriptor : public ContinuousFileDescriptor {
private:
    unsigned position;
public:
    explicit OpenedContinuousFileDescriptor(ContinuousFileDescriptor);

    [[nodiscard]] unsigned int getPosition() const;

    void setPosition(unsigned int position);

    friend std::ostream &operator<<(std::ostream &os, const OpenedContinuousFileDescriptor &descriptor);
};


#endif //FILESYSTEM_SIMULATOR_OPENEDCONTINUOUSFILEDESCRIPTOR_H
