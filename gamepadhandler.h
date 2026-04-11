#ifndef GAMEPADHANDLER_H
#define GAMEPADHANDLER_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QGamepad>
#include <QGamepadManager>


class GamepadHandler : public QObject
{
    Q_OBJECT
public:
    explicit GamepadHandler(QObject *parent = nullptr);

private slots:
    void onStatusTimer();
    void onGamepadConnected(int deviceId);
    void onGamepadDisconnected(int deviceId);

    void onButtonAChanged(bool pressed);
    void onLeftAxisXChanged(double value);
    void onLeftAxisYChanged(double value);

private:
    void attachToDevice(int deviceId);
    void detach();

    QTimer *m_statusTimer = nullptr;
    QGamepadManager *m_mgr = nullptr;  // not owned (Qt manages it)
    QGamepad *m_pad = nullptr;         // owned by this QObject via parent
    int m_deviceId = -1;
    quint8 m_buttonA = 0;
    quint8 m_leftAxisX = 0;
    quint8 m_leftAxisY = 0;
    quint8 m_heartbeat = 1;

signals:
    void gamepadStatusUpdate(quint8 buttonA, quint8 leftAxisX, quint8 leftAxisY, quint8 timeStamp);

};

#endif // GAMEPADHANDLER_H
