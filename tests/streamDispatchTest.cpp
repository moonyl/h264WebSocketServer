//
// Created by admin on 2019-11-07.
//

#include <catch2/catch.hpp>

#include <StreamDispatcher.h>

TEST_CASE("StreamDispatcher를 실행한다.")    {
    StreamDispatcher dispatcher("baseline.h264");
    dispatcher.start([](const uint8_t* buf, int size){
        //std::cout << "size: " << size << std::endl;
    });

    REQUIRE(dispatcher.join() == true);
}