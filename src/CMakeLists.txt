cmake_minimum_required (VERSION 2.8.1)
#Dense kitchen project
project(dense_kitchen)

#something apple

#something windows

#magic QT preprocessor?

#load external libs

#load project libs
add_subdirectory(Configuration)
add_subdirectory(Debugging)
add_subdirectory(ImageProcessing)
add_subdirectory(Network)
add_subdirectory(Statistics)
add_subdirectory(Utilities)


#binaries
add_executable(dense_kitchen main.cpp DenseKitchen.cpp)


#link to libraries
target_link_libraries (dense_kitchen 
    Configuration 
    Debugging 
    ImageProcessing 
    Network 
    Statistics 
    Utilities) 