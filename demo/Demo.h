//
// Created by paracelsus on 3/28/2022.
//

#ifndef FILESYSTEM_SIMULATOR_DEMO_H
#define FILESYSTEM_SIMULATOR_DEMO_H

#include "../continuous-filesystem/ContinuousFilesystem.h"
#include "../scattered-filesystem/ScatteredFilesystem.h"

class Demo {
public:
    static void cfsDemo(ContinuousFilesystem &fs);

    static void sfsDemo(ScatteredFilesystem &fs);

    static void sfsWriteReadDemo(ScatteredFilesystem &fs, int index, unsigned size);

    static void cfsWriteReadDemo(ContinuousFilesystem &fs, int index);
};


#endif //FILESYSTEM_SIMULATOR_DEMO_H
