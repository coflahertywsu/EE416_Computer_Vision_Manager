#ifndef CVMODELCONFIG_H
#define CVMODELCONFIG_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include "network.h"

class CvModelConfig : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList labels READ labels NOTIFY labelsChanged)
    Q_PROPERTY(double preThreshold READ preThreshold WRITE setPreThreshold NOTIFY preThresholdChanged)
    Q_PROPERTY(double nmsIouThreshold READ nmsIouThreshold WRITE setNmsIouThreshold NOTIFY nmsIouThresholdChanged)

public:
    explicit CvModelConfig(Network *network, QObject *parent = nullptr);
    explicit CvModelConfig(Network *network, const QString &modelName, QObject *parent = nullptr);

    QStringList labels() const;
    double preThreshold() const;
    double nmsIouThreshold() const;

    void setPreThreshold(double value);
    void setNmsIouThreshold(double value);

    QString modelName() const;

    QJsonObject toJson() const;
    void fromJson(const QJsonObject &json);

signals:
    void labelsChanged();
    void preThresholdChanged();
    void nmsIouThresholdChanged();

private:
    void loadLabels();
    QDir modelDir() const;

    Network *m_network = nullptr;
    QString m_modelName;
    QStringList m_labels;
    double m_preThreshold = 0.4;
    double m_nmsIouThreshold = 0.5;
    const QString m_labelsFileName = "labels.txt";
};

#endif // CVMODELCONFIG_H
