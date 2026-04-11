#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>
#include <QObject>
#include <QRemoteObjectNode>
#include <QSharedPointer>
#include <QDebug>
#include "network.h"
#include "gamepadhandler.h"
#include "videocontroller.h"
#include "controlworker.h"
#include "rep_remotevehicle_replica.h"
#include "rep_remotepipeline_replica.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Create the pipeline controller
    VideoController videoController;
    VideoPipeline *m_videoPipeline = videoController.videoPipeline();

    Network *m_network = new Network(&app);
    GamepadHandler *m_gamepadHandler = new GamepadHandler(&app);

    QObject::connect(m_network, &Network::initialConnect, m_videoPipeline, &VideoPipeline::onInitialConnect);
    QObject::connect(m_network, &Network::connectionChanged, m_videoPipeline, &VideoPipeline::onConnectionChanged);

    QRemoteObjectNode *m_remoteObjectNode = m_network->connectToJetson();

    if(!m_remoteObjectNode)
    {
        qWarning() << "ControlWorker: Connect to Jetson failed.";
        return -1;
    }

    qDebug() << "Conncted to control node";

    RemoteVehicleReplica *m_remoteVehicle = m_remoteObjectNode->acquire<RemoteVehicleReplica>("Vehicle");
    m_remoteVehicle->setParent(&app);

    if(!m_remoteVehicle->waitForSource(2000))
    {
        qWarning() << "Vehicle replica not connected to source";
    }
    else
    {
        qDebug() << "Vehicle replica connected to source";
    }

    QObject::connect(m_gamepadHandler, &GamepadHandler::gamepadStatusUpdate, m_remoteVehicle, &RemoteVehicleReplica::onGamepadUpdate);


    QRemoteObjectNode remotePipelineNode;
    // QRemoteObjectNode remoteControlNode;

    if (!remotePipelineNode.connectToNode(QUrl(QStringLiteral("tcp://10.0.0.83:65213"))))
    {
        qDebug() << "Failed to connect to nodes";
        return -1;
    }

    RemotePipelineReplica *remotePipeline = remotePipelineNode.acquire<RemotePipelineReplica>(QStringLiteral("VideoPipeline"));

    if (!remotePipeline->waitForSource(3000))
    {
        qDebug() << "Replicas not connected to source";
         return -1;
    }



    // Create thread and workers for gamepad (xBox controller) and vehicle
    // QThread *controlThread = new QThread;

    // ControlWorker *controlWorker = new ControlWorker(videoController.videoPipeline());

    // controlWorker->moveToThread(controlThread);
    // QObject::connect(controlThread, &QThread::started, controlWorker, &ControlWorker::initialize);
    // QObject::connect(controlThread, &QThread::finished, controlWorker, &QObject::deleteLater);

    // controlThread->start();


    // // Create remote app controller
    // RemoteAppController remoteAppController;

    // Start remote TCP server that handles

    // Create and load QML engine
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    // Pass reference of remoteAppController to QML front end
    engine.rootContext()->setContextProperty("remotePipeline", remotePipeline);

    // Get root window and QML video item
    QQuickWindow *window = qobject_cast<QQuickWindow *>(engine.rootObjects().first());

    QQuickItem *videoItem = window->findChild<QQuickItem *>("videoItem");

    // Give the controller access only to what it needs
    videoController.setVideoItem(videoItem);

    // Start playing when the scene graph is ready
    window->scheduleRenderJob(
        new VideoSyncStart(videoController.pipelineHandle()),
        QQuickWindow::BeforeSynchronizingStage);

    return app.exec();
}
