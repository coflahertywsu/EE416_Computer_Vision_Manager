#include "dirtransfer.h"

DirTransfer::DirTransfer(Network *network,QObject *parent)
    : QObject{parent}
{
    m_network = network;
}

void DirTransfer::setDir(const QDir &dir)
{
    m_localDir = dir;
    m_remoteDir = QDir("//" + m_network->jetsonIp() + REMOTE_PARENT_DIR + "//"  + dir.dirName());
}

bool DirTransfer::remoteCopyExists()
{
    return m_remoteDir.exists();
}

bool DirTransfer::transfer(bool overwrite)
{
    if(m_remoteDir.exists())
    {
        if(!overwrite)
        {
            qWarning() << "DirTransfer: Failed to transfer, existing remote directory exists: " + m_remoteDir.path();
            return false;
        }
        else
        {
            qDebug() << "DirTransfer: Overwriting existing directory: " + m_remoteDir.path();

            if(!m_remoteDir.removeRecursively())
            {
                qWarning() << "DirTransfer: Failed to overwrite existing remote directory";
                return false;
            }
        }
    }

    if(!QDir().mkpath(m_remoteDir.path()))
    {
        qWarning() << "DirTransfer: Failed to create remote directory";
        return false;
    }

    QStringList localFiles = m_localDir.entryList(QDir::Files | QDir::NoDotAndDotDot);

    bool result = true;

    for(const QString &file : localFiles)
    {
        QString src = m_localDir.filePath(file);
        QString dest = m_remoteDir.filePath(file);

        if(!QFile::copy(src, dest))
            result = false;
    }

    if(!result)
        qWarning() << "DirTransfer: One or more files did not copy during transfer";

    return result;
}
