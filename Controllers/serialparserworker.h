#ifndef SERIALPARSERWORKER_H
#define SERIALPARSERWORKER_H

#include <QObject>
#include <QByteArray>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <QRunnable>

/**
 * @brief The SerialParserWorker class parses raw serial data in a separate thread.
 */
class SerialParserWorker : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit SerialParserWorker(bool debugMode = false, QObject *parent = nullptr);
    ~SerialParserWorker();

    void queueData(const QByteArray &data);
    void stop();

protected:
    void run() override;

signals:
    void dataParsed(float speed, int rpm, int accPedal, int brakePedal,
                    double encoderAngle, float temperature, int batteryLevel,
                    double gpsLongitude, double gpsLatitude,
                    int speedFL, int speedFR, int speedBL, int speedBR,
                    double lateralG, double longitudinalG);
    void errorOccurred(const QString &error);

private:
    QQueue<QByteArray> m_dataQueue;
    QMutex m_mutex;
    QWaitCondition m_waitCondition;
    bool m_running;
    bool m_debugMode;

    void parseData(const QByteArray &data);
};

#endif // SERIALPARSERWORKER_H

