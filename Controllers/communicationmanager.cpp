#include "CommunicationManager.h"
#include "serialmanager.h"
#include "udpclient.h"
#include <QDebug>

CommunicationManager::CommunicationManager(QObject *parent)
    : QObject(parent),
    m_udpClient(new UdpClient(this)),
    m_serialManager(new SerialManager(this)),
    m_currentSource(SourceType::None),
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
    m_longitudinalG(0.0),
    m_isSerialSource(false)
{
    // Connect signals from UdpClient to CommunicationManager's slots
    connect(m_udpClient, &UdpClient::speedChanged, this, &CommunicationManager::handleUdpSpeedChanged);
    connect(m_udpClient, &UdpClient::rpmChanged, this, &CommunicationManager::handleUdpRpmChanged);
    connect(m_udpClient, &UdpClient::accPedalChanged, this, &CommunicationManager::handleUdpAccPedalChanged);
    connect(m_udpClient, &UdpClient::brakePedalChanged, this, &CommunicationManager::handleUdpBrakePedalChanged);
    connect(m_udpClient, &UdpClient::encoderAngleChanged, this, &CommunicationManager::handleUdpEncoderAngleChanged);
    connect(m_udpClient, &UdpClient::temperatureChanged, this, &CommunicationManager::handleUdpTemperatureChanged);
    connect(m_udpClient, &UdpClient::batteryLevelChanged, this, &CommunicationManager::handleUdpBatteryLevelChanged);
    connect(m_udpClient, &UdpClient::gpsLongitudeChanged, this, &CommunicationManager::handleUdpGpsLongitudeChanged);
    connect(m_udpClient, &UdpClient::gpsLatitudeChanged, this, &CommunicationManager::handleUdpGpsLatitudeChanged);
    connect(m_udpClient, &UdpClient::speedFLChanged, this, &CommunicationManager::handleUdpSpeedFLChanged);
    connect(m_udpClient, &UdpClient::speedFRChanged, this, &CommunicationManager::handleUdpSpeedFRChanged);
    connect(m_udpClient, &UdpClient::speedBLChanged, this, &CommunicationManager::handleUdpSpeedBLChanged);
    connect(m_udpClient, &UdpClient::speedBRChanged, this, &CommunicationManager::handleUdpSpeedBRChanged);
    connect(m_udpClient, &UdpClient::lateralGChanged, this, &CommunicationManager::handleUdpLateralGChanged);
    connect(m_udpClient, &UdpClient::longitudinalGChanged, this, &CommunicationManager::handleUdpLongitudinalGChanged);
    connect(m_udpClient, &UdpClient::errorOccurred, this, &CommunicationManager::handleUdpError);

    // Connect signals from SerialManager to CommunicationManager's slots
    connect(m_serialManager, &SerialManager::speedChanged, this, &CommunicationManager::handleSerialSpeedChanged);
    connect(m_serialManager, &SerialManager::rpmChanged, this, &CommunicationManager::handleSerialRpmChanged);
    connect(m_serialManager, &SerialManager::accPedalChanged, this, &CommunicationManager::handleSerialAccPedalChanged);
    connect(m_serialManager, &SerialManager::brakePedalChanged, this, &CommunicationManager::handleSerialBrakePedalChanged);
    connect(m_serialManager, &SerialManager::encoderAngleChanged, this, &CommunicationManager::handleSerialEncoderAngleChanged);
    connect(m_serialManager, &SerialManager::temperatureChanged, this, &CommunicationManager::handleSerialTemperatureChanged);
    connect(m_serialManager, &SerialManager::batteryLevelChanged, this, &CommunicationManager::handleSerialBatteryLevelChanged);
    connect(m_serialManager, &SerialManager::gpsLongitudeChanged, this, &CommunicationManager::handleSerialGpsLongitudeChanged);
    connect(m_serialManager, &SerialManager::gpsLatitudeChanged, this, &CommunicationManager::handleSerialGpsLatitudeChanged);
    connect(m_serialManager, &SerialManager::speedFLChanged, this, &CommunicationManager::handleSerialSpeedFLChanged);
    connect(m_serialManager, &SerialManager::speedFRChanged, this, &CommunicationManager::handleSerialSpeedFRChanged);
    connect(m_serialManager, &SerialManager::speedBLChanged, this, &CommunicationManager::handleSerialSpeedBLChanged);
    connect(m_serialManager, &SerialManager::speedBRChanged, this, &CommunicationManager::handleSerialSpeedBRChanged);
    connect(m_serialManager, &SerialManager::lateralGChanged, this, &CommunicationManager::handleSerialLateralGChanged);
    connect(m_serialManager, &SerialManager::longitudinalGChanged, this, &CommunicationManager::handleSerialLongitudinalGChanged);
    connect(m_serialManager, &SerialManager::errorOccurred, this, &CommunicationManager::handleSerialError);
}

