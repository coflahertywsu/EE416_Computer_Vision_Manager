#include "instruction.h"

// ================================================================
// InputCondition
// ================================================================

InputCondition::InputCondition(QObject *parent)
    : QObject(parent)
{
}

InstructionTypes::InputType InputCondition::inputType() const
{
    return m_inputType;
}

int InputCondition::inputChannel() const
{
    return m_inputChannel;
}

QString InputCondition::labelText() const
{
    return m_labelText;
}

bool InputCondition::activeHigh() const
{
    return m_activeHigh;
}

void InputCondition::setInputType(InstructionTypes::InputType value)
{
    if (m_inputType == value)
        return;

    m_inputType = value;
    emit inputTypeChanged();
}

void InputCondition::setInputChannel(int value)
{
    if (m_inputChannel == value)
        return;

    m_inputChannel = value;
    emit inputChannelChanged();
}

void InputCondition::setLabelText(const QString &value)
{
    if (m_labelText == value)
        return;

    m_labelText = value;
    emit labelTextChanged();
}

void InputCondition::setActiveHigh(bool value)
{
    if (m_activeHigh == value)
        return;

    m_activeHigh = value;
    emit activeHighChanged();
}

QJsonObject InputCondition::toJson() const
{
    QJsonObject json;

    json["inputType"] = static_cast<int>(m_inputType);
    json["inputChannel"] = m_inputChannel;
    json["labelText"] = m_labelText;
    json["activeHigh"] = m_activeHigh;

    return json;
}

void InputCondition::fromJson(const QJsonObject &json)
{
    if (json.contains("inputType"))
        setInputType(static_cast<InstructionTypes::InputType>(json["inputType"].toInt()));

    if (json.contains("inputChannel"))
        setInputChannel(json["inputChannel"].toInt());

    if (json.contains("labelText"))
        setLabelText(json["labelText"].toString());

    if (json.contains("activeHigh"))
        setActiveHigh(json["activeHigh"].toBool());
}

// ================================================================
// OutputAction
// ================================================================

OutputAction::OutputAction(QObject *parent)
    : QObject(parent)
{
}

InstructionTypes::OutputType OutputAction::outputType() const
{
    return m_outputType;
}

int OutputAction::outputChannel() const
{
    return m_outputChannel;
}

int OutputAction::outputFunction() const
{
    return m_outputFunction;
}

int OutputAction::pulseTimeMs() const
{
    return m_pulseTimeMs;
}

void OutputAction::setOutputType(InstructionTypes::OutputType value)
{
    if (m_outputType == value)
        return;

    m_outputType = value;
    emit outputTypeChanged();
}

void OutputAction::setOutputChannel(int value)
{
    if (m_outputChannel == value)
        return;

    m_outputChannel = value;
    emit outputChannelChanged();
}

void OutputAction::setOutputFunction(int value)
{
    if (m_outputFunction == value)
        return;

    m_outputFunction = value;
    emit outputFunctionChanged();
}

void OutputAction::setPulseTimeMs(int value)
{
    if (m_pulseTimeMs == value)
        return;

    m_pulseTimeMs = value;
    emit pulseTimeMsChanged();
}

QJsonObject OutputAction::toJson() const
{
    QJsonObject json;

    json["outputType"] = static_cast<int>(m_outputType);
    json["outputChannel"] = m_outputChannel;
    json["outputFunction"] = m_outputFunction;
    json["pulseTimeMs"] = m_pulseTimeMs;

    return json;
}

void OutputAction::fromJson(const QJsonObject &json)
{
    if (json.contains("outputType"))
        setOutputType(static_cast<InstructionTypes::OutputType>(json["outputType"].toInt()));

    if (json.contains("outputChannel"))
        setOutputChannel(json["outputChannel"].toInt());

    if (json.contains("outputFunction"))
        setOutputFunction(json["outputFunction"].toInt());

    if (json.contains("pulseTimeMs"))
        setPulseTimeMs(json["pulseTimeMs"].toInt());
}

// ================================================================
// Instruction
// ================================================================

Instruction::Instruction(QObject *parent)
    : QObject(parent)
{
    m_output = new OutputAction(this);
}

QString Instruction::name() const
{
    return m_name;
}

OutputAction* Instruction::output() const
{
    return m_output;
}

int Instruction::inputCount() const
{
    return m_inputs.size();
}

void Instruction::setName(const QString &value)
{
    if (m_name == value)
        return;

    m_name = value;
    emit nameChanged();
}

InputCondition* Instruction::addInput()
{
    InputCondition *input = new InputCondition(this);
    m_inputs.append(input);

    emit inputsChanged();

    return input;
}

void Instruction::removeInput(int index)
{
    if (index < 0 || index >= m_inputs.size())
        return;

    InputCondition *input = m_inputs.takeAt(index);
    input->deleteLater();

    emit inputsChanged();
}

InputCondition* Instruction::inputAt(int index) const
{
    if (index < 0 || index >= m_inputs.size())
        return nullptr;

    return m_inputs.at(index);
}

void Instruction::clearInputs()
{
    for (InputCondition *input : m_inputs)
        input->deleteLater();

    m_inputs.clear();

    emit inputsChanged();
}

QJsonObject Instruction::toJson() const
{
    QJsonObject json;

    json["name"] = m_name;

    QJsonArray inputsArray;
    for (InputCondition *input : m_inputs)
    {
        if (input)
            inputsArray.append(input->toJson());
    }

    json["inputs"] = inputsArray;

    if (m_output)
        json["output"] = m_output->toJson();

    return json;
}

void Instruction::fromJson(const QJsonObject &json)
{
    if (json.contains("name"))
        setName(json["name"].toString());

    if (json.contains("inputs") && json["inputs"].isArray())
    {
        clearInputs();

        QJsonArray inputsArray = json["inputs"].toArray();

        for (const QJsonValue &value : inputsArray)
        {
            InputCondition *input = addInput();
            input->fromJson(value.toObject());
        }
    }

    if (json.contains("output") && json["output"].isObject() && m_output)
        m_output->fromJson(json["output"].toObject());
}
