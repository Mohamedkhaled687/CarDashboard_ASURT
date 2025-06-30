#ifndef MQTTRECEIVERWORKER_H
#define MQTTRECEIVERWORKER_H

#include <QObject>
#include <QtMqtt/QMqttClient>
#include <QtMqtt/QMqttSubscription>
#include <QSslConfiguration>



/**
 * @brief The MqttReceiverWorker class handles MQTT message reception in a dedicated thread
 *
 * This class is designed to run in its own thread and efficiently receive MQTT messages
 * without blocking the main thread or other processing threads.
 */


class MqttReceiverWorker : public QObject
{
    Q_OBJECT
public:
    explicit MqttReceiverWorker(QObject *parent = nullptr);
    ~MqttReceiverWorker();

public slots:

    /**
     * @brief Initialize the worker
     * Called when the thread starts
     */

    void initialize();

    /**
     * @brief Start receiving messages on the specified topic
     * @param brokerAddress The MQTT broker address
     * @param port The MQTT broker port
     * @param useTls Whether to use TLS
     * @param clientId The MQTT client ID
     * @param username The MQTT username
     * @param password The MQTT password
     * @param topic The MQTT topic to subscribe to
     */

    void startReceiving(const QString &brokerAddress, quint16 port, bool useTls, const QString &clientId, const QString &username, const QString &password, const QString &topic);

    /**
     * @brief Stop receiving messages
     */

    void stopReceiving();


signals:
    /**
     * @brief Signal emitted when a message is received
     * @param data The message data
     */
    void messageReceived(const QByteArray &message);

    /**
     * @brief Signal emitted when an error occurs
     * @param error The error message
     */
    void errorOccurred(const QString &error);

private slots:

    /**
     * @brief Handle MQTT client connected signal
     */
    void onConnected();
    /**
     * @brief Handle MQTT client disconnected signal
     */
    void onDisconnected();
    /**
     * @brief Process received MQTT messages
     * @param message The received message payload
     * @param topic The topic the message was received on
     */
    void onMessageReceived(const QByteArray &message, const QMqttTopicName &topic);

    /**
     * @brief Handle MQTT client errors
     * @param error The MQTT error code
     */
    void onMqttError(QMqttClient::ClientError error);

private:
    QMqttClient *m_client;
    QMqttSubscription *m_subscription;
    QString m_topic;
    bool m_useTls;

    void setupMqttClient(const QString &brokerAddress, quint16 port, const QString &clientId, const QString &username, const QString &password);
};

#endif // MQTTRECEIVERWORKER_H


