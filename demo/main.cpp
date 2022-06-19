#include "../persistent-storage/PersistentStorage.h"
#include "../metadata-handlers/OccupationMap.h"
#include "../metadata-handlers/DescriptorManager.h"
#include "../filesystem/Directory.h"
#include "../continuous-filesystem/ContinuousFilesystem.h"
#include "Demo.h"
#include "../scattered-filesystem/ScatteredFilesystem.h"

using namespace std;

void cfs() {
    PersistentStorage psFormat(10000, 128);
    PersistentStorageController::setDefaultPersistentStorage(&psFormat);

    ContinuousFilesystem cfsFormat(psFormat, ContinuousFilesystem::STANDARD_FORMAT);
    cfsFormat.serializeInFile("cfsDemo.cfs");

    PersistentStorage psMount("cfsDemo.cfs");
    ContinuousFilesystem cfsMount(psMount, ContinuousFilesystem::STANDARD_MOUNT);

    Demo::cfsDemo(cfsMount);
}

void sfs() {
    PersistentStorage psFormat(10000, 128);
    PersistentStorageController::setDefaultPersistentStorage(&psFormat);

    ScatteredFilesystem sfsFormat(psFormat, ScatteredFilesystem::STANDARD_FORMAT);
    sfsFormat.serializeInFile("sfsDemo.cfs");

    PersistentStorage psMount("sfsDemo.cfs");
    ScatteredFilesystem sfsMount(psMount, ScatteredFilesystem::STANDARD_MOUNT);

    Demo::sfsDemo(sfsMount);
}

int main() {
    sfs();
}




