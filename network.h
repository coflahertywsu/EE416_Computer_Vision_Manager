#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QThread>
#include <QDateTime>
#include <QTimer>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QRemoteObjectNode>
#include <QSharedPointer>
#include <QProcess>
#include "rep_remotenetwork_replica.h"

#define TCP_PORT                65214
#define SUBNET_MASK             "255.255.255.0"         //May need to switch to /16 for bigger network (which may also accomodate smaller network most of the time)
#define DESKTOP_HEARBEAT_MSEC   500
#define DEFAULT_CONNECT_TIMEOUT 15000
#define MISSED_BEATS_ALLOWED    4

class Network : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connection READ connection  NOTIFY connectionChanged FINAL)

public:
    explicit Network(QObject *parent = nullptr);
    ~Network();

    QRemoteObjectNode* connectToJetson(int timeoutMsec = DEFAULT_CONNECT_TIMEOUT);  //Note: Blocking function
    QString jetsonIp() const;
    QString desktopIp() const;
    bool connection() const;

signals:
    void initialConnect(const QString &ip);
    void initialConnectFailed();
    void desktopHeartbeat(quint8 val);    
    void connectionChanged(bool connection);

public slots:
    void onJetsonHeartbeat(quint8 val);

private slots:
    void onHeartbeatTimer();
    void connectCheck();
    void onConnectionChanged(bool connected);

private:
    bool discoverAddresses(int timeoutMsec);
    QRemoteObjectNode* initializeRemoteObjects();
    void connectModelsShare();

    QHostAddress m_desktopIp;
    QHostAddress m_jetsonIp;
    QTimer m_heartbeatTimer;
    quint8 m_desktopHeartbeat = 0;
    quint8 m_jetsonHeartbeat = 0;
    quint8 m_jetsonHeartbeatLast = 0;
    int m_missedConsecutiveBeats = 0;
    bool m_connection = false;
    QRemoteObjectNode *m_remoteObjectNode = nullptr;
    RemoteNetworkReplica *m_jetsonNetwork = nullptr;
};

#endif // Network_H
