
#include "serialreceiverworker.h"
#include <QDebug>

SerialReceiverWorker::SerialReceiverWorker(QObject *parent)
    : QObject(parent),
    m_serialPort(nullptr),
    m_receiving(false)
{
}

SerialReceiverWorker::~SerialReceiverWorker()
{
    stopReceiving();
    if (m_serialPort)
    {
        m_serialPort->deleteLater();
    }
}

void SerialReceiverWorker::initialize()
{
    // This slot is called when the worker's thread starts.
    // Any thread-specific initialization should happen here.
    if (!m_serialPort)
    {
        m_serialPort = new QSerialPort(this);
        connect(m_serialPort, &QSerialPort::readyRead, this, &SerialReceiverWorker::readSerialData);
        connect(m_serialPort, &QSerialPort::errorOccurred, this, &SerialReceiverWorker::handleError);
    }
}

void SerialReceiverWorker::startReceiving(const QString &portName, qint32 baudRate)
{
    if (!m_serialPort)
    {
        emit errorOccurred("Serial port not initialized.");
        return;
    }

    if (m_serialPort->isOpen())
    {
        m_serialPort->close();
    }

    m_serialPort->setPortName(portName);
    m_serialPort->setBaudRate(baudRate);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (m_serialPort->open(QIODevice::ReadOnly))
    {
        m_receiving = true;
        qDebug() << "SerialReceiverWorker: Started receiving on" << portName << "at" << baudRate << "baud.";
    }
    else
    {
        m_receiving = false;
        emit errorOccurred(QString("Failed to open serial port %1: %2").arg(portName).arg(m_serialPort->errorString()));
    }
}

void SerialReceiverWorker::stopReceiving()
{
    if (m_serialPort && m_serialPort->isOpen())
    {
        m_serialPort->close();
        m_receiving = false;
        qDebug() << "SerialReceiverWorker: Stopped receiving.";
    }
}

void SerialReceiverWorker::readSerialData()
{
    if (m_receiving && m_serialPort->bytesAvailable() > 0)
    {
        QByteArray data = m_serialPort->readAll();
        emit serialDataReceived(data);
    }
}

void SerialReceiverWorker::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError != QSerialPort::NoError)
    {
        emit errorOccurred(m_serialPort->errorString());
    }
}

