#include <StreamDispatcher.h>
#include <H264RawStreamService.h>
#include <QCoreApplication>

void sendRawStreamOnSocketContext(QWebSocket& socket, const uint8_t* buf, int size)
{
    QByteArray rawData((const char*)buf, size);
    QMetaObject::invokeMethod(&socket, [&socket, rawData](){
        socket.sendBinaryMessage(rawData);
        std::cout << "size: " << rawData.size() << std::endl;
    });
    QThread::msleep(30);
}

void sendRawStreamWithShowInfoOnSocketContext(QWebSocket& socket, const uint8_t* buf, int size, bool show = true)
{
    QByteArray packet("psd");
    packet.append(show == true ? 1 : 0);
//    for (int i = 3; i >= 0; i--) {
//        packet.append((size >> i*8) & 0xff);
//    }
    for (int i = 0; i < 4; i++) {
        packet.append((size >> i*8) & 0xff);
    }
    //packet.append(QByteArray::number(size));
    QByteArray rawData((const char*)buf, size);
    packet.append(rawData);
    //QMetaObject::invokeMethod(&socket, [&socket, rawData](){
    QMetaObject::invokeMethod(&socket, [&socket, packet](){
        //socket.sendBinaryMessage(rawData);
        socket.sendBinaryMessage(packet);
        //std::cout << "size: " << rawData.size() << std::endl;
        std::cout << "size: " << packet.size() << std::endl;
    });
    QThread::msleep(30);
}

#include <QMap>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    QMap<QWebSocket*, QSharedPointer<StreamDispatcher>> dispatcherTable;
    H264RawStreamService rawStrService{5000, [&dispatcherTable](const QUuid& id, QWebSocket& socket){
        std::cout << "someone : " << id.toString(QUuid::WithoutBraces) << std::endl;
        // TODO : socket 에 대한 dispatcher를 만들어야 한다.
        dispatcherTable[&socket] = QSharedPointer<StreamDispatcher>::create("baseline.h264");

        auto onRead = [&socket](const uint8_t* buf, int size){
            //sendRawStreamOnSocketContext(socket, buf, size);
            sendRawStreamWithShowInfoOnSocketContext(socket, buf, size, true);
        };
        dispatcherTable[&socket]->start(onRead);
    }};

    int ret = app.exec();

    //dispatcher.join();
    return ret;
}