CommunicationManager::~CommunicationManager()
{
    stop();
}

bool CommunicationManager::startSerial(const QString &portName, qint32 baudRate)
{
    stop(); // Stop any active communication first
    bool success = m_serialManager->start(portName, baudRate);
    if (success) {
        m_currentSource = SourceType::Serial;
        setIsSerialSource(true);
        qDebug() << "CommunicationManager: Serial started.";
    } else {
        qDebug() << "CommunicationManager: Failed to start Serial.";
    }
    return success;
}

bool CommunicationManager::startUdp(quint16 port)
{
    stop(); // Stop any active communication first
    bool success = m_udpClient->start(port);
    if (success) {
        m_currentSource = SourceType::Udp;
        setIsSerialSource(false);
        qDebug() << "CommunicationManager: UDP started.";
    } else {
        qDebug() << "CommunicationManager: Failed to start UDP.";
    }
    return success;
}

bool CommunicationManager::stop()
{
    bool success = true;
    if (m_currentSource == SourceType::Serial) {
        success = m_serialManager->stop();
        qDebug() << "CommunicationManager: Serial stopped.";
    } else if (m_currentSource == SourceType::Udp) {
        success = m_udpClient->stop();
        qDebug() << "CommunicationManager: UDP stopped.";
    }
    m_currentSource = SourceType::None;
    return success;
}

void CommunicationManager::setIsSerialSource(bool isSerialSource)
{
    if (m_isSerialSource != isSerialSource) {
        m_isSerialSource = isSerialSource;
        emit isSerialSourceChanged(m_isSerialSource);
    }
}



void CommunicationManager::handleUdpError(const QString &error)
{
    emit errorOccurred(error);
}

void CommunicationManager::handleSerialError(const QString &error)
{
    emit errorOccurred(error);
}






void CommunicationManager::handleUdpSpeedChanged(float newSpeed)
{
    if (m_currentSource == SourceType::Udp) {
        if (m_speed != newSpeed) {
            m_speed = newSpeed;
            emit speedChanged(m_speed);
        }
    }
}

void CommunicationManager::handleUdpRpmChanged(int newRpm)
{
    if (m_currentSource == SourceType::Udp) {
        if (m_rpm != newRpm) {
            m_rpm = newRpm;
            emit rpmChanged(m_rpm);
        }
    }
}

void CommunicationManager::handleUdpAccPedalChanged(int newAccPedal)
{
    if (m_currentSource == SourceType::Udp) {
        if (m_accPedal != newAccPedal) {
            m_accPedal = newAccPedal;
            emit accPedalChanged(m_accPedal);
        }
    }
}

void CommunicationManager::handleUdpBrakePedalChanged(int newBrakePedal)
{
    if (m_currentSource == SourceType::Udp) {
        if (m_brakePedal != newBrakePedal) {
            m_brakePedal = newBrakePedal;
            emit brakePedalChanged(m_brakePedal);
        }
    }
}

