#ifndef DIRTRANSFER_H
#define DIRTRANSFER_H

#include <QObject>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QDebug>
#include "network.h"

#define LOCAL_PARENT_DIR  ".//models"
#define REMOTE_PARENT_DIR "//srv//models"

class DirTransfer : public QObject
{
    Q_OBJECT
public:
    explicit DirTransfer(Network *network, QObject *parent = nullptr);
    Q_INVOKABLE void setDir(const QDir &dir);
    Q_INVOKABLE bool remoteCopyExists();
    Q_INVOKABLE bool transfer(bool overwrite = false);

signals:

private:
    Network *m_network = nullptr;
    QDir m_localDir;
    QDir m_remoteDir;
};

#endif // DIRTRANSFER_H
