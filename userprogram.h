#ifndef USERPROGRAM_H
#define USERPROGRAM_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QList>
#include <QQmlListProperty>
#include <QJsonObject>

#include "cvmodelconfig.h"
#include "instruction.h"
#include "network.h"

class UserProgram : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList cvModelNames READ cvModelNames NOTIFY cvModelNamesChanged)
    Q_PROPERTY(CvModelConfig* selectedCvModelConfig READ selectedCvModelConfig NOTIFY selectedCvModelConfigChanged)
    Q_PROPERTY(QQmlListProperty<Instruction> instructions READ instructions NOTIFY instructionsChanged)

public:
    explicit UserProgram(Network *network, QObject *parent = nullptr);

    QStringList cvModelNames() const;
    CvModelConfig* selectedCvModelConfig() const;
    QQmlListProperty<Instruction> instructions();

    QJsonObject toJson() const;
    void fromJson(const QJsonObject &json);

    Q_INVOKABLE void loadModelNames();
    Q_INVOKABLE void selectCvModelConfig(int index);

    Q_INVOKABLE Instruction* addInstruction();
    Q_INVOKABLE void removeInstruction(int index);
    Q_INVOKABLE Instruction* instructionAt(int index) const;
    Q_INVOKABLE int instructionCount() const;

    Q_INVOKABLE bool save();
    Q_INVOKABLE bool load();

signals:
    void cvModelNamesChanged();
    void selectedCvModelConfigChanged();
    void instructionsChanged();

private:
    bool modelConfigExists(const QString &modelName) const;
    QDir modelsDir() const;
    QString programFilePath() const;

    Network *m_network = nullptr;
    QStringList m_cvModelNames;
    QList<Instruction*> m_instructions;
    CvModelConfig *m_selectedCvModelConfig = nullptr;
};

#endif // USERPROGRAM_H
