#include "MqttParserWorker.h"
#include <QDebug>
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

MqttParserWorker::MqttParserWorker(bool debugMode, QObject *parent)
    : QObject(parent),
    m_debugMode(debugMode),
    m_running(true),
    m_messagesParsed(0)
{
    setAutoDelete(true);
}

MqttParserWorker::~MqttParserWorker()
{
    stop();

    // Clear the queue
    QMutexLocker locker(&m_queueMutex);
    m_queue.clear();
}

void MqttParserWorker::run()
{
    if (m_debugMode)
    {
        qDebug() << "MQTT Parser worker started in thread" << QThread::currentThreadId();
    }

    QByteArray message;

    while (m_running.load())
    {
        // Get a message from the queue
        {
            QMutexLocker locker(&m_queueMutex);

            // Wait for data if queue is empty
            while (m_queue.isEmpty() && m_running.load())
            {
                m_queueCondition.wait(&m_queueMutex, 100);
            }

            // Check if we should exit
            if (!m_running.load())
            {
                break;
            }

            // Get the next message
            if (!m_queue.isEmpty())
            {
                message = m_queue.dequeue();
            }
            else
            {
                continue;
            }
        }

        // Parse the message
        parseMessage(message);
    }

    if (m_debugMode)
    {
        qDebug() << "MQTT Parser worker stopped in thread" << QThread::currentThreadId();
    }
}

void MqttParserWorker::queueMessage(const QByteArray &data)
{
    QMutexLocker locker(&m_queueMutex);

    // Add message to queue
    m_queue.enqueue(data);

    // Wake up the worker thread
    m_queueCondition.wakeOne();
}

void MqttParserWorker::stop()
{
    m_running.store(false);

    // Wake up the worker thread
    QMutexLocker locker(&m_queueMutex);
    m_queueCondition.wakeAll();
}

void MqttParserWorker::parseMessage(const QByteArray &message)
{
    try
    {
        // Convert message data to string and split by comma
        QString dataStr = QString::fromUtf8(message).trimmed();
        QStringList parts = dataStr.split(',');

        // Check if we have enough parts (now 15 parts with lateral and longitudinal G)
        if (parts.size() >= 15)
        {
            bool okSpeed = false, okRpm = false, okAcc = false, okBrake = false;
            bool okAngle = false, okTemp = false, okBattery = false;
            bool okGpsLon = false, okGpsLat = false;
            bool okWSFL = false, okWSFR = false, okWSBL = false, okWSBR = false;
            bool okLateralG = false, okLongitudinalG = false;

            // Parse values
            float speed = parts[0].toFloat(&okSpeed);
            int rpm = parts[1].toInt(&okRpm);
            int accPedal = parts[2].toInt(&okAcc);
            int brakePedal = parts[3].toInt(&okBrake);
            double encoderAngle = parts[4].toDouble(&okAngle);
            float temperature = parts[5].toFloat(&okTemp);
            int batteryLevel = parts[6].toInt(&okBattery);
            double gpsLongitude = parts[7].toDouble(&okGpsLon);
            double gpsLatitude = parts[8].toDouble(&okGpsLat);
            int speedFL = parts[9].toInt(&okWSFL);
            int speedFR = parts[10].toInt(&okWSFR);
            int speedBL = parts[11].toInt(&okWSBL);
            int speedBR = parts[12].toInt(&okWSBR);
            double lateralG = parts[13].toDouble(&okLateralG);
            double longitudinalG = parts[14].toDouble(&okLongitudinalG);

            // Check if all values were parsed successfully
            if (okSpeed && okRpm && okAcc && okBrake && okAngle && okTemp &&
                okBattery && okGpsLon && okGpsLat &&
                okWSFL && okWSFR && okWSBL && okWSBR &&
                okLateralG && okLongitudinalG)
            {
                // Emit signal with parsed data
                emit messageParsed(
                    speed, rpm, accPedal, brakePedal,
                    encoderAngle, temperature, batteryLevel,
                    gpsLongitude, gpsLatitude,
                    speedFL, speedFR, speedBL, speedBR,
                    lateralG, longitudinalG);

                // Log debug info
                if (m_debugMode)
                {
                    qDebug() << "MqttParserWorker: Parsed message - Speed:" << speed << "RPM:" << rpm;
                }
            }
            else
            {
                emit errorOccurred("MQTT: Failed to parse some values in message");
            }
        }
        else
        {
            emit errorOccurred(QString("MQTT message has incorrect format (expected 15+ parts, got %1)").arg(parts.size()));
        }
    }
    catch (const std::exception &e)
    {
        emit errorOccurred(QString("MQTT: Exception during parsing: %1").arg(e.what()));
    }
    catch (...)
    {
        emit errorOccurred("MQTT: Unknown exception during parsing");
    }
}


