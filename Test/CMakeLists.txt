project(Test)
set(EXECUTABLE_OUTPUT_PATH ./bin)
link_directories(${LIBRARY_OUTPUT_PATH})

include_directories(../include)
add_executable(${PROJECT_NAME} ./try.cpp)
target_link_libraries(${PROJECT_NAME} ${CMAKE_PROJECT_NAME})