void CommunicationManager::handleUdpEncoderAngleChanged(double newAngle)
{
    if (m_currentSource == SourceType::Udp) {
        if (m_encoderAngle != newAngle) {
            m_encoderAngle = newAngle;
            emit encoderAngleChanged(m_encoderAngle);
        }
    }
}

void CommunicationManager::handleUdpTemperatureChanged(float newTemperature)
{
    if (m_currentSource == SourceType::Udp) {
        if (m_temperature != newTemperature) {
            m_temperature = newTemperature;
            emit temperatureChanged(m_temperature);
        }
    }
}

void CommunicationManager::handleUdpBatteryLevelChanged(int newBatteryLevel)
{
    if (m_currentSource == SourceType::Udp) {
        if (m_batteryLevel != newBatteryLevel) {
            m_batteryLevel = newBatteryLevel;
            emit batteryLevelChanged(m_batteryLevel);
        }
    }
}

void CommunicationManager::handleUdpGpsLongitudeChanged(double newLongitude)
{
    if (m_currentSource == SourceType::Udp) {
        if (m_gpsLongitude != newLongitude) {
            m_gpsLongitude = newLongitude;
            emit gpsLongitudeChanged(m_gpsLongitude);
        }
    }
}

void CommunicationManager::handleUdpGpsLatitudeChanged(double newGpsLatitude)
{
    if (m_currentSource == SourceType::Udp) {
        if (m_gpsLatitude != newGpsLatitude) {
            m_gpsLatitude = newGpsLatitude;
            emit gpsLatitudeChanged(m_gpsLatitude);
        }
    }
}

void CommunicationManager::handleUdpSpeedFLChanged(int newSpeedFL)
{
    if (m_currentSource == SourceType::Udp) {
        if (m_speedFL != newSpeedFL) {
            m_speedFL = newSpeedFL;
            emit speedFLChanged(m_speedFL);
        }
    }
}

void CommunicationManager::handleUdpSpeedFRChanged(int newSpeedFR)
{
    if (m_currentSource == SourceType::Udp) {
        if (m_speedFR != newSpeedFR) {
            m_speedFR = newSpeedFR;
            emit speedFRChanged(m_speedFR);
        }
    }
}

void CommunicationManager::handleUdpSpeedBLChanged(int newSpeedBL)
{
    if (m_currentSource == SourceType::Udp) {
        if (m_speedBL != newSpeedBL) {
            m_speedBL = newSpeedBL;
            emit speedBLChanged(m_speedBL);
        }
    }
}

void CommunicationManager::handleUdpSpeedBRChanged(int newSpeedBR)
{
    if (m_currentSource == SourceType::Udp) {
        if (m_speedBR != newSpeedBR) {
            m_speedBR = newSpeedBR;
            emit speedBRChanged(m_speedBR);
        }
    }
}

void CommunicationManager::handleUdpLateralGChanged(double newLateralG)
{
    if (m_currentSource == SourceType::Udp) {
        if (m_lateralG != newLateralG) {
            m_lateralG = newLateralG;
            emit lateralGChanged(m_lateralG);
        }
    }
}

void CommunicationManager::handleUdpLongitudinalGChanged(double newLongitudinalG)
{
    if (m_currentSource == SourceType::Udp) {
        if (m_longitudinalG != newLongitudinalG) {
            m_longitudinalG = newLongitudinalG;
            emit longitudinalGChanged(m_longitudinalG);
        }
    }
}

void CommunicationManager::handleSerialSpeedChanged(float newSpeed)
{
    if (m_currentSource == SourceType::Serial) {
        if (m_speed != newSpeed) {
            m_speed = newSpeed;
            emit speedChanged(m_speed);
        }
    }
}

void CommunicationManager::handleSerialRpmChanged(int newRpm)
{
    if (m_currentSource == SourceType::Serial) {
        if (m_rpm != newRpm) {
            m_rpm = newRpm;
            emit rpmChanged(m_rpm);
        }
    }
}

