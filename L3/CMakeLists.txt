cmake_minimum_required(VERSION 3.1)
project( Example )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_library(descriptores descriptores.cpp descriptores.hpp)

add_executable( main main.cpp )
target_link_libraries( main ${OpenCV_LIBS} descriptores)