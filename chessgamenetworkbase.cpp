#include "chessgamenetworkbase.h"

void ChessGameNetworkBase::connectToSocket(QTcpSocket *socket) {
    assert(socket);
    this->socket = socket;

    QObject::connect(socket, &QTcpSocket::readyRead, this, &ChessGameNetworkBase::socketDidReceivedData);
}

void ChessGameNetworkBase::socketDidReceivedData() {
    QByteArray buffer = socket->readAll();

    // FIXME
    assert(buffer.size() > 4);
    this->handleReceivedData(buffer);
}

void ChessGameNetworkBase::handleReceivedData(QByteArray buffer) {
    qint32 size, index;
    DataType type;

    QDataStream stream(&buffer, QIODevice::ReadOnly);

    stream >> size >> type >> index;

    if (type == Flip) {
        ChessPoint pos;
        stream >> pos;
        emit didReceiveFlipChessData(pos, index);
    } else {
        ChessPoint src, des;
        stream >> src >> des;
        emit didReceiveMoveChessData(src, des, index);
    }
}

void ChessGameNetworkBase::sendFlipChessData(const ChessPoint &pos, qint32 operationIndex) {
    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::ReadWrite);

    stream << qint32(Flip) << operationIndex << pos;
    sendBytes(bytes);
}

void ChessGameNetworkBase::sendMoveChessData(const ChessPoint &src, const ChessPoint &des, qint32 operationIndex) {
    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::ReadWrite);

    stream << qint32(Move) << operationIndex << src << des;
    sendBytes(bytes);
}

void ChessGameNetworkBase::sendBytes(QByteArray bytes) {
    assert(socket->state() == QAbstractSocket::ConnectedState);

    QByteArray finalData;
    QDataStream stream(&finalData, QIODevice::ReadWrite);
    stream << bytes.size();
    stream << bytes;

    socket->write(finalData);
}
