#include "ChatbaseHttpRequest.h"
// #include "log/RvDebug.h"

#include <thread>
#include <iostream>

#define DOWNLOAD_TIMEOUT  30000

using namespace std;

ChatbaseHttpRequest::ChatbaseHttpRequest(QObject *parent) : QObject(parent)
{
	m_downloadManager = new QNetworkAccessManager(this);
    m_timeout = new QTimer();
    m_timeout->setInterval(20000);  // 设置超时时间 20 秒
    m_timeout->setSingleShot(true); // 单次触发
}

ChatbaseHttpRequest::~ChatbaseHttpRequest()
{
    if (m_postReply) {
        m_postReply->deleteLater();
    }
	m_downloadManager->deleteLater();
}

bool ChatbaseHttpRequest::post(const QUrl &url, const QByteArray &query)
{
    // rvInfo << "zjGuo test post start";
    if (m_postReply && !m_postReply->isFinished()) {
        return false;
    }
    // rvInfo << "zjGuo test request start";

    QNetworkRequest request;
	request.setUrl(url);
    
	request.setRawHeader("Content-Type", "application/json");
	request.setRawHeader("X-chatbot", "revoscan5");
	request.setRawHeader("X-signature", "");
	request.setRawHeader("X-timestamp", time_t(NULL));
    // request.setRawHeader(QByteArray("Accept"), QByteArray("ACCEPTHEADER"));
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork); // Events shouldn't be cached
	if (m_downloadManager->networkAccessible() == QNetworkAccessManager::NotAccessible) {
		m_downloadManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
	}
    m_postReply = m_downloadManager->post(request, query);

    m_timeout->start();
    connect(m_postReply, &QNetworkReply::finished, [&]{
        m_timeout->stop();
    });
    connect(m_timeout, &QTimer::timeout, this, &ChatbaseHttpRequest::slotTimeoutWork);

	// connect(m_postReply, SIGNAL(finished()), this, SLOT(slotPostReplyFinished()));
	connect(m_postReply, SIGNAL(finished()), this, SLOT(slotPostReplyStreamFinished()));
	connect(m_postReply, &QNetworkReply::readyRead, [&]{
        m_isReplying = true;
        slotPostReplying();
    });
	connect(m_postReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotPostError(QNetworkReply::NetworkError)));
    connect(m_postReply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), this, &ChatbaseHttpRequest::slotPostError);
	return true;
}

void ChatbaseHttpRequest::slotPostReplyFinished()
{
    // rvInfo << "zjGuo test post return";
    QJsonObject result;
    int statusCode = m_postReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

	if((statusCode == 200) || (statusCode == 302))
	{
        QJsonParseError jsonError;
        QByteArray data = m_postReply->readAll();

        QJsonDocument doucment = QJsonDocument::fromJson(data, &jsonError);
        if (jsonError.error == QJsonParseError::NoError && doucment.isObject()) {
            result = doucment.object();
        }
	}else{
        emit sigPostReplyFailed();
    }

	m_postReply->deleteLater();
	m_postReply = nullptr;

    emit sigPostReplyQJsonObject(result);
}

void ChatbaseHttpRequest::slotPostReplying()
{
    // rvInfo << "zjGuo test post Replying";
    QString result;
    if(!m_postReply){
        return;
    }
    int statusCode = m_postReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if((statusCode == 200) || (statusCode == 302))
    {
        m_lastReply = m_postReply->readAll().data();
        // if(!m_lastReply.isEmpty()){
            // std::cout << "zjGuo test post Replying  ---- " << result.toStdString() << std::endl;
            emit sigPostReplyStream(m_lastReply, false);
        // }
    }else{
        QByteArray data = m_postReply->readAll();
        result = QString::fromUtf8(data);
        emit sigPostReplyFailed();
    }
}

void ChatbaseHttpRequest::slotPostReplyStreamFinished()
{
    // rvInfo << "zjGuo test post Reply finished";
    m_isReplying = false;
    // m_timer->stop();
    QString result;
    int statusCode = m_postReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

	if((statusCode == 200) || (statusCode == 302))
	{
        m_lastReply = m_postReply->readAll();
        emit sigPostReplyStream(m_lastReply, true);
	}else{
        emit sigPostReplyFailed();
    }
    // std::cout << "zjGuo test post finished  ---- " << result.toStdString() << std::endl;
    m_timeout->stop();
	m_postReply->deleteLater();
	m_postReply = nullptr;
}

void ChatbaseHttpRequest::slotPostError(QNetworkReply::NetworkError errorCode) /* handle error */
{

}

void ChatbaseHttpRequest::slotTimeoutWork(){
    if(m_postReply && m_postReply->isRunning()){
        emit sigPostReplyFailed();
    }
}

void ChatbaseHttpRequest::stopWaitingReply(){
    m_isReplying = false;
    m_timeout->stop();
	m_postReply->deleteLater();
	m_postReply = nullptr;
}
