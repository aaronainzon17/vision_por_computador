#!/bin/bash

#cmake  -DOPENCV_ENABLE_NONFREE:BOOL=ON -DBUILD_opencv_xfeatures2d:BOOL=ON -S . -B build 

cmake -D OPENCV_ENABLE_NONFREE=ON -D BUILD_opencv_xfeatures2d=ON -S . -B build

# cd build/
# make
