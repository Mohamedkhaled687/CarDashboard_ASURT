
#include "MqttClient.h"
#include "MqttReceiverWorker.h"
#include "MqttParserWorker.h"
#include <QDebug>
#include <QThread>

MqttClient::MqttClient(QObject *parent)
    : QObject(parent),
    m_nextParserIndex(0),
    m_parserThreadCount(QThread::idealThreadCount()),
    m_debugMode(true),
    m_messagesProcessed(0),
    m_messagesDropped(0),
    m_speed(0.0f),
    m_rpm(0),
    m_accPedal(0),
    m_brakePedal(0),
    m_encoderAngle(0.0),
    m_temperature(0.0f),
    m_batteryLevel(0),
    m_gpsLongitude(0.0),
    m_gpsLatitude(0.0),
    m_speedFL(0),
    m_speedFR(0),
    m_speedBL(0),
    m_speedBR(0),
    m_lateralG(0.0),
    m_longitudinalG(0.0)
{
    m_receiverWorker = new MqttReceiverWorker();
    m_receiverWorker->moveToThread(&m_receiverThread);

    connect(this, &MqttClient::startReceiving, m_receiverWorker, &MqttReceiverWorker::startReceiving, Qt::QueuedConnection);
    connect(this, &MqttClient::stopReceiving, m_receiverWorker, &MqttReceiverWorker::stopReceiving, Qt::QueuedConnection);
    connect(m_receiverWorker, &MqttReceiverWorker::messageReceived, this, &MqttClient::handleMqttMessageReceived, Qt::QueuedConnection);
    connect(m_receiverWorker, &MqttReceiverWorker::errorOccurred, this, &MqttClient::handleError, Qt::QueuedConnection);

    connect(&m_receiverThread, &QThread::started, m_receiverWorker, &MqttReceiverWorker::initialize);
    connect(&m_receiverThread, &QThread::finished, m_receiverWorker, &QObject::deleteLater);

    m_parserPool.setMaxThreadCount(m_parserThreadCount);
    m_receiverThread.setPriority(QThread::HighPriority);
}

MqttClient::~MqttClient()
{
    stop();

    if (m_receiverThread.isRunning())
    {
        m_receiverThread.quit();
        m_receiverThread.wait();
    }

    cleanupParsers();
}

bool MqttClient::start(const QString &brokerAddress, quint16 port, bool useTls, const QString &clientId, const QString &username, const QString &password, const QString &topic)
{
    QThread::currentThread()->setObjectName("Main Thread");

    stop();
    initializeParsers();

    m_receiverThread.start();

    emit startReceiving(brokerAddress, port, useTls, clientId, username, password, topic);

    if (m_debugMode)
    {
        qDebug() << "MQTT Client started on broker" << brokerAddress << ":" << port << "running on the " << QThread::currentThread()
        << "with" << m_parserThreadCount << "parser threads";
    }

    return true;
}

bool MqttClient::stop()
{
    emit stopReceiving();
    cleanupParsers();

    if (m_debugMode)
    {
        qDebug() << "MQTT Client stopped";
    }

    return true;
}

void MqttClient::setParserThreadCount(int count)
{
    if (count > 0 && count <= QThread::idealThreadCount() * 2)
    {
        m_parserThreadCount = count;
        m_parserPool.setMaxThreadCount(m_parserThreadCount);

        if (m_debugMode)
        {
            qDebug() << "Parser thread count set to" << count;
        }
    }
}

void MqttClient::setDebugMode(bool enabled)
{
    m_debugMode = enabled;

    if (m_debugMode)
    {
        qDebug() << "Debug mode enabled";
    }
}

void MqttClient::handleMqttMessageReceived(const QByteArray &message)
{
    if (!m_parsers.isEmpty())
    {
        MqttParserWorker *parser = m_parsers[m_nextParserIndex];
        parser->queueMessage(message);
        m_nextParserIndex = (m_nextParserIndex + 1) % m_parsers.size();
    }
}

