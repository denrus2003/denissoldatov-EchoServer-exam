#include "mytcpserver.h"
#include <QCoreApplication>
#include <QStringList>

MyTcpServer::~MyTcpServer()
{
    mTcpServer->close();
    server_status = 0;
}

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, 33333)){
        qDebug() << "Server is not started";
    } else {
        server_status = 1;
        qDebug() << "Server is started";
    }
}

void MyTcpServer::slotNewConnection()
{
    if(server_status == 1){
        mTcpSocket = mTcpServer->nextPendingConnection();
        mTcpSocket->write("Hello, World!!! I am echo server!\r\n");
        connect(mTcpSocket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
        connect(mTcpSocket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);
    }
}

void MyTcpServer::slotServerRead()
{
    while(mTcpSocket->bytesAvailable() > 0)
    {
        QByteArray data = mTcpSocket->readAll();
        QString message = QString::fromUtf8(data).trimmed();

        if (message.startsWith("sort&")) {
            QStringList parts = message.split('&');
            if (parts.size() >= 3) {
                int step = parts[1].toInt();
                QStringList numbers = parts[2].split(' ');
                QVector<int> array;
                for (const QString& number : numbers) {
                    array.append(number.toInt());
                }

                // Отправка результата сортировки шага номер <step>
                QVector<int> sortedArray = performHeapSortStep(array, step);
                QString result = formatResult(sortedArray);
                mTcpSocket->write(result.toUtf8());
            }
        }
    }
}

void MyTcpServer::slotClientDisconnected()
{
    mTcpSocket->close();
}

QVector<int> MyTcpServer::performHeapSortStep(const QVector<int>& array, int step)
{
    QVector<int> sortedArray = array; // Создайте копию исходного массива

    // Выполнение одного шага алгоритма пирамидальной сортировки
    int n = sortedArray.size();
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(sortedArray, n, i);

    for (int i = n - 1; i > 0; i--) {
        std::swap(sortedArray[0], sortedArray[i]);
        heapify(sortedArray, i, 0);
    }

    // Возвращение отсортированного массива
    return sortedArray;
}

void MyTcpServer::heapify(QVector<int>& array, int n, int i)
{
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && array[l] > array[largest])
        largest = l;

    if (r < n && array[r] > array[largest])
        largest = r;

    if (largest != i) {
        std::swap(array[i], array[largest]);
        heapify(array, n, largest);
    }
}



    QString MyTcpServer::formatResult(const QVector<int>& array)
    {
        QString result;

        // Объединяем элементы массива через пробел
        for (int i = 0; i < array.size(); ++i) {
            result += QString::number(array[i]);
            if (i < array.size() - 1) {
            result += " ";
            }
        }

        return result;
    }
