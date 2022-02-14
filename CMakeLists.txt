cmake_minimum_required(VERSION 3.21)
project(filesystem_simulator)

set(CMAKE_CXX_STANDARD 23)

add_executable(filesystem_simulator main.cpp PersistentStorage.cpp PersistentStorage.h FileDescriptor.cpp FileDescriptor.h FileManagementLayer.cpp FileManagementLayer.h OpenedFileDescriptor.cpp OpenedFileDescriptor.h ContinuousFileSystem.cpp ContinuousFileSystem.h Constants.h Directory.cpp Directory.h OccupationMap.cpp OccupationMap.h DescriptorManager.cpp DescriptorManager.h PathTransform.cpp PathTransform.h PersistentStorageController.cpp PersistentStorageController.h)