void CommunicationManager::handleSerialAccPedalChanged(int newAccPedal)
{
    if (m_currentSource == SourceType::Serial) {
        if (m_accPedal != newAccPedal) {
            m_accPedal = newAccPedal;
            emit accPedalChanged(m_accPedal);
        }
    }
}

void CommunicationManager::handleSerialBrakePedalChanged(int newBrakePedal)
{
    if (m_currentSource == SourceType::Serial) {
        if (m_brakePedal != newBrakePedal) {
            m_brakePedal = newBrakePedal;
            emit brakePedalChanged(m_brakePedal);
        }
    }
}

void CommunicationManager::handleSerialEncoderAngleChanged(double newAngle)
{
    if (m_currentSource == SourceType::Serial) {
        if (m_encoderAngle != newAngle) {
            m_encoderAngle = newAngle;
            emit encoderAngleChanged(m_encoderAngle);
        }
    }
}

void CommunicationManager::handleSerialTemperatureChanged(float newTemperature)
{
    if (m_currentSource == SourceType::Serial) {
        if (m_temperature != newTemperature) {
            m_temperature = newTemperature;
            emit temperatureChanged(m_temperature);
        }
    }
}

void CommunicationManager::handleSerialBatteryLevelChanged(int newBatteryLevel)
{
    if (m_currentSource == SourceType::Serial) {
        if (m_batteryLevel != newBatteryLevel) {
            m_batteryLevel = newBatteryLevel;
            emit batteryLevelChanged(m_batteryLevel);
        }
    }
}

void CommunicationManager::handleSerialGpsLongitudeChanged(double newLongitude)
{
    if (m_currentSource == SourceType::Serial) {
        if (m_gpsLongitude != newLongitude) {
            m_gpsLongitude = newLongitude;
            emit gpsLongitudeChanged(m_gpsLongitude);
        }
    }
}

void CommunicationManager::handleSerialGpsLatitudeChanged(double newGpsLatitude)
{
    if (m_currentSource == SourceType::Serial) {
        if (m_gpsLatitude != newGpsLatitude) {
            m_gpsLatitude = newGpsLatitude;
            emit gpsLatitudeChanged(m_gpsLatitude);
        }
    }
}

void CommunicationManager::handleSerialSpeedFLChanged(int newSpeedFL)
{
    if (m_currentSource == SourceType::Serial) {
        if (m_speedFL != newSpeedFL) {
            m_speedFL = newSpeedFL;
            emit speedFLChanged(m_speedFL);
        }
    }
}

void CommunicationManager::handleSerialSpeedFRChanged(int newSpeedFR)
{
    if (m_currentSource == SourceType::Serial) {
        if (m_speedFR != newSpeedFR) {
            m_speedFR = newSpeedFR;
            emit speedFRChanged(m_speedFR);
        }
    }
}

void CommunicationManager::handleSerialSpeedBLChanged(int newSpeedBL)
{
    if (m_currentSource == SourceType::Serial) {
        if (m_speedBL != newSpeedBL) {
            m_speedBL = newSpeedBL;
            emit speedBLChanged(m_speedBL);
        }
    }
}

void CommunicationManager::handleSerialSpeedBRChanged(int newSpeedBR)
{
    if (m_currentSource == SourceType::Serial) {
        if (m_speedBR != newSpeedBR) {
            m_speedBR = newSpeedBR;
            emit speedBRChanged(m_speedBR);
        }
    }
}

void CommunicationManager::handleSerialLateralGChanged(double newLateralG)
{
    if (m_currentSource == SourceType::Serial) {
        if (m_lateralG != newLateralG) {
            m_lateralG = newLateralG;
            emit lateralGChanged(m_lateralG);
        }
    }
}

void CommunicationManager::handleSerialLongitudinalGChanged(double newLongitudinalG)
{
    if (m_currentSource == SourceType::Serial) {
        if (m_longitudinalG != newLongitudinalG) {
            m_longitudinalG = newLongitudinalG;
            emit longitudinalGChanged(m_longitudinalG);
        }
    }
}


