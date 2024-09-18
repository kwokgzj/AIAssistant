#ifndef __CHATBASE_HTTP_REQUEST_H__
#define __CHATBASE_HTTP_REQUEST_H__

#include <QObject>
#include <QNetworkReply>
#include <QString>

#include <QJsonObject>
#include <QJsonDocument>
#include <QUrl>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

class ChatbaseHttpRequest : public QObject
{
	Q_OBJECT
public:
	explicit ChatbaseHttpRequest(QObject *parent = 0);
	virtual ~ChatbaseHttpRequest();

public:
    bool post(const QUrl &url, const QByteArray &query);
    void stopWaitingReply();

signals:
    void sigPostReplyQJsonObject(const QJsonObject &);
	void sigPostReplyStream(const QByteArray&, bool done);
	void sigPostReplyFailed();

public slots :
	// 一次返回全部数据
    void slotPostReplyFinished();
	// 数据流传输中
    void slotPostReplying();
	// 数据流传输完成
    void slotPostReplyStreamFinished();
	void slotPostError(QNetworkReply::NetworkError);
	void slotTimeoutWork();

private:
	QNetworkAccessManager    *m_downloadManager = nullptr;
	QNetworkReply            *m_postReply = nullptr;
	bool m_isReplying = false;
	QTimer* m_timeout{ nullptr };
	QByteArray m_lastReply;
};

#endif // __CHATBASE_HTTP_REQUEST_H__
