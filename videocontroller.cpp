#include "videocontroller.h"

VideoController::VideoController()
{
    m_vidPipeline.initialize();
}

void VideoController::setVideoItem(QQuickItem *item)
{
    if(!item || !item->window())
    {
        qDebug() << "VideoController: Video item or parent window are null";
        return;
    }

    m_videoItem = item;
    m_vidPipeline.setVideoItem(item);
}

void VideoController::syncVideo()
{
    if (!m_videoItem) {
        qDebug() << "syncVideo: no video item";
        return;
    }

    QQuickWindow *window = m_videoItem->window();
    qDebug() << "syncVideo: window =" << window;

    if (!window)
        return;

    window->scheduleRenderJob(
        new VideoSyncStart(m_vidPipeline.pipelineHandle()),
        QQuickWindow::BeforeSynchronizingStage);
}

GstElement *VideoController::pipelineHandle()
{
    return m_vidPipeline.pipelineHandle();
}

VideoPipeline *VideoController::videoPipeline()
{
    return &m_vidPipeline;
}

void VideoController::startStream()
{
    m_vidPipeline.start();
}

void VideoController::stopStream()
{
    m_vidPipeline.stop();
}
