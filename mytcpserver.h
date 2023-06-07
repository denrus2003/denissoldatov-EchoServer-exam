#ifndef MYSERVER_H
#define MYSERVER_H
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

#include <QtNetwork>
#include <QByteArray>
#include <QDebug>

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    ~MyTcpServer();

    QVector<int> performHeapSortStep(const QVector<int>& array, int step);
    void heapify(QVector<int>& array, int n, int i); // Включение объявления функции heapify
    QString formatResult(const QVector<int>& array);
public slots:
    void slotNewConnection();
    void slotClientDisconnected();

    void slotServerRead();
    //void slotReadClient();
private:
    QTcpServer * mTcpServer;
    QTcpSocket * mTcpSocket;
    int server_status;
};
#endif // MYSERVER_H







