cmake_minimum_required(VERSION 3.12)
project(VoronoiDiagram)

set(CMAKE_CXX_STANDARD 17)

add_executable(VoronoiDiagram main.cpp)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/Modules" ${CMAKE_MODULE_PATH})

find_package(SFML 2 REQUIRED graphics window system )
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(VoronoiDiagram ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()
