cmake_minimum_required(VERSION 3.10)
project(BdUI LANGUAGES "CXX" VERSION 0.1.0.0)
include_directories(./include)
option("3D" "2D module" ON)
option("Shared" "Compile to Shared lib" ON)

if(Shared)
set(LIBRARY_OUTPUT_PATH ./bin)
else()
set(LIBRARY_OUTPUT_PATH ./bin/lib)
endif()

aux_source_directory(./src SRC)

if(Shared)
add_library(${PROJECT_NAME} SHARED ${SRC})
else()
add_library(${PROJECT_NAME} STATIC ${SRC})
endif()
