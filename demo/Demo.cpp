//
// Created by paracelsus on 3/28/2022.
//

#include "Demo.h"
#include "../persistent-storage/PersistentStorage.h"
#include "../metadata-handlers/OccupationMap.h"
#include "../metadata-handlers/DescriptorManager.h"
#include "../filesystem/Directory.h"
#include "../continuous-filesystem/ContinuousFilesystem.h"

void Demo::cfsDemo(ContinuousFilesystem& fs) {
    std::cout<<"**********************************"<<std::endl;
    std::cout<<"Descriptor memory reserved, root directory and descriptor for it created."<<std::endl;

    fs.printState();
    fs.create("/f0.txt", false, 20);
    fs.create("/dir", true, 10);
    fs.create("/dir/f1.txt", false, 10);

    std::cout<<"**********************************"<<std::endl;
    std::cout<<"Files and directories created, memory allocated for directories with maps of access to their contents, and files themselves in both descriptors and occupation map, descriptors created."<<std::endl;

    fs.printState();

    std::cout<<"**********************************"<<std::endl;
    std::cout<<"Directories contents displayed."<<std::endl;

    std::cout<<"-------------------"<<std::endl;
    std::cout<<"Root directory contents:"<<std::endl;
    fs.listContentsAt("/");

    std::cout<<"-------------------"<<std::endl;
    std::cout<<"dir directory contents:"<<std::endl;
    fs.listContentsAt("/dir");

    auto index = fs.open("/f0.txt");

    std::cout<<"**********************************"<<std::endl;
    std::cout<<"File opened, its descriptor added to opened files collection."<<std::endl;

    fs.printState();

    int src[100];
    int dst[100];

    for (int i = 0; i < 100; ++i) {
        src[i] = 2*i;
    }

    fs.write(index, (char*)src, 100*sizeof(int));
    fs.close(index);

    std::cout<<"**********************************"<<std::endl;
    std::cout<<"Wrote in file, then closed the file, descriptor saved and removed from opened files collection."<<std::endl;

    fs.printState();
    index = fs.open("/f0.txt");
    fs.read(index, (char*)dst, 100*sizeof(int));

    std::cout<<"**********************************"<<std::endl;
    std::cout<<"Contents previously written in file now successfully loaded into memory."<<std::endl;


    std::cout<<"-------------------"<<std::endl;
    for (int i = 0; i < 100; ++i) {
        std::cout<<dst[i]<<".";
    }
    std::cout<<std::endl;

    for (int i = 0; i < 100; ++i) {
        src[i] = 3*i;
    }

    fs.seek(index, 15*sizeof(int));
    fs.write(index, (char*)src, 10*sizeof(int));
    fs.seek(index, 0);
    fs.read(index, (char*)dst, 100*sizeof(int));

    std::cout<<"**********************************"<<std::endl;
    std::cout<<"File pointer moved and new data written over part of file, then loaded into memory and displayed."<<std::endl;

    std::cout<<"-------------------"<<std::endl;
    for (int i : dst) {
        std::cout<<i<<".";
    }
    std::cout<<std::endl;


    fs.create("/f2.txt", false, 20);
    fs.remove("/dir");

    std::cout<<"**********************************"<<std::endl;
    std::cout<<"Directory removed, its contents recursevly removed, occupation map space freed (now there are multiple fragments of free space), root directory contents listed."<<std::endl;

    std::cout<<"-------------------"<<std::endl;
    std::cout<<"Root directory contents:"<<std::endl;
    fs.listContentsAt("/");
    fs.printState();

    fs.remove("/f2.txt");
    std::cout<<"**********************************"<<std::endl;
    std::cout<<"File removed, occupation map space freed and merged with previous free space."<<std::endl;
    fs.printState();
}