#include "gamepadhandler.h"

GamepadHandler::GamepadHandler(QObject *parent)
    : QObject{parent}
{
    m_statusTimer = new QTimer(this);           // Creating timer
    m_mgr = QGamepadManager::instance();  // Creating instance of gamepad (singleton API)

    connect(m_statusTimer, &QTimer::timeout, this, &GamepadHandler::onStatusTimer);
    connect(m_mgr, &QGamepadManager::gamepadConnected, this, &GamepadHandler::onGamepadConnected);
    connect(m_mgr, &QGamepadManager::gamepadDisconnected, this, &GamepadHandler::onGamepadDisconnected);

    // If something is already connected, attach to the first device.
    const QList<int> ids = m_mgr->connectedGamepads();
    if (!ids.isEmpty())
    {
        attachToDevice(ids.first());
    }
    else
    {
        qDebug() << "No gamepad detected yet. Plug one in / turn it on.";
    }
}

void GamepadHandler::onStatusTimer()
{
    //qDebug() << "Inside onStatusTimer slot";
    if(m_heartbeat < 255)
        m_heartbeat++;
    else
        m_heartbeat = 1;

    emit gamepadStatusUpdate(m_buttonA, m_leftAxisX, m_leftAxisY, m_heartbeat);
}

void GamepadHandler::onGamepadConnected(int deviceId)
{
    // This attaches to the most recently connected device.
    attachToDevice(deviceId);
    m_statusTimer->start(100);
    qDebug() << QString("Connected: deviceId=%1").arg(m_deviceId);
}

void GamepadHandler::onGamepadDisconnected(int deviceId)
{
    if (m_deviceId == deviceId)
        detach();
    m_statusTimer->stop();

    qDebug() << QString("Disconnected: deviceId=%1").arg(deviceId);
}

void GamepadHandler::onButtonAChanged(bool pressed)
{
    m_buttonA = (uint8_t)pressed + 1; //0 = null char, 1 = not pressed, 2 = pressed
}



void GamepadHandler::onLeftAxisXChanged(double value)
{
    m_leftAxisX = (value + 1) * 100 + 1; //val = 1 to 201
}

void GamepadHandler::onLeftAxisYChanged(double value)
{
    m_leftAxisY = (value + 1) * 100 + 1;
}

void GamepadHandler::attachToDevice(int deviceId)
{
    detach();

    m_deviceId = deviceId;
    m_pad = new QGamepad(m_deviceId, this);

    connect(m_pad, &QGamepad::buttonAChanged,
            this, &GamepadHandler::onButtonAChanged);

    connect(m_pad, &QGamepad::axisLeftXChanged,
            this, &GamepadHandler::onLeftAxisXChanged);

    connect(m_pad, &QGamepad::axisLeftYChanged,
            this, &GamepadHandler::onLeftAxisYChanged);
}

void GamepadHandler::detach()
{
    m_deviceId = -1;

    if (m_pad) {
        // Because m_pad is child of this deleteLater is safe and simple.
        m_pad->deleteLater();
        m_pad = nullptr;
    }
}
