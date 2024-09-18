#include "JsonConfig.h"
#include <QCoreApplication>
#include <QFileInfo>
#include <QMutex>
#include <QDateTime>
#include <QStringList>
#include <QDebug>
#include <QDir>
#include <QFileSystemWatcher>
#include <QLockFile>
#include <QJsonDocument>
#include <QJsonObject>

struct tagFileConfig{
    QString name;
    QDateTime timeStamp;
    qint64 size;
    QMutex mutex;
    //    QJsonDocument doc;
    QJsonObject data;
};

class JsonConfig::PrivateData{
public:
    bool pendingChanges;
    tagFileConfig fileConfig;
    QJsonObject curData;
    //    QJsonDocument document;
    QFileSystemWatcher systemWatcher;
};

JsonConfig::JsonConfig(const QString &fileName, QObject *parent) : QObject(parent)
{
    QString path = QFileInfo(fileName).absolutePath();
    static QDir dir;
    dir.mkpath(path );

    d = new PrivateData();
    d->fileConfig.size = 0;
    d->fileConfig.name = fileName;
    this->sync();


    connect(&d->systemWatcher, &QFileSystemWatcher::fileChanged, [=]{
        sync();
    });
    connect(&d->systemWatcher, SIGNAL(fileChanged(QString)), this, SIGNAL(fileChanged(QString)));    
    d->systemWatcher.addPath(fileName);
}


JsonConfig::~JsonConfig()
{
    if (d->pendingChanges) {
        flush();
    }
    delete d;
}

void JsonConfig::syncConfFile()
{
    bool readOnly = d->curData == d->fileConfig.data;

    if (readOnly && d->fileConfig.size > 0) {
        QFileInfo fileInfo(d->fileConfig.name);
        if (d->fileConfig.size == fileInfo.size() && d->fileConfig.timeStamp == fileInfo.lastModified())
            return;
    }

#ifndef QT_BOOTSTRAPPED
    /*
        Use a lockfile in order to protect us against other QSettings instances
        trying to write the same settings at the same time.

        We only need to lock if we are actually writing as only concurrent writes are a problem.
        Concurrent read and write are not a problem because the writing operation is atomic.
    */
    QLockFile lockFile(d->fileConfig.name + QLatin1String(".lock"));
    if (!readOnly && !lockFile.lock()) {
        // setStatus(QSettings::AccessError);
        return;
    }
#endif    

    QFileInfo fileInfo(d->fileConfig.name);
    bool mustReadFile = true;
    bool createFile = !fileInfo.exists();

    if (!readOnly)
        mustReadFile = (d->fileConfig.size != fileInfo.size()
                || (d->fileConfig.size != 0 && d->fileConfig.timeStamp != fileInfo.lastModified()));
    //读
    if (mustReadFile) {
        QFile file(d->fileConfig.name);
        if (!createFile && !file.open(QFile::ReadOnly)) {
            //            setStatus(QSettings::AccessError);
            return;
        }

        /*
              Files that we can't read (because of permissions or
              because they don't exist) are treated as empty files.
          */
        if (file.isReadable() && file.size() != 0) {

            QByteArray data = file.readAll();

            QJsonDocument doc = QJsonDocument().fromJson(data);
            if(doc.isNull()){
                doc = QJsonDocument();
            }
            d->curData = d->fileConfig.data = doc.object();

            // qDebug() << "read";
            // qDebug() << doc.toJson();

        }

        d->fileConfig.size = fileInfo.size();
        d->fileConfig.timeStamp = fileInfo.lastModified();
    }

    //写
    if (!readOnly) {
        QFile sf(d->fileConfig.name);

        if (!sf.open(QIODevice::WriteOnly)) {
            //            setStatus(QSettings::AccessError);
            return;
        }
        QJsonDocument doc;
        doc.setObject(d->curData);
        sf.write(doc.toJson());

        sf.close();

        QFileInfo fileInfo(d->fileConfig.name);
        d->fileConfig.size = fileInfo.size();
        d->fileConfig.timeStamp = fileInfo.lastModified();
        d->fileConfig.data = d->curData;

    }
}

void JsonConfig::sync()
{
    QMutexLocker locker(&d->fileConfig.mutex);
    syncConfFile();
}

void JsonConfig::flush()
{
    sync();
}

void JsonConfig::update()
{
    flush();
    d->pendingChanges = false;
}

void JsonConfig::requestUpdate()
{
    if (!d->pendingChanges) {
        d->pendingChanges = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}


bool JsonConfig::event(QEvent *event)
{
    if (event->type() == QEvent::UpdateRequest) {
        update();
        return true;
    }
    return QObject::event(event);
}

//QJsonObject createPath(const QString& path, QJsonObject parent)
//{
//    QStringList paths = path.split("/", QString::SkipEmptyParts);
//    if(paths.size() > 0){
//        QString name = paths.takeFirst();
//        if(parent.contains(name)){
//            QJsonObject obj = parent[name].toObject();
//            parent[name] = createPath(paths.join("/"), obj);
//            return parent;
//        }else{
//            QJsonObject obj;
//            parent[name] = createPath(paths.join("/"), obj);
//            return parent;
//        }

//    }
//    return QJsonObject();
//}

//QJsonObject JsonConfig::getPath(const QString path, bool bReadOnly)
//{
//    if(!bReadOnly){
//        createPath(path, d->curData);
//        requestUpdate();
//    }
//    QStringList paths = path.split("/", QString::SkipEmptyParts);
//    QJsonObject obj = d->curData;
//    foreach(QString p,  paths){
//        if(!obj.contains(p))
//            return QJsonObject();
//        obj = obj[p].toObject();
//    }
//    return obj;
//}

QJsonValue JsonConfig::getValue(const QString path, const QString& key)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QStringList paths = path.split("/", Qt::SkipEmptyParts);
#else
    QStringList paths = path.split("/", QString::SkipEmptyParts);
#endif    
    QJsonObject obj = d->curData;
    foreach(QString p,  paths){
        if(!obj.contains(p))
            return QJsonObject();
        obj = obj[p].toObject();
    }
    return obj[key];

}


QJsonObject setValueImpl(QJsonObject parent, const QString& path, const QString& key, const QJsonValue& value)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QStringList paths = path.split("/", Qt::SkipEmptyParts);
#else
    QStringList paths = path.split("/", QString::SkipEmptyParts);
#endif
    if(paths.size() > 0){
        QString name = paths.takeFirst();
        if(parent.contains(name)){
            QJsonObject obj = parent[name].toObject();
            parent[name] = setValueImpl(obj, paths.join("/"), key, value);
            return parent;
        }else{
            QJsonObject obj;
            parent[name] = setValueImpl(obj, paths.join("/"), key, value);
            return parent;
        }

    }else{
        parent[key] = value;
        return parent;
    }
    return QJsonObject();
}


void JsonConfig::setValue(const QString path, const QString& key, const QJsonValue& value)
{
    d->curData = setValueImpl(d->curData, path, key, value);

    requestUpdate();

    // qDebug() << d->curData;
}

QStringList JsonConfig::getKeys(const QString path)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QStringList paths = path.split("/", Qt::SkipEmptyParts);
#else
    QStringList paths = path.split("/", QString::SkipEmptyParts);
#endif
    QJsonObject obj = d->curData;
    foreach(QString p,  paths) {
        if(!obj.contains(p))
            return QStringList();
        obj = obj[p].toObject();
    }
    return obj.keys();
}
