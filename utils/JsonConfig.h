#ifndef JSONCONFIG_H
#define JSONCONFIG_H

#include <QObject>
#include <QJsonValue>


class JsonConfig : public QObject
{
    Q_OBJECT
public:
    explicit JsonConfig(const QString &fileName, QObject *parent = nullptr);
    ~JsonConfig();

    QJsonValue getValue(const QString path, const QString& key);
    void setValue(const QString path, const QString& key, const QJsonValue& value);

    QStringList getKeys(const QString path);
    
    void clear();
    void sync();
    void flush();

protected:
    bool event(QEvent *event) override;
private:
    void requestUpdate();
    void update();
    void syncConfFile();

signals:
    void fileChanged(const QString &path);
protected:
private:
    class PrivateData;
    class PrivateData* d;

};

#endif // JSONCONFIG_H