void MqttClient::handleParsedData(float speed, int rpm, int accPedal, int brakePedal,
                                  double encoderAngle, float temperature, int batteryLevel,
                                  double gpsLongitude, double gpsLatitude,
                                  int speedFL, int speedFR, int speedBL, int speedBR,
                                  double lateralG, double longitudinalG)
{
    m_messagesProcessed.fetch_add(1);

    float oldSpeed = m_speed.load(std::memory_order_relaxed);
    if (!qFuzzyCompare(oldSpeed, speed))
    {
        m_speed.store(speed, std::memory_order_relaxed);
        emit speedChanged(speed);
    }

    int oldRpm = m_rpm.load(std::memory_order_relaxed);
    if (oldRpm != rpm)
    {
        m_rpm.store(rpm, std::memory_order_relaxed);
        emit rpmChanged(rpm);
    }

    int oldAccPedal = m_accPedal.load(std::memory_order_relaxed);
    if (oldAccPedal != accPedal)
    {
        m_accPedal.store(accPedal, std::memory_order_relaxed);
        emit accPedalChanged(accPedal);
    }

    int oldBrakePedal = m_brakePedal.load(std::memory_order_relaxed);
    if (oldBrakePedal != brakePedal)
    {
        m_brakePedal.store(brakePedal, std::memory_order_relaxed);
        emit brakePedalChanged(brakePedal);
    }

    double oldEncoderAngle = m_encoderAngle.load(std::memory_order_relaxed);
    if (!qFuzzyCompare(oldEncoderAngle, encoderAngle))
    {
        m_encoderAngle.store(encoderAngle, std::memory_order_relaxed);
        emit encoderAngleChanged(encoderAngle);
    }

    float oldTemperature = m_temperature.load(std::memory_order_relaxed);
    if (!qFuzzyCompare(oldTemperature, temperature))
    {
        m_temperature.store(temperature, std::memory_order_relaxed);
        emit temperatureChanged(temperature);
    }

    int oldBatteryLevel = m_batteryLevel.load(std::memory_order_relaxed);
    if (oldBatteryLevel != batteryLevel)
    {
        m_batteryLevel.store(batteryLevel, std::memory_order_relaxed);
        emit batteryLevelChanged(batteryLevel);
    }

    double oldGpsLongitude = m_gpsLongitude.load(std::memory_order_relaxed);
    if (!qFuzzyCompare(oldGpsLongitude, gpsLongitude))
    {
        m_gpsLongitude.store(gpsLongitude, std::memory_order_relaxed);
        emit gpsLongitudeChanged(gpsLongitude);
    }

    double oldGpsLatitude = m_gpsLatitude.load(std::memory_order_relaxed);
    if (!qFuzzyCompare(oldGpsLatitude, gpsLatitude))
    {
        m_gpsLatitude.store(gpsLatitude, std::memory_order_relaxed);
        emit gpsLatitudeChanged(gpsLatitude);
    }

    int oldSpeedFL = m_speedFL.load(std::memory_order_relaxed);
    if (oldSpeedFL != speedFL)
    {
        m_speedFL.store(speedFL, std::memory_order_relaxed);
        emit speedFLChanged(speedFL);
    }

    int oldSpeedFR = m_speedFR.load(std::memory_order_relaxed);
    if (oldSpeedFR != speedFR)
    {
        m_speedFR.store(speedFR, std::memory_order_relaxed);
        emit speedFRChanged(speedFR);
    }

    int oldSpeedBL = m_speedBL.load(std::memory_order_relaxed);
    if (oldSpeedBL != speedBL)
    {
        m_speedBL.store(speedBL, std::memory_order_relaxed);
        emit speedBLChanged(speedBL);
    }

    int oldSpeedBR = m_speedBR.load(std::memory_order_relaxed);
    if (oldSpeedBR != speedBR)
    {
        m_speedBR.store(speedBR, std::memory_order_relaxed);
        emit speedBRChanged(speedBR);
    }

    double oldLateralG = m_lateralG.load(std::memory_order_relaxed);
    if (!qFuzzyCompare(oldLateralG, lateralG))
    {
        m_lateralG.store(lateralG, std::memory_order_relaxed);
        emit lateralGChanged(lateralG);
    }

    double oldLongitudinalG = m_longitudinalG.load(std::memory_order_relaxed);
    if (!qFuzzyCompare(oldLongitudinalG, longitudinalG))
    {
        m_longitudinalG.store(longitudinalG, std::memory_order_relaxed);
        emit longitudinalGChanged(longitudinalG);
    }
}

void MqttClient::handleError(const QString &error)
{
    if (m_debugMode)
    {
        qDebug() << "MQTT Client error:" << error;
    }

    emit errorOccurred(error);
}

void MqttClient::initializeParsers()
{
    for (int i = 0; i < m_parserThreadCount; ++i)
    {
        MqttParserWorker *parser = new MqttParserWorker(m_debugMode);

        connect(parser, &MqttParserWorker::messageParsed, this, &MqttClient::handleParsedData, Qt::QueuedConnection);
        connect(parser, &MqttParserWorker::errorOccurred, this, &MqttClient::handleError, Qt::QueuedConnection);

        m_parsers.append(parser);
        m_parserPool.start(parser);
    }

    m_nextParserIndex = 0;
}

void MqttClient::cleanupParsers()
{
    for (MqttParserWorker *parser : m_parsers)
    {
        parser->stop();
    }

    m_parserPool.waitForDone();

    for (MqttParserWorker *parser : m_parsers)
    {
        disconnect(parser, &MqttParserWorker::messageParsed, this, &MqttClient::handleParsedData);
        disconnect(parser, &MqttParserWorker::errorOccurred, this, &MqttClient::handleError);
    }

    qDeleteAll(m_parsers);
    m_parsers.clear();
}


