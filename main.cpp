#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QRemoteObjectNode>
#include <QDebug>
#include "network.h"
#include "dirtransfer.h"
#include "gamepadhandler.h"
#include "videocontroller.h"
#include "userprogram.h"
#include "cvmodelconfig.h"
#include "instruction.h"
#include "rep_remotevehicle_replica.h"
#include "rep_remotepipeline_replica.h"
#include "rep_remotecvmodel_replica.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // ------------------------------------------------------------
    // Local controllers
    // ------------------------------------------------------------
    VideoController videoController;
    VideoPipeline *videoPipeline = videoController.videoPipeline();

    Network *network = new Network(&app);
    GamepadHandler *gamepadHandler = new GamepadHandler(&app);

    QObject::connect(network, &Network::initialConnect, videoPipeline, &VideoPipeline::onInitialConnect);
    QObject::connect(network, &Network::connectionChanged, videoPipeline, &VideoPipeline::onConnectionChanged);

    // ------------------------------------------------------------
    // Connect to Jetson control node
    // ------------------------------------------------------------
    QRemoteObjectNode *remoteObjectNode = network->connectToJetson();

    if (!remoteObjectNode)
    {
        qWarning() << "Main: Connect to Jetson failed.";
        return -1;
    }

    qDebug() << "Connected to control node";

    // ------------------------------------------------------------
    // Vehicle replica
    // ------------------------------------------------------------
    RemoteVehicleReplica *remoteVehicle = remoteObjectNode->acquire<RemoteVehicleReplica>("Vehicle");
    remoteVehicle->setParent(&app);

    if (!remoteVehicle->waitForSource(2000))
    {
        qWarning() << "Vehicle replica not connected to source";
    }
    else
    {
        qDebug() << "Vehicle replica connected to source";
    }

    QObject::connect(gamepadHandler, &GamepadHandler::gamepadStatusUpdate, remoteVehicle, &RemoteVehicleReplica::onGamepadUpdate);

    // ------------------------------------------------------------
    // CvModel replica
    // ------------------------------------------------------------
    RemoteCvModelReplica *remoteCvModel = remoteObjectNode->acquire<RemoteCvModelReplica>("CvModel");

    if (!remoteCvModel->waitForSource(2000))
    {
        qWarning() << "CvModel replica not connected to source";
    }
    else
    {
        qDebug() << "CvModel replica connected to source";
    }

    // ------------------------------------------------------------
    // Pipeline replica
    // ------------------------------------------------------------
    QRemoteObjectNode remotePipelineNode;

    if (!remotePipelineNode.connectToNode(QUrl(QStringLiteral("tcp://10.0.0.83:65213"))))
    {
        qWarning() << "Failed to connect to pipeline node";
        return -1;
    }

    RemotePipelineReplica *remotePipeline =
        remotePipelineNode.acquire<RemotePipelineReplica>(QStringLiteral("VideoPipeline"));

    if (!remotePipeline->waitForSource(3000))
    {
        qWarning() << "Pipeline replica not connected to source";
        return -1;
    }

    // ------------------------------------------------------------
    // File transfer
    // ------------------------------------------------------------
    DirTransfer dirTransfer(network);

    // ------------------------------------------------------------
    // User program
    // ------------------------------------------------------------
    UserProgram userProgram(network);

    // ------------------------------------------------------------
    // QML
    // ------------------------------------------------------------
    QQmlApplicationEngine engine;

    qmlRegisterUncreatableType<CvModelConfig>("App", 1, 0, "CvModelConfig", "Dynamic allocation in UserProgram");
    qmlRegisterUncreatableType<Instruction>("App", 1, 0, "Instruction", "Dynamic allocation in UserProgram");
    qmlRegisterUncreatableType<InputCondition>("App", 1, 0, "InputCondition", "Dynamic allocation in UserProgram");
    qmlRegisterUncreatableType<OutputAction>("App", 1, 0, "OutputAction", "Dynamic allocation in UserProgram");
    qmlRegisterUncreatableType<InstructionTypes>("App", 1, 0, "InstructionTypes", "Enums only");

    engine.rootContext()->setContextProperty("remotePipeline", remotePipeline);
    engine.rootContext()->setContextProperty("videoController", &videoController);
    engine.rootContext()->setContextProperty("dirTransfer", &dirTransfer);
    engine.rootContext()->setContextProperty("remoteCvModel", remoteCvModel);
    engine.rootContext()->setContextProperty("userProgram", &userProgram);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}




























// #include <QGuiApplication>
// #include <QQmlApplicationEngine>
// #include <QQmlContext>
// #include <QThread>
// #include <QObject>
// #include <QRemoteObjectNode>
// #include <QSharedPointer>
// #include <QDebug>
// #include "network.h"
// #include "dirtransfer.h"
// #include "gamepadhandler.h"
// #include "videocontroller.h"
// #include "controlworker.h"
// #include "rep_remotevehicle_replica.h"
// #include "rep_remotepipeline_replica.h"
// #include "rep_remotecvmodel_replica.h"

// int main(int argc, char *argv[])
// {
//     QGuiApplication app(argc, argv);

//     // Create the pipeline controller
//     VideoController videoController;
//     VideoPipeline *m_videoPipeline = videoController.videoPipeline();

//     Network *m_network = new Network(&app);
//     GamepadHandler *m_gamepadHandler = new GamepadHandler(&app);

//     QObject::connect(m_network, &Network::initialConnect, m_videoPipeline, &VideoPipeline::onInitialConnect);
//     QObject::connect(m_network, &Network::connectionChanged, m_videoPipeline, &VideoPipeline::onConnectionChanged);

//     QRemoteObjectNode *m_remoteObjectNode = m_network->connectToJetson();

//     if(!m_remoteObjectNode)
//     {
//         qWarning() << "ControlWorker: Connect to Jetson failed.";
//         return -1;
//     }

//     qDebug() << "Conncted to control node";

//     RemoteVehicleReplica *m_remoteVehicle = m_remoteObjectNode->acquire<RemoteVehicleReplica>("Vehicle");
//     m_remoteVehicle->setParent(&app);

//     if(!m_remoteVehicle->waitForSource(2000))
//     {
//         qWarning() << "Vehicle replica not connected to source";
//     }
//     else
//     {
//         qDebug() << "Vehicle replica connected to source";
//     }

//     QObject::connect(m_gamepadHandler, &GamepadHandler::gamepadStatusUpdate, m_remoteVehicle, &RemoteVehicleReplica::onGamepadUpdate);


//     QRemoteObjectNode remotePipelineNode;
//     // QRemoteObjectNode remoteControlNode;

//     if (!remotePipelineNode.connectToNode(QUrl(QStringLiteral("tcp://10.0.0.83:65213"))))
//     {
//         qDebug() << "Failed to connect to nodes";
//         return -1;
//     }

//     RemotePipelineReplica *remotePipeline = remotePipelineNode.acquire<RemotePipelineReplica>(QStringLiteral("VideoPipeline"));

//     if (!remotePipeline->waitForSource(3000))
//     {
//         qDebug() << "Replicas not connected to source";
//          return -1;
//     }

//     DirTransfer dirTransfer(m_network);

//     // Create and load QML engine
//     QQmlApplicationEngine engine;

//     // Pass reference of remoteAppController to QML front end
//     engine.rootContext()->setContextProperty("remotePipeline", remotePipeline);
//     engine.rootContext()->setContextProperty("videoController", &videoController);
//     engine.rootContext()->setContextProperty("dirTransfer", &dirTransfer);

//     engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
//     if (engine.rootObjects().isEmpty())
//         return -1;

//     return app.exec();
// }
