#ifndef VIDEOCONTROLLER_H
#define VIDEOCONTROLLER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QDebug>
#include "videopipeline.h"
#include "videosyncstart.h"

class VideoController : public QObject
{
    Q_OBJECT

public:
    VideoController();

    Q_INVOKABLE void setVideoItem(QQuickItem *item);
    Q_INVOKABLE void syncVideo();
    GstElement* pipelineHandle();
    VideoPipeline* videoPipeline();
    void startStream();
    void stopStream();
private:
    VideoPipeline m_vidPipeline;
    QQuickItem *m_videoItem;
};

#endif // VIDEOCONTROLLER_H
