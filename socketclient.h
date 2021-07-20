#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H
#include <QTcpSocket>
#include <QtWebSockets/QWebSocket>

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QJsonDocument>

class SocketClient : public QObject
{
Q_OBJECT
public:
    explicit SocketClient(QUrl url = QUrl("ws://localhost:12345"), bool debug = false, QObject *parent = Q_NULLPTR);

    void closeConnection();

    QString getFilePath() const;
    void setFilePath(QString &filePath);

    QString getNoteText() const;
    void setNoteText(QString &noteText);


    void syncNotes (QString data)
    {
        m_webSocket.sendTextMessage(QString(data));
    }

    void syncNotes (QByteArray data)
    {
        m_webSocket.sendBinaryMessage(data);
    }
//    template <typename T>
//    void syncNotes(T data)
//    {
//#include <type_traits>
//        if(std::is_same<T, QString>::value)
//        {
//            m_webSocket.sendTextMessage(QString(data));
//        } else if(std::is_same<T, QByteArray>::value) {
//            m_webSocket.sendBinaryMessage(data);
//        }
//    }

    void setUrl(QString url);

Q_SIGNALS:
    void closed();
    void newNoteReceived();

private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(QString message);

private:
    QString noteText = "";
    QString filePath = "";
    QJsonDocument doc;
    QString commitHistory;
    QString lastMessage;
    QWebSocket m_webSocket;
    QUrl m_url;
    bool m_debug;



};
#endif // SOCKETCLIENT_H
