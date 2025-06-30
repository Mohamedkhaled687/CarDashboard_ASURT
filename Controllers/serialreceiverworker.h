#ifndef SERIALRECEIVERWORKER_H
#define SERIALRECEIVERWORKER_H

#include <QObject>
#include <QSerialPort>
#include <QByteArray>

/**
 * @brief The SerialReceiverWorker class handles receiving data from the serial port in a separate thread.
 */
class SerialReceiverWorker : public QObject
{
    Q_OBJECT
public:
    explicit SerialReceiverWorker(QObject *parent = nullptr);
    ~SerialReceiverWorker();

public slots:
    void initialize();
    void startReceiving(const QString &portName, qint32 baudRate);
    void stopReceiving();

private slots:
    void readSerialData();
    void handleError(QSerialPort::SerialPortError serialPortError);

signals:
    void serialDataReceived(const QByteArray &data);
    void errorOccurred(const QString &error);

private:
    QSerialPort *m_serialPort;
    bool m_receiving;
};

#endif // SERIALRECEIVERWORKER_H

