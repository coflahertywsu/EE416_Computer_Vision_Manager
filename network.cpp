#include "network.h"

Network::Network(QObject *parent)
    : QObject{parent}
{
    m_remoteObjectNode = new QRemoteObjectNode(this);
    connect(&m_heartbeatTimer, &QTimer::timeout, this, &Network::onHeartbeatTimer);
    connect(&m_heartbeatTimer, &QTimer::timeout, this, &Network::connectCheck);
}

Network::~Network()
{
    emit connectionChanged(false);
}

QRemoteObjectNode* Network::connectToJetson(int timeoutMsec)
{
    if (!discoverAddresses(timeoutMsec))
        return nullptr;

    qDebug() << "Going to initialize remote objects";

    return initializeRemoteObjects();
}

QString Network::jetsonIp() const
{
    return m_jetsonIp.toString();
}

QString Network::desktopIp() const
{
    return m_desktopIp.toString();
}

void Network::onJetsonHeartbeat(quint8 val)
{
    // qDebug() << "Network: Jetosn heartbeat = " << QString::number(val);
    m_jetsonHeartbeat = val;
}

void Network::onHeartbeatTimer()
{
    // qDebug() << "Network: Desktop heartbeat = " << QString::number(m_desktopHeartbeat);
    emit desktopHeartbeat(m_desktopHeartbeat++);
}

void Network::connectCheck()
{
    if (m_jetsonHeartbeat != m_jetsonHeartbeatLast)
    {
        m_missedConsecutiveBeats = 0;
        m_jetsonHeartbeatLast = m_jetsonHeartbeat;
        return;
    }

    m_missedConsecutiveBeats++;

    if(m_missedConsecutiveBeats == MISSED_BEATS_ALLOWED && m_connection)
    {
        m_connection = false;
        emit connectionChanged(m_connection);
        qDebug() << "Network: Jetson heartbeat failed";
    }
}

bool Network::discoverAddresses(int timeoutMsec)
{
    quint64 start = QDateTime::currentMSecsSinceEpoch();
    bool result = false;

    while (QDateTime::currentMSecsSinceEpoch() - start < timeoutMsec)
    {
        QHostInfo jetsonInfo = QHostInfo::fromName("artemis-jetson");

        if (!jetsonInfo.addresses().isEmpty())
        {
            for (const QHostAddress &jetsonIp : jetsonInfo.addresses())
            {
                if (jetsonIp.protocol() == QAbstractSocket::IPv4Protocol)
                {
                    m_jetsonIp = jetsonIp;
                    break;
                }
            }

            qDebug() << "Network: artemis-jetson detected at " << m_jetsonIp.toString();

            QHostAddress netmask(SUBNET_MASK);

            const QList<QNetworkInterface> allInterfaces = QNetworkInterface::allInterfaces();

            for (const QNetworkInterface &netInterface : allInterfaces)
            {
                const QList<QNetworkAddressEntry> addressEntries = netInterface.addressEntries();

                for (const QNetworkAddressEntry &entry : addressEntries)
                {
                    QHostAddress desktopIp = entry.ip();

                    if (desktopIp.protocol() == QAbstractSocket::IPv4Protocol &&
                        desktopIp != QHostAddress::LocalHost &&
                        (desktopIp.toIPv4Address() & netmask.toIPv4Address()) ==
                            (m_jetsonIp.toIPv4Address() & netmask.toIPv4Address()))
                    {
                        m_desktopIp = desktopIp;
                        qDebug() << "Network: dekstop ip address " << m_desktopIp.toString() << " selected";
                        result = true;
                        break;
                    }
                }

                if (result)
                    break;
            }

            if (result)
                break;
        }

        QThread::msleep(250);
    }

    return result;
}


QRemoteObjectNode *Network::initializeRemoteObjects()
{
    QString url = "tcp://" + m_jetsonIp.toString() + ":" + QString::number(TCP_PORT);

    qDebug() << "Network: Attmpting to connect to remote object node at " << url;

    if (!m_remoteObjectNode->connectToNode(QUrl(url)))
    {
        qWarning() << "Failed to connect to control node";
        return nullptr;
    }

    m_jetsonNetwork = m_remoteObjectNode->acquire<JetsonNetworkReplica>("JetsonNetwork");

    if (!m_jetsonNetwork)
    {
        qWarning() << "Failed to acquire Network replica (nullptr)";
        return nullptr;
    }

    m_jetsonNetwork->setParent(this);

    if (!m_jetsonNetwork->waitForSource(2000))
    {
        qWarning() << "Network replica not connected to source";
        return nullptr;
    }
    else
    {
        qDebug() << "Network: Replica connected";
    }

    connect(this, &Network::initialConnect, m_jetsonNetwork, &JetsonNetworkReplica::onInitialConnect);
    connect(this, &Network::desktopHeartbeat, m_jetsonNetwork, &JetsonNetworkReplica::onDesktopHeartbeat);
    connect(m_jetsonNetwork, &JetsonNetworkReplica::jetsonHeartbeat, this, &Network::onJetsonHeartbeat);

    m_connection = true;
    emit initialConnect(m_desktopIp.toString());
    emit connectionChanged(m_connection);
    m_heartbeatTimer.start(DESKTOP_HEARBEAT_MSEC);

    return m_remoteObjectNode;
}

bool Network::connection() const
{
    return m_connection;
}
