cmake_minimum_required(VERSION 3.1)
project( Example )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_library(descriptores descriptores.cpp descriptores.hpp)

add_library(aprender aprender.cpp aprender.hpp)
add_library(reconocer reconocer.cpp reconocer.hpp)

add_executable( main main.cpp )

target_link_libraries(descriptores ${OpenCV_LIBS})
target_link_libraries( reconocer ${OpenCV_LIBS} descriptores)
target_link_libraries( aprender ${OpenCV_LIBS} descriptores reconocer)
target_link_libraries( main aprender reconocer)
#target_link_libraries( main reconocer)