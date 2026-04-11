#ifndef VIDEOSYNCSTART_H
#define VIDEOSYNCSTART_H

#include <QRunnable>
#include <gst/gst.h>

class VideoSyncStart : public QRunnable
{
public:
    VideoSyncStart(GstElement *pipeline);
    ~VideoSyncStart();

    void run() override;

private:
    GstElement *m_pipeline;
};

#endif // VIDEOSYNCSTART_H
