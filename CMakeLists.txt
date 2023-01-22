cmake_minimum_required(VERSION 3.18)
project(BdUI LANGUAGES "CXX")
include_directories(${PROJECT_SOURCE_DIR}/include)

option("Shared" "Compile to Shared lib" OFF)

set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(${PROJECT_SOURCE_DIR}/lib)
set(LIB glad)

file(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)

message(STATUS ${OPENGL})
if(WIN32)
set(PLATFORM_LIB "dwmapi" "opengl32" "imm32" glad_wgl)
endif()
if(Shared)
add_library(${PROJECT_NAME} SHARED ${SRC})
else()
add_library(${PROJECT_NAME} STATIC ${SRC})
endif()
target_link_libraries(${PROJECT_NAME} ${LIB} ${PLATFORM_LIB})

add_subdirectory(${PROJECT_SOURCE_DIR}/Test)