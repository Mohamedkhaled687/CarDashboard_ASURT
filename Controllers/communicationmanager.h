#ifndef COMMUNICATIONMANAGER_H
#define COMMUNICATIONMANAGER_H

#include <QObject>
#include <QVariant>
#include <QDebug>

// Forward declarations
class UdpClient;
class SerialManager;
class MqttClient;


class CommunicationManager : public QObject
{
    Q_OBJECT

    // Expose all properties that were previously in UdpClient and SerialManager
    Q_PROPERTY(float speed READ speed NOTIFY speedChanged)
    Q_PROPERTY(int rpm READ rpm NOTIFY rpmChanged)
    Q_PROPERTY(int accPedal READ accPedal NOTIFY accPedalChanged)
    Q_PROPERTY(int brakePedal READ brakePedal NOTIFY brakePedalChanged)
    Q_PROPERTY(double encoderAngle READ encoderAngle NOTIFY encoderAngleChanged)
    Q_PROPERTY(float temperature READ temperature NOTIFY temperatureChanged)
    Q_PROPERTY(int batteryLevel READ batteryLevel NOTIFY batteryLevelChanged)
    Q_PROPERTY(double gpsLongitude READ gpsLongitude NOTIFY gpsLongitudeChanged)
    Q_PROPERTY(double gpsLatitude READ gpsLatitude NOTIFY gpsLatitudeChanged)
    Q_PROPERTY(int speedFL READ speedFL NOTIFY speedFLChanged)
    Q_PROPERTY(int speedFR READ speedFR NOTIFY speedFRChanged)
    Q_PROPERTY(int speedBL READ speedBL NOTIFY speedBLChanged)
    Q_PROPERTY(int speedBR READ speedBR NOTIFY speedBRChanged)
    Q_PROPERTY(double lateralG READ lateralG NOTIFY lateralGChanged)
    Q_PROPERTY(double longitudinalG READ longitudinalG NOTIFY longitudinalGChanged)
    Q_PROPERTY(bool isSerialSource READ isSerialSource WRITE setIsSerialSource NOTIFY isSerialSourceChanged)

public:
    explicit CommunicationManager(QObject *parent = nullptr);
    ~CommunicationManager();

    // Property getters
    float speed() const { return m_speed; }
    int rpm() const { return m_rpm; }
    int accPedal() const { return m_accPedal; }
    int brakePedal() const { return m_brakePedal; }
    double encoderAngle() const { return m_encoderAngle; }
    float temperature() const { return m_temperature; }
    int batteryLevel() const { return m_batteryLevel; }
    double gpsLongitude() const { return m_gpsLongitude; }
    double gpsLatitude() const { return m_gpsLatitude; }
    int speedFL() const { return m_speedFL; }
    int speedFR() const { return m_speedFR; }
    int speedBL() const { return m_speedBL; }
    int speedBR() const { return m_speedBR; }
    double lateralG() const { return m_lateralG; }
    double longitudinalG() const { return m_longitudinalG; }

    Q_INVOKABLE bool startSerial(const QString &portName, qint32 baudRate);
    Q_INVOKABLE bool startUdp(quint16 port);
    Q_INVOKABLE bool startMqtt(const QString &brokerAddress, quint16 port, bool useTls, const QString &clientId, const QString &username, const QString &password, const QString &topic);
    Q_INVOKABLE bool stop();

