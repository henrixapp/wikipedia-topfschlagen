project(wikipedia_topfschlagen)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
cmake_minimum_required(VERSION 2.8)
#add_subdirectory (database)
#add_subdirectory (Tree)
#server geräus
add_subdirectory(casablanca/Release)
include_directories(${PROJECT_SOURCE_DIR}/casablanca/Release/include)
file(GLOB SRC_LIST "*.cpp")
#Copieren des Quellcodes
  configure_file(search.cl searching.cl
                 COPYONLY)
#entfernen von test.cpp
list(REMOVE_ITEM SRC_LIST ${PROJECT_SOURCE_DIR}/search.cpp)
list(REMOVE_ITEM SRC_LIST ${PROJECT_SOURCE_DIR}/main.cpp)
list(REMOVE_ITEM SRC_LIST ${PROJECT_SOURCE_DIR}/server.cpp)
list(REMOVE_ITEM SRC_LIST ${PROJECT_SOURCE_DIR}/fardistance_travelling.cpp)
add_executable(vorbereitung ${SRC_LIST} main.cpp)
target_link_libraries (vorbereitung -lxerces-c  -lOpenCL ${Casablanca_LIBRARIES} cpprest)

add_executable(suchen ${SRC_LIST} search.cpp)
target_link_libraries (suchen -lxerces-c  -lOpenCL ${Casablanca_LIBRARIES} cpprest)

add_executable(server ${SRC_LIST} server.cpp)
target_link_libraries (server -lxerces-c  -lOpenCL ${Casablanca_LIBRARIES} cpprest)

add_executable(abstands_bestimmung ${SRC_LIST} fardistance_travelling.cpp)
target_link_libraries (abstands_bestimmung -lxerces-c  -lOpenCL ${Casablanca_LIBRARIES} cpprest)
