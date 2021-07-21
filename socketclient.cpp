#include "socketclient.h"
#include <QtCore/QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>

SocketClient::SocketClient(QUrl url, bool debug, QObject *parent) :
        QObject(parent),
        m_url(url),
        m_debug(debug)
{
    if (m_debug)
        qDebug() << "WebSocket server:" << url;
    connect(&m_webSocket, &QWebSocket::connected, this, &SocketClient::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &SocketClient::closed);
    m_webSocket.open(url);
}

void SocketClient::closeConnection()
{
    m_webSocket.close();
}

void SocketClient::setUrl(QString url)
{
    this->m_url = QUrl(url);

    m_webSocket.close();
    m_webSocket.open(url);
}

void SocketClient::onConnected()
{
    if (m_debug)
        qDebug() << "WebSocket connected";
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &SocketClient::onTextMessageReceived);
}

void SocketClient::onTextMessageReceived(QString message)
{
    if (m_debug)
        qDebug() << "Message received:" << message;

    doc = QJsonDocument::fromJson(message.toUtf8());

    auto root = doc.object();

    filePath = root.find("file").value().toString();

    noteText = root.find("text").value().toString();

    if(filePath != "" and noteText != "")
    {
        emit newNoteReceived();
    }
}

QString SocketClient::getFilePath() const
{
    return this->filePath;
}

void SocketClient::setFilePath(QString &filePath)
{
    this->filePath = filePath;
}

QString SocketClient::getNoteText() const
{
    return noteText;
}

void SocketClient::setNoteText(QString &noteText)
{
    this->noteText = noteText;
}
