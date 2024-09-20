#ifndef PIPEMESSAGE_H
#define PIPEMESSAGE_H
#include <QJsonObject>
#include "AIAssistant.pipeMsg.pb.h"

// 定义消息头长度为7字节
const int HEADER_LENGTH = 7;
const uint8_t PACKET_VERSION = 1;

//协议头部
#pragma pack(1)
struct Head
{
    uint8_t version; //协议版本号
    uint16_t type; //报文类型
    uint32_t length; //body长度
};

struct Init{
    Head head;
    AIAssistant::init body;
};

struct InitResponse{
    Head head;
    AIAssistant::initResponse body;
};

struct UserContext{
    Head head;
    AIAssistant::userContext body;
};

struct Command{
    Head head;
    AIAssistant::command body;
};

struct CommandResponse{
    Head head;
    AIAssistant::commandResponse body;
};

struct StatusRepost{
    Head head;
    AIAssistant::statusRepost body;
};

class PipeMessage{
public:
    PipeMessage();
    ~PipeMessage();
    QByteArray encodeInit(std::string softwareName, std::string softwareVersion, std::string robotID, std::string robotSecretKey, std::string systemVersion,
                    int softWareXPos, int softWareYPos, int softWareWidth, int softWareHeight);
    QByteArray  encodeInitResponse(bool success, std::string msg);
    QByteArray encodeUserContext(QJsonObject obj);
    QByteArray encodeCommand(QJsonObject obj);
    QByteArray encodeCommandResponse(QJsonObject obj);
    QByteArray encodeStatusRepost(QJsonObject obj);

    Init decodeInit(QByteArray& byteArray);
    InitResponse decodeInitResponse(QByteArray& byteArray);
    UserContext decodeUserContext(QByteArray& byteArray);
    Command decodeCommand(QByteArray& byteArray);
    CommandResponse decodeCommandResponse(QByteArray& byteArray);
    StatusRepost decodeStatusRepost(QByteArray& byteArray);
};
#endif // PIPEMESSAGE_H
