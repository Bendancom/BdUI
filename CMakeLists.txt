project(BdUI LANGUAGE c_plus_plus VERSION 0.1.0.0)
include_directories(./include)
option("3D" "2D module" ON)
option("IsShared" "Compile to Shared lib" ON)

if(IsShared)
set(LIBRARY_OUTPUT_PATH ./bin)
else()
set(LIBRARY_OUTPUT_PATH ./bin/lib)
endif()

aux_source_directory(./src SRC)

if(IsShared)
add_library(${PROJECT_NAME} SHARED ${SRC})
else()
add_library(${PROJECT_NAME} STATIC ${SRC})
endif()
