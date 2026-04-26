#ifndef DIRTRANSFER_H
#define DIRTRANSFER_H

#include <QObject>
#include <QDir>
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QFile>
#include <QDebug>
#include "network.h"

#define LOCAL_REPO "./models"
#define REMOTE_REPO "models"

class DirTransfer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList localRepoEntryList READ localRepoEntryList NOTIFY localRepoEntryListChanged FINAL)

public:
    explicit DirTransfer(Network *network, QObject *parent = nullptr);
    Q_INVOKABLE void setDirToCopy(const QString &dirPath);
    Q_INVOKABLE void setDirToCopy(int localRepoIdx);
    Q_INVOKABLE void setLocalRepoWatcher(bool enable);
    Q_INVOKABLE bool selectedDirIsValid() const;                //checks for model file and labels.txt file within selected dir
    Q_INVOKABLE bool remoteCopyExists() const;
    Q_INVOKABLE bool localRepoCopyExists() const;                   //doesn't make sense when selected dir is in local repo
    Q_INVOKABLE bool transferToRemote(bool overwrite = false);  //selected dir goes to Jetson
    Q_INVOKABLE bool copyToLocalRepo(bool overwrite = false);       //Selected dir goes to local repo, doesn't make sense when selected dir is in local repo
    QStringList localRepoEntryList() const;

signals:
    void localRepoEntryListChanged();

private slots:
    void onLocalRepoChanged();

private:
    bool transferDir(QDir dst, bool overwrite);

    Network *m_network = nullptr;
    QDir m_localRepo;
    QDir m_selectedDir;
    QDir m_remoteDir;
    QFileSystemWatcher m_fileSystemWatcher;
    QStringList m_localRepoEntryList;
};

#endif // DIRTRANSFER_H
