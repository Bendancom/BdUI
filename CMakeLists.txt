cmake_minimum_required(VERSION 3.10)
project(BdUI LANGUAGES "CXX")
include_directories(./include)
option("3D" "2D module" ON)
option("Shared" "Compile to Shared lib" OFF)

if(Shared)
set(LIBRARY_OUTPUT_PATH ./bin)
else()
set(LIBRARY_OUTPUT_PATH ./bin/lib)
endif()


set(EXECUTABLE_OUTPUT_PATH ./bin)
link_directories(${PROJECT_SOURCE_DIR}/build/bin/lib)


aux_source_directory(./src SRC)

if(Shared)
add_library(${PROJECT_NAME} SHARED ${SRC})
else()
add_library(${PROJECT_NAME} STATIC ${SRC})
endif()
add_executable(Try ./Debug/try.cpp)
target_link_libraries(Try BdUI)
