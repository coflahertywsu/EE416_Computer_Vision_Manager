#include "videosyncstart.h"

VideoSyncStart::VideoSyncStart(GstElement *pipeline)
{
    m_pipeline = pipeline ? GST_ELEMENT(gst_object_ref(pipeline)) : nullptr;
}

VideoSyncStart::~VideoSyncStart()
{
    if (m_pipeline)
        gst_object_unref(m_pipeline);
}

void VideoSyncStart::run()
{
    if (m_pipeline)
        gst_element_set_state(m_pipeline, GST_STATE_PLAYING);
}
