#include "cvmodelconfig.h"

#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>

CvModelConfig::CvModelConfig(Network *network, QObject *parent)
    : QObject(parent)
{
    m_network = network;
}

CvModelConfig::CvModelConfig(Network *network, const QString &modelName, QObject *parent)
    : QObject(parent)
{
    m_network = network;
    m_modelName = modelName;
    loadLabels();
}

QStringList CvModelConfig::labels() const
{
    return m_labels;
}

double CvModelConfig::preThreshold() const
{
    return m_preThreshold;
}

double CvModelConfig::nmsIouThreshold() const
{
    return m_nmsIouThreshold;
}

void CvModelConfig::setPreThreshold(double value)
{
    if (qAbs(m_preThreshold - value) < 0.001)
        return;

    m_preThreshold = value;
    emit preThresholdChanged();
}

void CvModelConfig::setNmsIouThreshold(double value)
{
    if (qAbs(m_nmsIouThreshold - value) < 0.001)
        return;

    m_nmsIouThreshold = value;
    emit nmsIouThresholdChanged();
}

QString CvModelConfig::modelName() const
{
    return m_modelName;
}

QJsonObject CvModelConfig::toJson() const
{
    QJsonObject json;

    json["modelName"] = m_modelName;
    json["preThreshold"] = m_preThreshold;
    json["nmsIouThreshold"] = m_nmsIouThreshold;

    return json;
}

void CvModelConfig::fromJson(const QJsonObject &json)
{
    if (json.contains("modelName"))
    {
        m_modelName = json["modelName"].toString();
        setPreThreshold(json["preThreshold"].toDouble(0.4));
        setNmsIouThreshold(json["nmsIouThreshold"].toDouble(0.5));

        loadLabels();
    }
}

void CvModelConfig::loadLabels()
{
    m_labels.clear();

    QFile labelFile(modelDir().filePath(m_labelsFileName));

    if (!labelFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "CvModelConfig: Failed to open labels.txt:" << labelFile.fileName();
        return;
    }

    QTextStream textStream(&labelFile);
    while (!textStream.atEnd())
    {
        QString line = textStream.readLine().trimmed();
        if (!line.isEmpty())
            m_labels.append(line);
    }

    labelFile.close();

    emit labelsChanged();
}

QDir CvModelConfig::modelDir() const
{
    if (!m_network)
        return QDir();

    return QDir("//" + m_network->jetsonIp() + "/models/" + m_modelName);
}
