#include "udpparserworker.h"
#include <QDebug>
#include <QStringList>
#include <QThread>

/*A worker class responsible for parsing raw datagram data. It is designed to run in a thread pool,
 *  using a thread-safe queue (internally) to receive datagrams, parse them into numeric values,
 *   and then emit a signal with the parsed values or an error if parsing fails.
 */

UdpParserWorker::UdpParserWorker(bool debugMode, QObject *parent)
    : QObject(parent),
    m_debugMode(debugMode),
    m_running(true),
    m_datagramsParsed(0)
{
    setAutoDelete(false);
}

UdpParserWorker::~UdpParserWorker()
{
    stop();

    // Clear the queue
    QMutexLocker locker(&m_queueMutex);
    m_queue.clear();
}

void UdpParserWorker::run()
{
    if (m_debugMode)
    {
        qDebug() << "Parser worker started in thread" << QThread::currentThreadId();
    }

    QByteArray datagram;

    while (m_running.load())
    { // The flag is accessed using load() to ensure that changes made to it in other threads are observed safely.
        // Get a datagram from the queue
        {
            QMutexLocker locker(&m_queueMutex); // Acquires m_queueMutex to protect concurrent access to the internal datagram queue (m_queue).

            // Wait for data if queue is empty
            while (m_queue.isEmpty() && m_running.load())
            {
                // checks if the queue is empty. If it is, it waits (up to 100 milliseconds) for new data to arrive.
                m_queueCondition.wait(&m_queueMutex, 100);

                /* If another thread enqueues data during the waiting period,
                the waiting thread will observe the new state after the wait and process the queued data accordingly.*/

                /* If Thread B is still working and holding the mutex when the timeout in Thread A expires,
                 *  Thread A will be blocked at the point of re-locking until Thread B unlocks the mutex.
                 *  Once Thread B finishes and unlocks, Thread A will acquire the lock.
                 */

                /*After reacquiring the lock, Thread A resumes execution and checks the condition (e.g., whether there is data in the queue).
                 *  Any modifications made by Thread B while Thread A was waiting will be visible
                 *  because the mutex ensures that shared memory access is serialized and consistent.
                 */
            }

            // Check if we should exit
            if (!m_running.load())
            {
                break;
            }

            // Get the next datagram
            if (!m_queue.isEmpty())
            {
                datagram = m_queue.dequeue();
            }
            else
            {
                continue;
            }
        }

        // Parse the datagram
        parseDatagram(datagram);
    }

    if (m_debugMode)
    {
        qDebug() << "Parser worker stopped in thread" << QThread::currentThreadId();
    }
}

void UdpParserWorker::queueDatagram(const QByteArray &data)
{
    QMutexLocker locker(&m_queueMutex);

    // Add datagram to queue
    m_queue.enqueue(data);

    // Wake up the worker thread
    m_queueCondition.wakeOne();
}

void UdpParserWorker::stop()
{
    m_running.store(false);

    // Wake up the worker thread
    QMutexLocker locker(&m_queueMutex);
    m_queueCondition.wakeAll(); // wake up any thread that might be blocked waiting on the condition variable
}

void UdpParserWorker::parseDatagram(const QByteArray &data)
{
    try
    {
        // Convert datagram data to string and split by comma
        QString dataStr = QString::fromUtf8(data).trimmed();
        QStringList parts = dataStr.split(",");

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

                // Increment counter
                m_datagramsParsed++;

                // Emit signal with parsed data
                emit datagramParsed(
                    speed, rpm, accPedal, brakePedal,
                    encoderAngle, temperature, batteryLevel,
                    gpsLongitude, gpsLatitude,
                    speedFL, speedFR, speedBL, speedBR,
                    lateralG, longitudinalG);

                // Log debug info occasionally
                if (m_debugMode && m_datagramsParsed % 1000 == 0)
                {
                    qDebug() << "Parser" << QThread::currentThreadId()
                    << "has processed" << m_datagramsParsed << "datagrams";
                }
            }
            else
            {
                emit errorOccurred("Failed to parse some values in datagram");
            }
        }
        else
        {
            emit errorOccurred(QString("Datagram has incorrect format (expected 15+ parts, got %1)").arg(parts.size()));
        }
    }
    catch (const std::exception &e)
    {
        emit errorOccurred(QString("Exception during parsing: %1").arg(e.what()));
    }
    catch (...)
    {
        emit errorOccurred("Unknown exception during parsing");
    }
}
