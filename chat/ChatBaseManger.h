#ifndef __CHATBASE_MANAGER_H__
#define __CHATBASE_MANAGER_H__

#include <QObject>
#include <QUrl>
#include <QJsonObject>
#include <QJsonArray>
#define CHAT_MESSAGE_VERSION "1.0"
class ChatbaseHttpRequest;

class ChatbaseManager : public QObject
{
	Q_OBJECT
public:
	explicit ChatbaseManager(QObject *parent = 0);
	virtual ~ChatbaseManager();

public:
    bool sendMessage(std::vector<QJsonObject> messageVec);
    bool sendMessage1(QJsonObject obj);
    bool post(const QUrl &url, const QByteArray &query);
	QString getLastErrorMessage();
    void stopWaitingReply();
    void pageInfoChanged(QJsonObject obj);
private:
    std::vector<QString> splitJson(const QString& s);
signals:
    void sigPostReply(QString reply, bool done);
    void sigPostReplyFailed();
public slots:
    void slotPostReply(const QJsonObject &versionInfo);
    void slotPostReplyStream(const QByteArray &, bool done);
private:
	ChatbaseHttpRequest *m_httpRequest = nullptr;
    QString m_conversationId;
    QJsonObject m_userContext;  // userContext 用户上下文
    QJsonArray m_chatContext;   // chatContext 会话上下文
};

#endif // __CHATBASE_MANAGER_H__
