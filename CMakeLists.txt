cmake_minimum_required (VERSION 2.9.0)
project (BarcodeDetectionHough)

find_package(OpenCV 3.4 REQUIRED)

add_definitions(-std=c++11)

find_package(Boost COMPONENTS filesystem system chrono program_options REQUIRED)
include_directories (${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
add_definitions(${Boost_DEFINITIONS})

include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/utils-3rdparty")

file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB SOURCES_3RD_PARTY "${CMAKE_SOURCE_DIR}/utils-3rdparty/*.cpp")
add_library(BarcodeLibrary SHARED ${SOURCES})
add_library(Barcode3rdParty SHARED ${SOURCES_3RD_PARTY})

add_executable (barcodedetection main.cpp)

target_link_libraries(barcodedetection BarcodeLibrary Barcode3rdParty ${OpenCV_LIBS} ${Boost_LIBRARIES})
