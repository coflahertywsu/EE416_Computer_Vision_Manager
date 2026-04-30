#include "userprogram.h"

#include <QJsonArray>
#include <QJsonValue>
#include <QJsonDocument>
#include <QDir>
#include <QFile>
#include <QDebug>

UserProgram::UserProgram(Network *network, QObject *parent)
    : QObject(parent)
{
    m_network = network;

    loadModelNames();
}

QStringList UserProgram::cvModelNames() const
{
    return m_cvModelNames;
}

CvModelConfig* UserProgram::selectedCvModelConfig() const
{
    return m_selectedCvModelConfig;
}

QQmlListProperty<Instruction> UserProgram::instructions()
{
    return QQmlListProperty<Instruction>(this, &m_instructions);
}

QJsonObject UserProgram::toJson() const
{
    QJsonObject json;

    if (m_selectedCvModelConfig)
        json["selectedCvModel"] = m_selectedCvModelConfig->toJson();

    QJsonArray instructionsArray;
    for (Instruction *instruction : m_instructions)
    {
        if (instruction)
            instructionsArray.append(instruction->toJson());
    }

    json["instructions"] = instructionsArray;

    return json;
}

void UserProgram::fromJson(const QJsonObject &json)
{
    if (json.contains("selectedCvModel") && json["selectedCvModel"].isObject())
    {
        if (m_selectedCvModelConfig)
            m_selectedCvModelConfig->deleteLater();

        m_selectedCvModelConfig = new CvModelConfig(m_network, this);
        m_selectedCvModelConfig->fromJson(json["selectedCvModel"].toObject());

        emit selectedCvModelConfigChanged();
    }

    for (Instruction *instruction : m_instructions)
    {
        instruction->deleteLater();
    }

    m_instructions.clear();

    if (json.contains("instructions") && json["instructions"].isArray())
    {
        QJsonArray instructionsArray = json["instructions"].toArray();

        for (const QJsonValue &value : instructionsArray)
        {
            Instruction *instruction = new Instruction(this);
            instruction->fromJson(value.toObject());
            m_instructions.append(instruction);
        }
    }

    emit instructionsChanged();

    loadModelNames();

    if (m_selectedCvModelConfig && !modelConfigExists(m_selectedCvModelConfig->modelName()))
        qWarning() << "UserProgram: selected model was not found.";
}

void UserProgram::loadModelNames()
{
    m_cvModelNames.clear();

    QDir parentModelsDir = modelsDir();

    QStringList modelNames = parentModelsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QString &modelName : modelNames)
    {
        QString modelPath = parentModelsDir.filePath(modelName);

        bool hasEngine = QFile::exists(QDir(modelPath).filePath("model.engine"));
        bool hasLabels = QFile::exists(QDir(modelPath).filePath("labels.txt"));

        if (hasEngine && hasLabels)
            m_cvModelNames.append(modelName);
    }

    emit cvModelNamesChanged();
}

void UserProgram::selectCvModelConfig(int index)
{
    if (index < 0 || index >= m_cvModelNames.size())
        return;

    if (m_selectedCvModelConfig)
        m_selectedCvModelConfig->deleteLater();

    m_selectedCvModelConfig = new CvModelConfig(m_network, m_cvModelNames.at(index), this);

    emit selectedCvModelConfigChanged();
}

Instruction* UserProgram::addInstruction()
{
    Instruction *instruction = new Instruction(this);
    instruction->setName("Instruction " + QString::number(m_instructions.size() + 1));

    instruction->addInput();   // guarantee at least one input

    m_instructions.append(instruction);
    emit instructionsChanged();

    return instruction;
}

void UserProgram::removeInstruction(int index)
{
    if (index < 0 || index >= m_instructions.size())
        return;

    Instruction *instruction = m_instructions.takeAt(index);
    instruction->deleteLater();

    emit instructionsChanged();
}

Instruction* UserProgram::instructionAt(int index) const
{
    if (index < 0 || index >= m_instructions.size())
        return nullptr;

    return m_instructions.at(index);
}

int UserProgram::instructionCount() const
{
    return m_instructions.size();
}

bool UserProgram::save()
{
    QFile file(programFilePath());

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        return false;

    QJsonDocument doc(toJson());
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}

bool UserProgram::load()
{
    QFile file(programFilePath());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (!doc.isObject())
        return false;

    fromJson(doc.object());

    return true;
}

bool UserProgram::modelConfigExists(const QString &modelName) const
{
    return m_cvModelNames.contains(modelName);
}

QDir UserProgram::modelsDir() const
{
    return QDir("//" + m_network->jetsonIp() + "/models");
}

QString UserProgram::programFilePath() const
{
    return QDir("//" + m_network->jetsonIp() + "/profiles")
    .filePath("program.json");
}
