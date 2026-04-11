#include "videopipeline.h"

VideoPipeline::VideoPipeline(QObject *parent) : QObject(parent)
{
    gst_init(nullptr, nullptr);
}

VideoPipeline::~VideoPipeline()
{
    if (m_pipeline)
    {
        gst_element_set_state(m_pipeline, GST_STATE_NULL);
        gst_object_unref(m_pipeline);
    }
}

bool VideoPipeline::initialize()
{
    if (!createElements())
        return false;

    if (!linkElements())
        return false;

    return true;
}

GstElement *VideoPipeline::pipelineHandle()
{
    return m_pipeline;
}

void VideoPipeline::setVideoItem(QQuickItem *item)
{
    g_object_set(m_sink, "widget", item, NULL);
}

void VideoPipeline::start()
{
    gst_element_set_state(m_pipeline, GST_STATE_PLAYING);
}

void VideoPipeline::stop()
{
    gst_element_set_state(m_pipeline, GST_STATE_NULL);
}

void VideoPipeline::onInitialConnect(const QString &ip)
{
    m_desktopIp = ip.toUtf8();

    // Set caps on udpsrc exactly as your working pipeline
    GstCaps *rtp_caps = gst_caps_new_simple("application/x-rtp",
                                            "media", G_TYPE_STRING, "video",
                                            "encoding-name", G_TYPE_STRING, "JPEG",
                                            "payload", G_TYPE_INT, 26,
                                            NULL);

    g_object_set(m_udpSrc, "address", m_desktopIp.constData(), "port", 5000, "caps", rtp_caps, NULL);
    gst_caps_unref(rtp_caps);
}

void VideoPipeline::onConnectionChanged(bool connection)
{
    if(connection && m_autoRestart)
    {
        start();
        m_autoRestart = false;
    }
    else if(!connection)
    {
        stop();
        m_autoRestart = true;
    }
}

bool VideoPipeline::createElements()
{
    m_pipeline        = gst_pipeline_new("video-pipeline");
    m_udpSrc          = gst_element_factory_make("udpsrc", "udpSrc");
    m_rtpDepay        = gst_element_factory_make("rtpjpegdepay", "rtpDepay");
    m_jpegDecoder     = gst_element_factory_make("jpegdec", "jpegDec");
    m_videoConverter  = gst_element_factory_make("videoconvert", "vidConv");
    m_queue1          = gst_element_factory_make("queue", "queue1");

    m_glUpload        = gst_element_factory_make("glupload", "glUpload");
    m_glColorConvert  = gst_element_factory_make("glcolorconvert", "glColorConv");
    m_converterCaps   = gst_element_factory_make("capsfilter", "converterCaps");
    m_queue2          = gst_element_factory_make("queue", "queue2");
    m_sink            = gst_element_factory_make("qmlglsink", "qmlSink");

    if (!m_pipeline || !m_udpSrc || !m_rtpDepay || !m_jpegDecoder || !m_videoConverter ||
        !m_queue1 || !m_glUpload || !m_glColorConvert || !m_converterCaps || !m_queue2 || !m_sink)

    {
        qWarning() << "Failed to create some GStreamer elements";
        return false;
    }

    // Set caps on udpsrc exactly as your working pipeline
    GstCaps *rtp_caps = gst_caps_new_simple("application/x-rtp",
                                            "media", G_TYPE_STRING, "video",
                                            "encoding-name", G_TYPE_STRING, "JPEG",
                                            "payload", G_TYPE_INT, 26,
                                            NULL);

    g_object_set(m_udpSrc, "address", m_desktopIp.constData(), "port", 5000, "caps", rtp_caps, NULL);
    gst_caps_unref(rtp_caps);

    // GL caps
    g_object_set(m_converterCaps, "caps",
                 gst_caps_from_string("video/x-raw(memory:GLMemory), format=RGB"),
                 NULL);

    g_object_set(m_sink, "sync", FALSE, NULL);

    gst_bin_add_many(GST_BIN(m_pipeline),
                     m_udpSrc, m_rtpDepay, m_jpegDecoder, m_videoConverter, m_queue1,
                     m_glUpload, m_glColorConvert, m_converterCaps, m_queue2, m_sink, NULL);

    return true;
}

bool VideoPipeline::linkElements()
{
    if (!gst_element_link_many(m_udpSrc, m_rtpDepay, m_jpegDecoder, m_videoConverter, m_queue1,
                               m_glUpload, m_glColorConvert, m_converterCaps, m_queue2, m_sink, NULL))

    {
        qWarning() << "Failed to link GStreamer pipeline.";
        return false;
    }

    return true;
}
