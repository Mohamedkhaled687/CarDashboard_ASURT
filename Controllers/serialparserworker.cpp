
#include "serialparserworker.h"
#include <QDebug>
#include <QDataStream>

SerialParserWorker::SerialParserWorker(bool debugMode, QObject *parent)
    : QObject(parent),
    m_running(true),
    m_debugMode(debugMode)
{
    setAutoDelete(true);

}

SerialParserWorker::~SerialParserWorker()
{
    stop();
}

void SerialParserWorker::queueData(const QByteArray &data)
{
    QMutexLocker locker(&m_mutex);
    m_dataQueue.enqueue(data);
    m_waitCondition.wakeOne();
}

void SerialParserWorker::stop()
{
    QMutexLocker locker(&m_mutex);
    m_running = false;
    m_waitCondition.wakeOne(); // Wake up the thread to let it exit the run loop
}

void SerialParserWorker::run()
{
    while (m_running)
    {
        QByteArray data;
        {
            QMutexLocker locker(&m_mutex);
            if (m_dataQueue.isEmpty())
            {
                m_waitCondition.wait(locker.mutex());
            }
            if (!m_running)
            {
                break;
            }
            if (!m_dataQueue.isEmpty())
            {
                data = m_dataQueue.dequeue();
            }
        }

        if (!data.isEmpty())
        {
            parseData(data);
        }
    }
    if (m_debugMode)
    {
        qDebug() << "SerialParserWorker: Exiting run loop.";
    }
}

void SerialParserWorker::parseData(const QByteArray &data)
{
    QString dataString = QString::fromUtf8(data).trimmed();
    QStringList fields = dataString.split(",");

    // Expected number of fields based on the MCU format
    const int expectedFields = 15;

    if (fields.size() != expectedFields)
    {
        if (m_debugMode)
        {
            qDebug() << "SerialParserWorker: Received incomplete or malformed data. Expected" << expectedFields << "fields, got" << fields.size() << ":" << dataString;
        }
        emit errorOccurred("Incomplete or malformed serial data received.");
        return;
    }

    // Parse data based on the MCU format:
    // speed, rpm, accPedal, brakePedal, encoderAngle,
    // temperature, batteryLevel, gpsLongitude, gpsLatitude,
    // frWheelSpeed, flWheelSpeed, brWheelSpeed, blWheelSpeed,
    // lateralG, longitudinalG

    bool ok;
    float speed = fields.at(0).toFloat(&ok);
    if (!ok) { if (m_debugMode) qDebug() << "Failed to parse speed"; return; }
    int rpm = fields.at(1).toInt(&ok);
    if (!ok) { if (m_debugMode) qDebug() << "Failed to parse rpm"; return; }
    int accPedal = fields.at(2).toInt(&ok);
    if (!ok) { if (m_debugMode) qDebug() << "Failed to parse accPedal"; return; }
    int brakePedal = fields.at(3).toInt(&ok);
    if (!ok) { if (m_debugMode) qDebug() << "Failed to parse brakePedal"; return; }
    double encoderAngle = fields.at(4).toDouble(&ok);
    if (!ok) { if (m_debugMode) qDebug() << "Failed to parse encoderAngle"; return; }
    float temperature = fields.at(5).toFloat(&ok);
    if (!ok) { if (m_debugMode) qDebug() << "Failed to parse temperature"; return; }
    int batteryLevel = fields.at(6).toInt(&ok);
    if (!ok) { if (m_debugMode) qDebug() << "Failed to parse batteryLevel"; return; }
    double gpsLongitude = fields.at(7).toDouble(&ok);
    if (!ok) { if (m_debugMode) qDebug() << "Failed to parse gpsLongitude"; return; }
    double gpsLatitude = fields.at(8).toDouble(&ok);
    if (!ok) { if (m_debugMode) qDebug() << "Failed to parse gpsLatitude"; return; }
    int speedFR = fields.at(9).toInt(&ok); // frWheelSpeed
    if (!ok) { if (m_debugMode) qDebug() << "Failed to parse speedFR"; return; }
    int speedFL = fields.at(10).toInt(&ok); // flWheelSpeed
    if (!ok) { if (m_debugMode) qDebug() << "Failed to parse speedFL"; return; }
    int speedBR = fields.at(11).toInt(&ok); // brWheelSpeed
    if (!ok) { if (m_debugMode) qDebug() << "Failed to parse speedBR"; return; }
    int speedBL = fields.at(12).toInt(&ok); // blWheelSpeed
    if (!ok) { if (m_debugMode) qDebug() << "Failed to parse speedBL"; return; }
    double lateralG = fields.at(13).toDouble(&ok);
    if (!ok) { if (m_debugMode) qDebug() << "Failed to parse lateralG"; return; }
    double longitudinalG = fields.at(14).toDouble(&ok);
    if (!ok) { if (m_debugMode) qDebug() << "Failed to parse longitudinalG"; return; }

    emit dataParsed(speed, rpm, accPedal, brakePedal, encoderAngle, temperature, batteryLevel,
                    gpsLongitude, gpsLatitude, speedFL, speedFR, speedBL, speedBR,
                    lateralG, longitudinalG);
}



