cmake_minimum_required(VERSION 3.21)
project(filesystem_simulator)

set(CMAKE_CXX_STANDARD 20)

add_executable(filesystem_simulator main.cpp PersistentStorage.cpp PersistentStorage.h FileDescriptor.cpp FileDescriptor.h Filesystem.cpp Filesystem.h OpenedFileDescriptor.cpp OpenedFileDescriptor.h ContinuousFilesystem.cpp ContinuousFilesystem.h Constants.h Directory.cpp Directory.h OccupationMap.cpp OccupationMap.h DescriptorManager.cpp DescriptorManager.h PathTransform.cpp PathTransform.h PersistentStorageController.cpp PersistentStorageController.h)
