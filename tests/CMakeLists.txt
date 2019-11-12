project(h264WebSocketServerTest)

#find_package(Catch2 REQUIRED)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in catch-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/catch-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/catch-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/catch-src
        ${CMAKE_CURRENT_BINARY_DIR}/catch-build
        EXCLUDE_FROM_ALL)

add_executable(h264WebSocketServerTest test-main.cpp streamDispatchTest.cpp WebSocketServerTest.cpp)
target_link_libraries(h264WebSocketServerTest
        PRIVATE Catch2::Catch2 h264ServiceLib)

file(COPY F:/test-videos/baseline.h264 DESTINATION ${CMAKE_CURRENT_BINARY_DIR})