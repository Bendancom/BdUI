cmake_minimum_required(VERSION 3.20)
project(BdUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("${PROJECT_SOURCE_DIR}/include")
option("Shared" "Compile to Shared lib" OFF)

find_package(Vulkan REQUIRED)

set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

file(GLOB_RECURSE SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)

if(WIN32)
set(PLATFORM_LIB "dwmapi" "imm32")
endif()
if(Shared)
add_library(${PROJECT_NAME} SHARED ${SRC})
else()
add_library(${PROJECT_NAME} STATIC ${SRC})
endif()
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include" Vulkan-HEADER)
target_link_libraries(${PROJECT_NAME} ${PLATFORM_LIB} Vulkan::Vulkan)

add_subdirectory(${PROJECT_SOURCE_DIR}/Test)