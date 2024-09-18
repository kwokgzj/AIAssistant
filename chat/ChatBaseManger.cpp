#include "ChatBaseManger.h"
#include "ChatbaseHttpRequest.h"

#include <thread>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QUuid>
#include <iostream>

// #define CHATBASE_API_URL "https://test-admin.chishine3d.com/ai-api/v1/chat”
// #define CHATBASE_CHATBOTID "WwRjeXR36tPjAcpWDULpA”
ChatbaseManager::ChatbaseManager(QObject *parent) :
QObject(parent)
{
	m_httpRequest = new ChatbaseHttpRequest(this);
    QUuid id = QUuid::createUuid();
    QChar startDelimiter = '{';
    QChar endDelimiter = '}';
    QString uuidStr = id.toString();
    int startIndex = uuidStr.indexOf(startDelimiter);
    int endIndex = uuidStr.indexOf(endDelimiter, startIndex + 1);
    if (startIndex != -1 && endIndex != -1 && endIndex > startIndex) {
        int contentStartIndex = startIndex + 1;
        m_conversationId = uuidStr.mid(contentStartIndex, endIndex - contentStartIndex);
    }else{
        m_conversationId = uuidStr;
    }
	connect(m_httpRequest, &ChatbaseHttpRequest::sigPostReplyQJsonObject, this, &ChatbaseManager::slotPostReply);
	connect(m_httpRequest, &ChatbaseHttpRequest::sigPostReplyStream, this, &ChatbaseManager::slotPostReplyStream);
	// connect(m_httpRequest, &ChatbaseHttpRequest::sigPostReplyFailed, this, &ChatbaseManager::sigPostReplyFailed);
	connect(m_httpRequest, SIGNAL(sigPostReplyFailed()), SIGNAL(sigPostReplyFailed()));

}

ChatbaseManager::~ChatbaseManager()
{
	if (m_httpRequest) {
		delete m_httpRequest;
	}
}

bool ChatbaseManager::sendMessage(std::vector<QJsonObject> messageVec){
    // 创建消息数组
    QJsonArray messagesArray;
    // 创建消息对象
    for(int i = 0; i < messageVec.size(); i++){
        QJsonObject userMessage;
        userMessage.insert("content", messageVec[i].value("context"));
        userMessage.insert("role", messageVec[i].value("user"));
        messagesArray.append(userMessage);
    }

    QJsonObject json;
    json.insert("messages", messagesArray); 
    json.insert("chatbotId", "WwRjeXR36tPjAcpWDULpA");
    json.insert("stream", true);
    json.insert("temperature", 0.0);
    json.insert("model", "gpt-4o");
    json.insert("conversationId", m_conversationId);

    QJsonDocument document;
    document.setObject(json);
    QByteArray dataArray = document.toJson(QJsonDocument::Compact);

    // return post(QUrl("https://test-admin.chishine3d.com/ai-api/v1/chat"), dataArray);
    // return post(QUrl("https://test-admin.chishine3d.com/assistant-api/v1/chat"), dataArray);
    return post(QUrl("https://test-admin.chishine3d.com/assistant-api/v1/chat2"), dataArray);
}

bool ChatbaseManager::sendMessage1(QJsonObject obj){
    QJsonObject json;
    json.insert("version", CHAT_MESSAGE_VERSION); 
    json.insert("conversationId", m_conversationId);
    // 消息
    QJsonArray messagesArray;
    QString message;
    // 创建消息对象
    QJsonObject userMessage;
    QJsonObject msgContent;
    msgContent.insert("text", obj.value("context"));
    userMessage.insert("msgType", "textElem");
    userMessage.insert("msgContent", msgContent);
    messagesArray.append(userMessage);

    json.insert("message", messagesArray); 
    json.insert("chatContext", m_chatContext);
    json.insert("userContext", m_userContext);

    QJsonDocument document;
    document.setObject(json);
    QByteArray dataArray = document.toJson(QJsonDocument::Compact);

    // 消息的文本加入会话上下文
    QJsonObject obj1;
    obj1.insert("content", message);
    obj1.insert("role", "user");
    m_chatContext.append(obj1);

    return post(QUrl("http://test-admin.chishine3d.com/assistant-api/v1/chat"), dataArray);
    // return post(QUrl("http://192.168.10.153:10280/assistant-api/v1/chat"), dataArray);
}

bool ChatbaseManager::post(const QUrl &url, const QByteArray &query)
{
	return m_httpRequest->post(url, query);
}

QString ChatbaseManager::getLastErrorMessage()
{
	return "";	
}

void ChatbaseManager::slotPostReply(const QJsonObject &versionInfo)
{
    if (!versionInfo.contains("text")) {
        return;
    }
    QString text = versionInfo["text"].toString();
    emit sigPostReply(text, true);
}

void ChatbaseManager::stopWaitingReply(){
    m_httpRequest->stopWaitingReply();
}

void ChatbaseManager::pageInfoChanged(QJsonObject obj){
    // std::cout << "zjGuo test userContext {" << std::endl;
    for (QJsonObject::iterator iter = obj.begin(); iter != obj.end(); ++iter){
        QString key = iter.key();
        QJsonValue value = iter.value();
        if(value.isString()){
            // std::cout << "zjGuo test   " << key.toStdString() << " : " << value.toString().toStdString() << std::endl;
        }
        else if(value.isDouble()){
            // std::cout << "zjGuo test   " << key.toStdString() << " : " << value.toDouble() << std::endl;
        }
        else if(value.isBool()){
            // std::cout << "zjGuo test   " << key.toStdString() << " : " << value.toBool() << std::endl;
        }
        else if(value.isUndefined()){
            // std::cout << "zjGuo test   " << key.toStdString() << " : " << value.toInt() << std::endl;
        }
    }
    // std::cout << "zjGuo test    }" << std::endl;
    m_userContext = obj;
}

void ChatbaseManager::slotPostReplyStream(const QByteArray &reply, bool done){
    QString str = QString::fromUtf8(reply); 
    std::vector<QString> messages = splitJson(str);
    QString msg;
    for (const QString& message : messages) {
        QJsonParseError jsonParseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(message.toUtf8().data(), &jsonParseError);
        if(jsonDoc.isNull()){
            qDebug() << "zjGuo test post Replying error result.toLocal8Bit ---- " << str;
            continue;
        }
        QJsonObject obj = jsonDoc.object();
        if(obj.value("errCode") != 0){
            emit sigPostReplyFailed();
            return;
        }
        QJsonArray message = obj.value("message").toArray();
        QString text;
        for(QJsonArray::iterator it=message.begin();it!=message.end();++it){
            QJsonValue jvalue = *it;
            QJsonObject msgElem = jvalue.toObject();
            text += msgElem.value("msgContent").toObject().value("text").toString();
        }
        msg.push_back(text);
    }
    msg.replace("\\n", "\n");
    emit sigPostReply(msg, done);
}

std::vector<QString> ChatbaseManager::splitJson(const QString& s) {
    std::vector<QString> result;
    std::vector<QChar> stk;
    QString currentJson;

    for (const QChar c : s) {
        if (c == '{') {
            if (stk.empty() && !currentJson.isEmpty()) {
                result.push_back(currentJson);
                currentJson.clear();
            }
            stk.push_back(c);
        } else if (!stk.empty() && c == '}') {
            stk.pop_back();
            if (stk.empty()) {
                currentJson += c;
                result.push_back(currentJson);
                currentJson.clear();
            }
        }
        currentJson += c;
    }
    // 处理边缘情况
    if (!currentJson.isEmpty()) {
        result.push_back(currentJson);
    }
    return result;
}