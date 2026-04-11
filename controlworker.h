#ifndef CONTROLWORKER_H
#define CONTROLWORKER_H

#include <QObject>
#include <QScopedPointer>
#include <QDebug>
#include <QRemoteObjectHost>
#include <QRemoteObjectNode>
#include <QSharedPointer>
#include "gamepadhandler.h"
#include "videopipeline.h"
#include "network.h"
#include "rep_remotevehicle_replica.h"



class ControlWorker : public QObject
{
    Q_OBJECT

public:
    explicit ControlWorker(VideoPipeline *pipeline, QObject *parent = nullptr);
    ~ControlWorker();

public slots:
    void initialize();

private:
    VideoPipeline *m_videoPipeline = nullptr;
    GamepadHandler *m_gamepadHandler = nullptr;
    Network *m_network = nullptr;
    QRemoteObjectNode *m_remoteControlNode = nullptr;
    RemoteVehicleReplica *m_remoteVehicle = nullptr;
};

#endif // CONTROLWORKER_H



