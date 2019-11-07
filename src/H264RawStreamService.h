//
// Created by admin on 2019-11-07.
//

#pragma once

#include <QWebSocketServer>
#include <QWebSocket>
#include <iostream>
#include <QRegularExpression>
#include <QUuid>

#include <utils.h>

class H264RawStreamService
{
    QWebSocketServer _server;
    using OnConnecedHandle = std::function<void(const QUuid& id, QWebSocket& socket)>;
    OnConnecedHandle _onConnected;
public:
    H264RawStreamService(qint16 port = 5000, OnConnecedHandle onConnected = nullptr) : _server("H264RawStreamService", QWebSocketServer::NonSecureMode) {
        _server.close();
        _server.listen(QHostAddress::LocalHost, port);
        QObject::connect(&_server, &QWebSocketServer::newConnection, [this]() {
            std::cout << "someone is connected" << std::endl;
            auto client = _server.nextPendingConnection();

            QRegularExpression re("/h264Stream/(\\s*([a-f0-9\\-]*){1}\\s*)/*");
            std::cout << "path: " << client->requestUrl().path() << std::endl;
            auto match = re.match(client->requestUrl().path());
            if (match.hasMatch()) {
                if (_onConnected)   {
                    _onConnected(match.captured(1), *client);
                }
            }
        });
    }
};
