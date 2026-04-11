#ifndef VIDEOCONTROLLER_H
#define VIDEOCONTROLLER_H

#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QDebug>
#include "videopipeline.h"
#include "videosyncstart.h"

class VideoController
{
public:
    VideoController();

    void setVideoItem(QQuickItem *item);
    GstElement* pipelineHandle();
    VideoPipeline* videoPipeline();
    void startStream();
    void stopStream();
private:
    VideoPipeline m_vidPipeline;
};

#endif // VIDEOCONTROLLER_H
