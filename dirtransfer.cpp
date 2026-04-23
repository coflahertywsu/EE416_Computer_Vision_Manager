#include "dirtransfer.h"

DirTransfer::DirTransfer(Network *network,QObject *parent)
    : QObject{parent}
{
    m_network = network;
    m_localRepo = QDir(LOCAL_REPO);

    connect(&m_fileSystemWatcher, &QFileSystemWatcher::directoryChanged, this, &DirTransfer::onLocalRepoChanged);
}

void DirTransfer::setDirToCopy(const QDir &dir)
{
    m_selectedDir = dir;
    m_remoteDir = QDir("//" + m_network->jetsonIp() + "/" + REMOTE_REPO + "/"  + dir.dirName());
}

void DirTransfer::setLocalRepoWatcher(bool enable)
{
    if(enable)
    {
        if(!m_fileSystemWatcher.directories().contains(m_localRepo.path()))
            m_fileSystemWatcher.addPath(m_localRepo.path());

        onLocalRepoChanged();
    }
    else
    {
        m_fileSystemWatcher.removePath(m_localRepo.path());
    }
}

bool DirTransfer::selectedDirIsValid() const
{
    QFileInfoList fileInfoList = m_selectedDir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);

    bool modelFound = false;
    bool labelsFound = false;

    for(const QFileInfo &fileInfo : fileInfoList)
    {
        QString suffix = fileInfo.suffix().toLower();

        if(suffix == "onnx" || suffix == "pt")
            modelFound = true;

        if(fileInfo.fileName() == "labels.txt")
            labelsFound = true;

        if(modelFound && labelsFound)
            return true;
    }

    return false;
}

bool DirTransfer::remoteCopyExists() const
{
    return m_remoteDir.exists();
}

bool DirTransfer::localCopyExists() const
{
    QString selectedDirName = QFileInfo(m_selectedDir.path()).fileName();

    return m_localRepoEntryList.contains(selectedDirName);
}

bool DirTransfer::transferToRemote(bool overwrite)
{
    return transferDir(m_remoteDir, overwrite);
}

bool DirTransfer::copyToLocal(bool overwrite)
{
    QDir dstDir(m_localRepo.filePath(m_selectedDir.dirName()));

    return transferDir(dstDir, overwrite);
}

QStringList DirTransfer::localRepoEntryList() const
{
    return m_localRepoEntryList;
}

void DirTransfer::onLocalRepoChanged()
{
    QStringList newList = m_localRepo.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    if (newList == m_localRepoEntryList)
        return;

    m_localRepoEntryList = newList;
    emit localRepoEntryListChanged();
}

bool DirTransfer::transferDir(QDir dst, bool overwrite)
{
    if(dst.exists())
    {
        if(!overwrite)
        {
            qWarning() << "DirTransfer: Failed to transfer, existing destination directory exists: " + dst.path();
            return false;
        }
        else
        {
            qDebug() << "DirTransfer: Overwriting existing directory: " + dst.path();

            if(!dst.removeRecursively())
            {
                qWarning() << "DirTransfer: Failed to overwrite existing destination directory " + dst.path();
                return false;
            }
        }
    }

    if(!QDir().mkpath(dst.path()))
    {
        qWarning() << "DirTransfer: Failed to create destination directory";
        return false;
    }

    QStringList internalFiles = m_selectedDir.entryList(QDir::Files | QDir::NoDotAndDotDot);

    bool result = true;

    for(const QString &file : internalFiles)
    {
        QString srcFile = m_selectedDir.filePath(file);
        QString dstFile = dst.filePath(file);

        if(!QFile::copy(srcFile, dstFile))
            result = false;
    }

    if(!result)
        qWarning() << "DirTransfer: One or more files did not copy during transfer";

    return result;
}
