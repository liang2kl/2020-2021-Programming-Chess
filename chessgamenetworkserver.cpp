#include "chessgamenetworkserver.h"
#include "constants.h"

ChessGameNetworkServer::ChessGameNetworkServer() {
    server = new QTcpServer();
}

void ChessGameNetworkServer::startListening() {
    server->listen(QHostAddress::Any, Constant::portNumber);
    connect(server, &QTcpServer::newConnection, this, &ChessGameNetworkServer::serverDidInitiateNewConnection);
}

void ChessGameNetworkServer::serverDidInitiateNewConnection() {
    auto *socket = server->nextPendingConnection();
    connectToSocket(socket);
    emit didConnectToHost();
}

void ChessGameNetworkServer::sendChessboardData(QVector<Chess> data) {
    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::ReadWrite);

    stream << qint32(ChessGameNetworkBase::DataType::Chessboard);

    for (auto chess : data) {
        stream << chess;
        qDebug() << chess.position();
    }

    sendBytes(bytes);
}
