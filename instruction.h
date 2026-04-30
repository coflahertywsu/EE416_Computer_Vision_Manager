#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>

class InstructionTypes : public QObject
{
    Q_OBJECT

public:
    enum InputType {
        CvObject,
        DigitalInput,
        LatchInput
    };
    Q_ENUM(InputType)

    enum DigitalInputs {
        Input1,
        Input2,
        Input3,
        Input4
    };
    Q_ENUM(DigitalInputs)

    enum OutputType {
        DigitalOutput,
        LatchOutput
    };
    Q_ENUM(OutputType)

    enum DigitalOutputs {
        Output1,
        Output2,
        Output3,
        Output4
    };
    Q_ENUM(DigitalOutputs)

    enum DigitalOutFunction {
        On,
        Pulse
    };
    Q_ENUM(DigitalOutFunction)

    enum LatchOutFunction {
        Set,
        Reset
    };
    Q_ENUM(LatchOutFunction)

    enum Latches {
        LatchA,
        LatchB,
        LatchC,
        LatchD
    };
    Q_ENUM(Latches)
};

class InputCondition : public QObject
{
    Q_OBJECT

    Q_PROPERTY(InstructionTypes::InputType inputType READ inputType WRITE setInputType NOTIFY inputTypeChanged)
    Q_PROPERTY(int inputChannel READ inputChannel WRITE setInputChannel NOTIFY inputChannelChanged)
    Q_PROPERTY(QString labelText READ labelText WRITE setLabelText NOTIFY labelTextChanged)
    Q_PROPERTY(bool activeHigh READ activeHigh WRITE setActiveHigh NOTIFY activeHighChanged)

public:
    explicit InputCondition(QObject *parent = nullptr);

    InstructionTypes::InputType inputType() const;
    int inputChannel() const;
    QString labelText() const;
    bool activeHigh() const;

    void setInputType(InstructionTypes::InputType value);
    void setInputChannel(int value);
    void setLabelText(const QString &value);
    void setActiveHigh(bool value);

    QJsonObject toJson() const;
    void fromJson(const QJsonObject &json);

signals:
    void inputTypeChanged();
    void inputChannelChanged();
    void labelTextChanged();
    void activeHighChanged();

private:
    InstructionTypes::InputType m_inputType = InstructionTypes::CvObject;
    int m_inputChannel = 0;
    QString m_labelText;
    bool m_activeHigh = true;
};

class OutputAction : public QObject
{
    Q_OBJECT

    Q_PROPERTY(InstructionTypes::OutputType outputType READ outputType WRITE setOutputType NOTIFY outputTypeChanged)
    Q_PROPERTY(int outputChannel READ outputChannel WRITE setOutputChannel NOTIFY outputChannelChanged)
    Q_PROPERTY(int outputFunction READ outputFunction WRITE setOutputFunction NOTIFY outputFunctionChanged)
    Q_PROPERTY(int pulseTimeMs READ pulseTimeMs WRITE setPulseTimeMs NOTIFY pulseTimeMsChanged)

public:
    explicit OutputAction(QObject *parent = nullptr);

    InstructionTypes::OutputType outputType() const;
    int outputChannel() const;
    int outputFunction() const;
    int pulseTimeMs() const;

    void setOutputType(InstructionTypes::OutputType value);
    void setOutputChannel(int value);
    void setOutputFunction(int value);
    void setPulseTimeMs(int value);

    QJsonObject toJson() const;
    void fromJson(const QJsonObject &json);

signals:
    void outputTypeChanged();
    void outputChannelChanged();
    void outputFunctionChanged();
    void pulseTimeMsChanged();

private:
    InstructionTypes::OutputType m_outputType = InstructionTypes::DigitalOutput;
    int m_outputChannel = 0;
    int m_outputFunction = 0;
    int m_pulseTimeMs = 0;
};

class Instruction : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(OutputAction* output READ output CONSTANT)
    Q_PROPERTY(int inputCount READ inputCount NOTIFY inputsChanged)

public:
    explicit Instruction(QObject *parent = nullptr);

    QString name() const;
    OutputAction* output() const;
    int inputCount() const;

    void setName(const QString &value);

    Q_INVOKABLE InputCondition* addInput();
    Q_INVOKABLE void removeInput(int index);
    Q_INVOKABLE InputCondition* inputAt(int index) const;
    Q_INVOKABLE void clearInputs();

    QJsonObject toJson() const;
    void fromJson(const QJsonObject &json);

signals:
    void nameChanged();
    void inputsChanged();

private:
    QString m_name = "Instruction";
    QVector<InputCondition*> m_inputs;
    OutputAction *m_output = nullptr;
};

#endif // INSTRUCTION_H
