#ifndef MQTTPARSERWORKER_H
#define MQTTPARSERWORKER_H

#include <QObject>
#include <QRunnable>
#include <QByteArray>
#include <QMutex>
#include <QQueue>
#include <QWaitCondition>
#include <atomic>

/**
 * @brief The MqttParserWorker class parses MQTT messages in a thread pool
 *
 * This class is designed to run in a thread pool and efficiently parse MQTT messages
 * without blocking the main thread or the receiver thread.
 */
class MqttParserWorker : public QObject, public QRunnable
{
    Q_OBJECT

public:
    explicit MqttParserWorker(bool debugMode = false, QObject *parent = nullptr);
    ~MqttParserWorker();

    /**
     * @brief Implement QRunnable interface
     * This method will be executed in a thread pool thread
     */
    void run() override;

public slots:
    /**
     * @brief Queue a message for parsing
     * @param data The message data to parse
     */
    void queueMessage(const QByteArray &data);

    /**
     * @brief Stop the parser worker
     */
    void stop();

signals:
    /**
     * @brief Signal emitted when a message is successfully parsed
     */
    void messageParsed(float speed, int rpm, int accPedal, int brakePedal,
                       double encoderAngle, float temperature, int batteryLevel,
                       double gpsLongitude, double gpsLatitude,
                       int speedFL, int speedFR, int speedBL, int speedBR,
                       double lateralG, double longitudinalG);

    /**
     * @brief Signal emitted when an error occurs during parsing
     * @param error The error message
     */
    void errorOccurred(const QString &error);

private:
    /**
     * @brief Parse a single message
     * @param data The message data to parse
     */
    void parseMessage(const QByteArray &data);

    bool m_debugMode;
    std::atomic<bool> m_running;
    std::atomic<quint64> m_messagesParsed;

    // Thread-safe queue for messages
    QQueue<QByteArray> m_queue;
    QMutex m_queueMutex;
    QWaitCondition m_queueCondition;
};

#endif // MQTTPARSERWORKER_H


