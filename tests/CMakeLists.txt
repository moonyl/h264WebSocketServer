project(h264WebSocketServerTest)

find_package(Catch2 REQUIRED)

add_executable(h264WebSocketServerTest test-main.cpp streamDispatchTest.cpp WebSocketServerTest.cpp)
target_link_libraries(h264WebSocketServerTest
        PRIVATE Catch2::Catch2 h264ServiceLib)

file(COPY F:/test-videos/baseline.h264 DESTINATION ${CMAKE_CURRENT_BINARY_DIR})