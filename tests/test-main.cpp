//
// Created by admin on 2019-11-07.
//

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

extern "C"  {
#include <libavformat/avformat.h>
};

TEST_CASE("baseline.h264 ���¿� �����ؾ� �Ѵ�.") {
    AVFormatContext *ifmtCtx = nullptr;
    REQUIRE(avformat_open_input(&ifmtCtx, "baseline.h264", nullptr, nullptr) == 0);
}

TEST_CASE("baseline.h264 ���� �� frame �бⰡ �����ؾ� �Ѵ�.") {
    AVFormatContext *ifmtCtx = nullptr;
    REQUIRE(avformat_open_input(&ifmtCtx, "baseline.h264", nullptr, nullptr) == 0);
    AVPacket pkt;
    REQUIRE(av_read_frame(ifmtCtx, &pkt) == 0);
}

#include <iostream>
TEST_CASE("section Ȯ��") {
    AVFormatContext *ifmtCtx = nullptr;
    REQUIRE(avformat_open_input(&ifmtCtx, "baseline.h264", nullptr, nullptr) == 0);
    //std::cout << "open_input" << std::endl;
    SECTION("baseline.h264 ���� �� frame �бⰡ �����ؾ� �Ѵ�.") {
        AVPacket pkt;
        REQUIRE(av_read_frame(ifmtCtx, &pkt) == 0);
        //std::cout << "av_read_frame" << std::endl;
        SECTION("pkt�� duration�� 0�� �ƴϾ�� �Ѵ�.") {
            REQUIRE(pkt.duration != 0);
            REQUIRE(pkt.duration == 96000);
        }
            //std::cout << "check 3" << std::endl;
        SECTION("pkt�� buf�� nullptr�� �ƴϾ�� �Ѵ�.")  {
            REQUIRE(pkt.buf != nullptr);
        }
        SECTION("pkt�� buf�� 3 bytes�� 0�̾�� �Ѵ�.")  {
            REQUIRE(pkt.buf->data[0] == 0);
            REQUIRE(pkt.buf->data[1] == 0);
            REQUIRE(pkt.buf->data[2] == 0);
        }
        SECTION("pkt�� buf�� 4th byte�� 1�̾�� �Ѵ�.")  {
            REQUIRE(pkt.buf->data[3] == 1);
        }
        SECTION("pkt�� buf�� 5th byte�� sps�� ��Ÿ���� �Ѵ�.")  {
            // sps pps idr �� ��� ���ԵǾ� �ִ� �� ����.
            REQUIRE((pkt.buf->data[4] & 0x1f) == 7);
        }
        SECTION("pkt�� buf�� size�� �־�� �Ѵ�.")  {
            REQUIRE(pkt.buf->size > 0);
//            for (int i = 0; i < pkt.buf->size; i++) {
//                std::cout << std::hex << static_cast<int>(pkt.buf->data[i]) << ", ";
//            }
//            std::cout << std::endl;
        }

        REQUIRE(av_read_frame(ifmtCtx, &pkt) == 0);
        //std::cout << "av_read_frame" << std::endl;
        SECTION("�ι�° �о pkt�� duration�� 0�� �ƴϾ�� �Ѵ�.") {
            REQUIRE(pkt.duration != 0);
            REQUIRE(pkt.duration == 96000);
            //std::cout << "check 4" << std::endl;
        }
        SECTION("pkt�� buf�� 3 bytes�� 0�̾�� �Ѵ�.")  {
            REQUIRE(pkt.buf->data[0] == 0);
            REQUIRE(pkt.buf->data[1] == 0);
            REQUIRE(pkt.buf->data[2] == 0);
        }
        SECTION("pkt�� buf�� 4th byte�� 1�� ��Ÿ���� �Ѵ�.")  {
            REQUIRE(pkt.buf->data[3] == 1);
        }
        SECTION("pkt�� buf�� 5th byte�� non-idr�� ��Ÿ���� �Ѵ�.")  {
            REQUIRE((pkt.buf->data[4] & 0x1f) == 1);
        }
        SECTION("pkt�� buf�� size�� �־�� �Ѵ�.")  {
            REQUIRE(pkt.buf->size > 0);
//            for (int i = 0; i < pkt.buf->size; i++) {
//                std::cout << std::hex << static_cast<int>(pkt.buf->data[i]) << ", ";
//            }
//            std::cout << std::endl;
        }

        REQUIRE(av_read_frame(ifmtCtx, &pkt) == 0);
        //std::cout << "av_read_frame" << std::endl;
        SECTION("����° �о pkt�� duration�� 0�� �ƴϾ�� �Ѵ�.")    {
            //std::cout << "av_read_frame" << std::endl;
            REQUIRE(pkt.duration != 0);
            REQUIRE(pkt.duration == 96000);
            //std::cout << "check 4" << std::endl;
            SECTION("pkt�� buf�� 3 bytes�� 0�̾�� �Ѵ�.")  {
                REQUIRE(pkt.buf->data[0] == 0);
                REQUIRE(pkt.buf->data[1] == 0);
                REQUIRE(pkt.buf->data[2] == 0);
            }
            SECTION("pkt�� buf�� 4th byte�� 1�� ��Ÿ���� �Ѵ�.")  {
                REQUIRE(pkt.buf->data[3] == 1);
            }
            SECTION("pkt�� buf�� 5th byte�� non-idr�� ��Ÿ���� �Ѵ�.")  {
                REQUIRE((pkt.buf->data[4] & 0x1f) == 1);
            }
        }
    }
    avformat_close_input(&ifmtCtx);
    //std::cout << "close" << std::endl;
}

TEST_CASE("type Ȯ��") {
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