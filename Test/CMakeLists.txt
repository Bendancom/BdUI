set(EXECUTABLE_OUTPUT_PATH ./bin)
link_directories(${LIBRARY_OUTPUT_PATH})

add_executable(Try ./try.cpp)
target_link_libraries(Try BdUI)