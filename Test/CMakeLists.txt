set(EXECUTABLE_OUTPUT_PATH ./bin)
link_directories(${LIBRARY_OUTPUT_PATH})

include_directories(../include)
add_executable(Try ./try.cpp)
target_link_libraries(Try ${PROJECT_NAME})