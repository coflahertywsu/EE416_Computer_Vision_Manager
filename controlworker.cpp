#include "controlworker.h"

ControlWorker::ControlWorker(VideoPipeline *pipeline, QObject *parent)
    : QObject{parent}
{
    m_videoPipeline = pipeline;
}

ControlWorker::~ControlWorker()
{
}

void ControlWorker::initialize()
{
    m_network = new Network(this);
    m_gamepadHandler = new GamepadHandler(this);

    connect(m_network, &Network::initialConnect, m_videoPipeline, &VideoPipeline::onInitialConnect);
    connect(m_network, &Network::connectionChanged, m_videoPipeline, &VideoPipeline::onConnectionChanged);

    m_remoteControlNode = m_network->connectToJetson();

    if(!m_remoteControlNode)
    {
        qWarning() << "ControlWorker: Connect to Jetson failed.";
        return;
    }

    qDebug() << "Conncted to control node";

    m_remoteVehicle = m_remoteControlNode->acquire<RemoteVehicleReplica>("Vehicle");

    if(!m_remoteVehicle->waitForSource(2000))
    {
        qWarning() << "Vehicle replica not connected to source";
    }
    else
    {
        qDebug() << "Vehicle replica connected to source";
    }

    connect(m_gamepadHandler, &GamepadHandler::gamepadStatusUpdate, m_remoteVehicle, &RemoteVehicleReplica::onGamepadUpdate);
}