    bool isSerialSource() const { return m_isSerialSource; }
    void setIsSerialSource(bool isSerialSource);

signals:
    void speedChanged(float newSpeed);
    void rpmChanged(int newRpm);
    void accPedalChanged(int newAccPedal);
    void brakePedalChanged(int newBrakePedal);
    void encoderAngleChanged(double newAngle);
    void temperatureChanged(float newTemperature);
    void batteryLevelChanged(int newBatteryLevel);
    void gpsLongitudeChanged(double newLongitude);
    void gpsLatitudeChanged(double newLatitude);
    void speedFLChanged(int newSpeedFL);
    void speedFRChanged(int newSpeedFR);
    void speedBLChanged(int newSpeedBL);
    void speedBRChanged(int newSpeedBR);
    void lateralGChanged(double newLateralG);
    void longitudinalGChanged(double newLongitudinalG);
    void isSerialSourceChanged(bool isSerialSource);
    void errorOccurred(const QString &error);

private slots:
    void handleUdpSpeedChanged(float newSpeed);
    void handleUdpRpmChanged(int newRpm);
    void handleUdpAccPedalChanged(int newAccPedal);
    void handleUdpBrakePedalChanged(int newBrakePedal);
    void handleUdpEncoderAngleChanged(double newAngle);
    void handleUdpTemperatureChanged(float newTemperature);
    void handleUdpBatteryLevelChanged(int newBatteryLevel);
    void handleUdpGpsLongitudeChanged(double newLongitude);
    void handleUdpGpsLatitudeChanged(double newGpsLatitude);
    void handleUdpSpeedFLChanged(int newSpeedFL);
    void handleUdpSpeedFRChanged(int newSpeedFR);
    void handleUdpSpeedBLChanged(int newSpeedBL);
    void handleUdpSpeedBRChanged(int newSpeedBR);
    void handleUdpLateralGChanged(double newLateralG);
    void handleUdpLongitudinalGChanged(double newLongitudinalG);
    void handleUdpError(const QString &error);

    void handleSerialSpeedChanged(float newSpeed);
    void handleSerialRpmChanged(int newRpm);
    void handleSerialAccPedalChanged(int newAccPedal);
    void handleSerialBrakePedalChanged(int newBrakePedal);
    void handleSerialEncoderAngleChanged(double newAngle);
    void handleSerialTemperatureChanged(float newTemperature);
    void handleSerialBatteryLevelChanged(int newBatteryLevel);
    void handleSerialGpsLongitudeChanged(double newLongitude);
    void handleSerialGpsLatitudeChanged(double newGpsLatitude);
    void handleSerialSpeedFLChanged(int newSpeedFL);
    void handleSerialSpeedFRChanged(int newSpeedFR);
    void handleSerialSpeedBLChanged(int newSpeedBL);
    void handleSerialSpeedBRChanged(int newSpeedBR);
    void handleSerialLateralGChanged(double newLateralG);
    void handleSerialLongitudinalGChanged(double newLongitudinalG);
    void handleSerialError(const QString &error);


    void handleMqttSpeedChanged(float newSpeed);
    void handleMqttRpmChanged(int newRpm);
    void handleMqttAccPedalChanged(int newAccPedal);
    void handleMqttBrakePedalChanged(int newBrakePedal);
    void handleMqttEncoderAngleChanged(double newAngle);
    void handleMqttTemperatureChanged(float newTemperature);
    void handleMqttBatteryLevelChanged(int newBatteryLevel);
    void handleMqttGpsLongitudeChanged(double newLongitude);
    void handleMqttGpsLatitudeChanged(double newGpsLatitude);
    void handleMqttSpeedFLChanged(int newSpeedFL);
    void handleMqttSpeedFRChanged(int newSpeedFR);
    void handleMqttSpeedBLChanged(int newSpeedBL);
    void handleMqttSpeedBRChanged(int newSpeedBR);
    void handleMqttLateralGChanged(double newLateralG);
    void handleMqttLongitudinalGChanged(double newLongitudinalG);
    void handleMqttError(const QString &error);

private:


    UdpClient *m_udpClient;
    SerialManager *m_serialManager;
    MqttClient *m_mqttClient;


    enum class SourceType { None, Serial, Udp, Mqtt };
    SourceType m_currentSource;

    // Internal storage for properties
    float m_speed;
    int m_rpm;
    int m_accPedal;
    int m_brakePedal;
    double m_encoderAngle;
    float m_temperature;
    int m_batteryLevel;
    double m_gpsLongitude;
    double m_gpsLatitude;
    int m_speedFL;
    int m_speedFR;
    int m_speedBL;
    int m_speedBR;
    double m_lateralG;
    double m_longitudinalG;
    bool m_isSerialSource;

};

#endif // COMMUNICATIONMANAGER_H

