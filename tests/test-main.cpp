//
// Created by admin on 2019-11-07.
//

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

extern "C"  {
#include <libavformat/avformat.h>
};

TEST_CASE("baseline.h264 오픈에 성공해야 한다.") {
    AVFormatContext *ifmtCtx = nullptr;
    REQUIRE(avformat_open_input(&ifmtCtx, "baseline.h264", nullptr, nullptr) == 0);
}

TEST_CASE("baseline.h264 오픈 후 frame 읽기가 성공해야 한다.") {
    AVFormatContext *ifmtCtx = nullptr;
    REQUIRE(avformat_open_input(&ifmtCtx, "baseline.h264", nullptr, nullptr) == 0);
    AVPacket pkt;
    REQUIRE(av_read_frame(ifmtCtx, &pkt) == 0);
}

#include <iostream>
TEST_CASE("section 확인") {
    AVFormatContext *ifmtCtx = nullptr;
    REQUIRE(avformat_open_input(&ifmtCtx, "baseline.h264", nullptr, nullptr) == 0);
    //std::cout << "open_input" << std::endl;
    SECTION("baseline.h264 오픈 후 frame 읽기가 성공해야 한다.") {
        AVPacket pkt;
        REQUIRE(av_read_frame(ifmtCtx, &pkt) == 0);
        //std::cout << "av_read_frame" << std::endl;
        SECTION("pkt의 duration이 0이 아니어야 한다.") {
            REQUIRE(pkt.duration != 0);
            REQUIRE(pkt.duration == 96000);
        }
            //std::cout << "check 3" << std::endl;
        SECTION("pkt의 buf가 nullptr이 아니어야 한다.")  {
            REQUIRE(pkt.buf != nullptr);
        }
        SECTION("pkt의 buf의 3 bytes는 0이어야 한다.")  {
            REQUIRE(pkt.buf->data[0] == 0);
            REQUIRE(pkt.buf->data[1] == 0);
            REQUIRE(pkt.buf->data[2] == 0);
        }
        SECTION("pkt의 buf의 4th byte는 1이어야 한다.")  {
            REQUIRE(pkt.buf->data[3] == 1);
        }
        SECTION("pkt의 buf의 5th byte는 sps를 나타내야 한다.")  {
            // sps pps idr 이 모두 포함되어 있는 것 같다.
            REQUIRE((pkt.buf->data[4] & 0x1f) == 7);
        }
        SECTION("pkt의 buf의 size는 있어야 한다.")  {
            REQUIRE(pkt.buf->size > 0);
//            for (int i = 0; i < pkt.buf->size; i++) {
//                std::cout << std::hex << static_cast<int>(pkt.buf->data[i]) << ", ";
//            }
//            std::cout << std::endl;
        }

        REQUIRE(av_read_frame(ifmtCtx, &pkt) == 0);
        //std::cout << "av_read_frame" << std::endl;
        SECTION("두번째 읽어도 pkt의 duration이 0이 아니어야 한다.") {
            REQUIRE(pkt.duration != 0);
            REQUIRE(pkt.duration == 96000);
            //std::cout << "check 4" << std::endl;
        }
        SECTION("pkt의 buf의 3 bytes는 0이어야 한다.")  {
            REQUIRE(pkt.buf->data[0] == 0);
            REQUIRE(pkt.buf->data[1] == 0);
            REQUIRE(pkt.buf->data[2] == 0);
        }
        SECTION("pkt의 buf의 4th byte는 1을 나타내야 한다.")  {
            REQUIRE(pkt.buf->data[3] == 1);
        }
        SECTION("pkt의 buf의 5th byte는 non-idr을 나타내야 한다.")  {
            REQUIRE((pkt.buf->data[4] & 0x1f) == 1);
        }
        SECTION("pkt의 buf의 size는 있어야 한다.")  {
            REQUIRE(pkt.buf->size > 0);
//            for (int i = 0; i < pkt.buf->size; i++) {
//                std::cout << std::hex << static_cast<int>(pkt.buf->data[i]) << ", ";
//            }
//            std::cout << std::endl;
        }

        REQUIRE(av_read_frame(ifmtCtx, &pkt) == 0);
        //std::cout << "av_read_frame" << std::endl;
        SECTION("세번째 읽어도 pkt의 duration이 0이 아니어야 한다.")    {
            //std::cout << "av_read_frame" << std::endl;
            REQUIRE(pkt.duration != 0);
            REQUIRE(pkt.duration == 96000);
            //std::cout << "check 4" << std::endl;
            SECTION("pkt의 buf의 3 bytes는 0이어야 한다.")  {
                REQUIRE(pkt.buf->data[0] == 0);
                REQUIRE(pkt.buf->data[1] == 0);
                REQUIRE(pkt.buf->data[2] == 0);
            }
            SECTION("pkt의 buf의 4th byte는 1을 나타내야 한다.")  {
                REQUIRE(pkt.buf->data[3] == 1);
            }
            SECTION("pkt의 buf의 5th byte는 non-idr을 나타내야 한다.")  {
                REQUIRE((pkt.buf->data[4] & 0x1f) == 1);
            }
        }
    }
    avformat_close_input(&ifmtCtx);
    //std::cout << "close" << std::endl;
}

TEST_CASE("type 확인") {
    AVFormatContext *ifmtCtx = nullptr;
    REQUIRE(avformat_open_input(&ifmtCtx, "baseline.h264", nullptr, nullptr) == 0);
    AVPacket pkt;
    int ret;
    do {
        ret = av_read_frame(ifmtCtx, &pkt);
        if (ret != 0) break;
        REQUIRE(pkt.buf->data[0] == 0);
        REQUIRE(pkt.buf->data[1] == 0);
        REQUIRE(pkt.buf->data[2] == 0);
        REQUIRE(pkt.buf->data[3] == 1);
        int type = pkt.buf->data[4] & 0x1f;
//        if (type != 1) {
//            std::cout << std::hex << type << std::endl;
//        }
//        if ((pkt.buf->data[0] != 0) ||
//                (pkt.buf->data[1] != 0) ||
//                (pkt.buf->data[2] != 0) ||
//                (pkt.buf->data[3] != 1))    {
//            //std::cout << "test" << std::endl;
//            std::cout << std::hex << static_cast<int>(pkt.buf->data[0]) << ", "
//                << static_cast<int>(pkt.buf->data[1]) << ", "
//                << static_cast<int>(pkt.buf->data[2]) << ", " << static_cast<int>(pkt.buf->data[3])
//                << std::endl;
//        }
    } while(true);
    REQUIRE(ret != 0);
}