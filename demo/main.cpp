#include "../persistent-storage/PersistentStorage.h"
#include "../metadata-handlers/OccupationMap.h"
#include "../metadata-handlers/DescriptorManager.h"
#include "../filesystem/Directory.h"
#include "../continuous-filesystem/ContinuousFilesystem.h"
#include "Demo.h"
#include "../scattered-filesystem/ScatteredFilesystem.h"

using namespace std;

int main() {
    PersistentStorage psFormat(10000, 128);

    ContinuousFilesystem cfsFormat(psFormat, ContinuousFilesystem::STANDARD_FORMAT);
    cfsFormat.serializeInFile("cfsDemo.cfs");

    PersistentStorage psMount("cfsDemo.cfs");
    ContinuousFilesystem cfsMount(psMount, ContinuousFilesystem::STANDARD_MOUNT);

    Demo::cfsDemo(cfsMount);
    /*
    ScatteredFilesystem sfs(psFormat, ScatteredFilesystem::STANDARD_FORMAT);
    sfs.create("/file.bin", false, 0);
    sfs.printState();
     */
}




