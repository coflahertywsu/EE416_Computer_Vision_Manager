#include "videocontroller.h"

VideoController::VideoController()
{
    m_vidPipeline.initialize();
}

void VideoController::setVideoItem(QQuickItem *item)
{
    m_vidPipeline.setVideoItem(item);
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
