#include "udpreceiverworker.h"
#include <QDebug>
#include <QNetworkDatagram>
#include <QThread>

/*A dedicated worker class that runs in its own thread. It owns the QUdpSocket and listens for incoming datagrams.
 *  When data is available, it processes the datagrams, updates throughput statistics,
 *   and emits signals to pass the raw data to parser workers.
 */

UdpReceiverWorker::UdpReceiverWorker(QObject *parent)
    : QObject(parent),
    m_running(false),
    m_datagramsReceived(0),
    m_bytesReceived(0)
{
    m_socket = new QUdpSocket(this);

    // Connect readyRead signal to processPendingDatagrams slot
    connect(m_socket, &QUdpSocket::readyRead, this, &UdpReceiverWorker::processPendingDatagrams);
}

UdpReceiverWorker::~UdpReceiverWorker()
{
    stopReceiving();
    m_socket->deleteLater();
}

void UdpReceiverWorker::initialize()
{
    m_statsTimer.start();
}

void UdpReceiverWorker::startReceiving(quint16 port)
{
    qDebug() << "UdpReceiver receives on" << QThread::currentThread();
    // Close socket if it's already open
    if (m_socket->state() != QAbstractSocket::UnconnectedState)
    {
        m_socket->close();
    }

    // Bind socket to the specified port
    if (!m_socket->bind(QHostAddress::Any, port))
    {
        emit errorOccurred(QString("Failed to bind UDP socket to port %1: %2")
                               .arg(port)
                               .arg(m_socket->errorString()));
        return;
    }

    m_running = true;
    m_datagramsReceived = 0;
    m_bytesReceived = 0;
    m_statsTimer.restart();
}

void UdpReceiverWorker::stopReceiving()
{
    m_running = false;
    m_socket->close();
}

void UdpReceiverWorker::processPendingDatagrams()
{
    // Process all pending datagrams
    while (m_socket->hasPendingDatagrams() && m_running)
    {
        QNetworkDatagram datagram = m_socket->receiveDatagram();
        QByteArray data = datagram.data();

        // Update statistics
        m_datagramsReceived++;
        m_bytesReceived += data.size();

        // Emit signal with datagram data
        emit datagramReceived(data);

    }
}
