//
// Created by admin on 2019-11-07.
//

#pragma once

extern "C"  {
#include <libavformat/avformat.h>
}
#include <QRunnable>
#include <iostream>
#include <QThreadPool>

class StreamDispatcher
{
    AVFormatContext *_ifmtCtx = nullptr;
    std::string _filename;
    using OnReadHandle = std::function<void(const uint8_t*, int size)>;
    //OnReadHandle _onRead;
public:
    StreamDispatcher(const char* filename) : _filename(filename) {}
    ~StreamDispatcher()
    {
        if (_ifmtCtx)   {
            avformat_close_input(&_ifmtCtx);
        }
    }
    void start(OnReadHandle onRead = nullptr)  {
        //_onRead = onRead;
        avformat_open_input(&_ifmtCtx, _filename.c_str(), nullptr, nullptr);

        class ReadRunnable : public QRunnable {
            const AVFormatContext _ifmtCtx;
            OnReadHandle _onRead;
        public:
            ReadRunnable(const AVFormatContext &ifmtCtx, OnReadHandle onRead) : _ifmtCtx(ifmtCtx), _onRead(onRead) {}
            void run() override {
                AVPacket pkt;
                int ret;
                do {
                    ret = av_read_frame(const_cast<AVFormatContext*>(&_ifmtCtx), &pkt);
                    //std::cout << "check read" << std::endl;
                    if (ret < 0)   {
                        break;
                    }
                    if (_onRead) {
                        _onRead(pkt.buf->data, pkt.buf->size);
                    }
                    av_packet_unref(&pkt);
                } while (true);
            }
        };
        QThreadPool::globalInstance()->start(new ReadRunnable{*_ifmtCtx, onRead});
    }

    bool join() {
        return QThreadPool::globalInstance()->waitForDone();
    }
};