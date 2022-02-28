#include <iostream>
#include "PersistentStorage.h"
#include "OccupationMap.h"
#include "DescriptorManager.h"
#include "Directory.h"
#include "PathTransform.h"
#include "ContinuousFilesystem.h"
using namespace std;

typedef void (*TestFunction) (Filesystem& fs);

void test1(Filesystem& fs) {
    fs.printState();
    fs.create("/f0.txt", false, 10);
    fs.create("/dir", true, 10);
    fs.create("/dir/f1.txt", false, 10);

    fs.printState();
    fs.listContentsAt("/");
    fs.listContentsAt("/dir");

    fs.remove("/dir");
    fs.listContentsAt("/");
    fs.printState();
}


int main() {
    PersistentStorage persistentStorage(1000, 512);
    ContinuousFilesystem cfs(persistentStorage);
    test1(cfs);
}




