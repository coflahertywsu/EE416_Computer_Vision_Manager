#ifndef VIDEOPIPELINE_H
#define VIDEOPIPELINE_H

#include <QObject>
#include <gst/gst.h>
#include <QQuickItem>
#include <QDebug>

#define HOST_IP "10.42.0.205"                       //"10.0.0.164"

class VideoPipeline: public QObject
{
    Q_OBJECT
public:
    VideoPipeline(QObject *parent = nullptr);
    ~VideoPipeline();
    bool initialize();
    GstElement* pipelineHandle();
    void setVideoItem(QQuickItem *item);

public slots:
    void start();
    void stop();
    void onInitialConnect(const QString &ip);
    void onConnectionChanged(bool connection);

private:
    bool createElements();
    bool linkElements();

    GstElement *m_pipeline = nullptr;
    GstElement *m_udpSrc = nullptr;
    GstElement *m_rtpDepay = nullptr;
    GstElement *m_jpegDecoder = nullptr;
    GstElement *m_videoConverter = nullptr;
    GstElement *m_queue1 = nullptr;

    // GL stages and sink
    GstElement *m_glUpload = nullptr;
    GstElement *m_glColorConvert = nullptr;
    GstElement *m_converterCaps = nullptr;
    GstElement *m_queue2 = nullptr;
    GstElement *m_sink = nullptr;

    QByteArray m_desktopIp = "";
    bool m_autoRestart = false;
};

#endif // VIDEOPIPELINE_H
