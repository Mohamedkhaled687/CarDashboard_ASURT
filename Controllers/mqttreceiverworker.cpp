
#include "MqttReceiverWorker.h"
#include <QDebug>
#include <QHostInfo>
#include <QThread>

MqttReceiverWorker::MqttReceiverWorker(QObject *parent)
    : QObject(parent),
    m_client(nullptr),
    m_subscription(nullptr),
    m_useTls(false)
{
}

MqttReceiverWorker::~MqttReceiverWorker()
{
    stopReceiving();
    if (m_client) {
        m_client->deleteLater();
    }
}

void MqttReceiverWorker::initialize()
{
    // This slot is called when the worker's thread starts.
    // Any thread-specific initialization can be done here.
    qDebug() << "MqttReceiverWorker initialized in thread:" << QThread::currentThread();
}

void MqttReceiverWorker::startReceiving(const QString &brokerAddress, quint16 port, bool useTls, const QString &clientId, const QString &username, const QString &password, const QString &topic)
{
    if (m_client && m_client->state() == QMqttClient::Connected) {
        qDebug() << "MQTT client already connected.";
        return;
    }

    m_topic = topic.trimmed();
    m_useTls = useTls;

    if (!m_client) {
        m_client = new QMqttClient(this);
        connect(m_client, &QMqttClient::connected, this, &MqttReceiverWorker::onConnected);
        connect(m_client, &QMqttClient::disconnected, this, &MqttReceiverWorker::onDisconnected);
        connect(m_client, &QMqttClient::messageReceived, this, &MqttReceiverWorker::onMessageReceived);
        connect(m_client, QOverload<QMqttClient::ClientError>::of(&QMqttClient::errorChanged), this, &MqttReceiverWorker::onMqttError);
    }

    setupMqttClient(brokerAddress, port, clientId, username, password);

    if (m_useTls)
    {
        QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
        sslConfig.setPeerVerifyMode(QSslSocket::QueryPeer);
        sslConfig.setProtocol(QSsl::TlsV1_2OrLater);
        m_client->connectToHostEncrypted(sslConfig);
    }
    else
    {
        m_client->connectToHost();
    }

    qDebug() << "Attempting to connect to MQTT broker:" << brokerAddress << ":" << port << "(TLS:" << useTls << ")";
}

void MqttReceiverWorker::stopReceiving()
{
    if (m_client) {
        if (m_client->state() == QMqttClient::Connected) {
            m_client->disconnectFromHost();
            qDebug() << "Disconnected from MQTT broker.";
        }
    }
}

void MqttReceiverWorker::onConnected()
{
    qDebug() << "Connected to MQTT broker.";
    if (m_client) {
        m_subscription = m_client->subscribe(m_topic);
        if (m_subscription) {
            qDebug() << "Subscribed to topic:" << m_topic;
        } else {
            emit errorOccurred("Failed to subscribe to topic: " + m_topic);
        }
    }
}

void MqttReceiverWorker::onDisconnected()
{
    qDebug() << "Disconnected from MQTT broker.";
    emit errorOccurred("Disconnected from MQTT broker.");
}

void MqttReceiverWorker::onMessageReceived(const QByteArray &message, const QMqttTopicName &topic)
{
    Q_UNUSED(topic);
    emit messageReceived(message);
}

void MqttReceiverWorker::onMqttError(QMqttClient::ClientError error)
{
    QString errorString;
    switch (error)
    {
    case QMqttClient::NoError:
        errorString = "No error";
        break;
    case QMqttClient::InvalidProtocolVersion:
        errorString = "Invalid protocol version";
        break;
    case QMqttClient::IdRejected:
        errorString = "Client ID rejected";
        break;
    case QMqttClient::ServerUnavailable:
        errorString = "Server unavailable";
        break;
    case QMqttClient::BadUsernameOrPassword:
        errorString = "Bad username or password";
        break;
    case QMqttClient::NotAuthorized:
        errorString = "Not authorized";
        break;
    case QMqttClient::TransportInvalid:
        errorString = "Transport invalid";
        break;
    case QMqttClient::ProtocolViolation:
        errorString = "Protocol violation";
        break;
    case QMqttClient::UnknownError:
        errorString = "Unknown error";
        break;
    case QMqttClient::Mqtt5SpecificError:
        errorString = "MQTT 5 specific error";
        break;
    default:
        errorString = "Unknown MQTT error";
        break;
    }
    emit errorOccurred("MQTT Error: " + errorString);
    qDebug() << "MQTT Error:" << errorString;
}

void MqttReceiverWorker::setupMqttClient(const QString &brokerAddress, quint16 port, const QString &clientId, const QString &username, const QString &password)
{
    m_client->setHostname(brokerAddress);
    m_client->setPort(port);
    m_client->setClientId(clientId);
    m_client->setUsername(username);
    m_client->setPassword(password);

}


