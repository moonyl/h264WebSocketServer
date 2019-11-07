#include <StreamDispatcher.h>
#include <H264RawStreamService.h>

int main()
{

    StreamDispatcher dispatcher{"baseline.h264"};
    H264RawStreamService rawStrService{5000, [&dispatcher](const QUuid& id, QWebSocket& socket){
        std::cout << "someone : " << id.toString(QUuid::WithoutBraces) << std::endl;
        dispatcher.start([](const uint8_t* buf, int size){
            // TODO : �� �������� ������ �ؾ� �Ѵ�.
            std::cout << "size: " << size << std::endl;
        });
    }};

    dispatcher.join();

    return 0;
}