cmake_minimum_required(VERSION 3.16.3)
#cmake_minimum_required(VERSION 3.1)
project( Example )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( main main.cpp )
target_link_libraries( main ${OpenCV_LIBS} )

# add_executable( prueba prueba.cpp )
# target_link_libraries( prueba ${OpenCV_LIBS} )

