#include "pipeMessage.h"
#include <QByteArray>

PipeMessage::PipeMessage(){

}

PipeMessage::~PipeMessage(){

}

QByteArray PipeMessage::encodeInit(std::string softwareName, std::string softwareVersion, std::string robotID, std::string robotSecretKey, std::string systemVersion,
                    int softWareXPos, int softWareYPos, int softWareWidth, int softWareHeight){
    Head head;
    head.version = PACKET_VERSION;
    head.type = 1;
    AIAssistant::init body;
    body.set_softwarename(softwareName);
    body.set_softwareversion(softwareVersion);
    body.set_robotid(robotID);
    body.set_robotsecretkey(robotSecretKey);
    body.set_systemversion(systemVersion);
    body.set_softwarexpos(softWareXPos);
    body.set_softwareypos(softWareYPos);
    body.set_softwarewidth(softWareWidth);
    body.set_softwareheight(softWareHeight);
    std::string str;
    body.SerializePartialToString(&str);
    QByteArray byteArray;
    byteArray.append((char*)&head, sizeof(head));
    QByteArray byteArray1 = byteArray + str.c_str();
    return byteArray1;
}

QByteArray PipeMessage::encodeInitResponse(bool success, std::string msg){
    Head head;
    head.version = PACKET_VERSION;
    head.type = 1;
    head.type |= (1 << 15);
    AIAssistant::initResponse body;
    body.set_success(success);
    body.set_responsemsg(msg);
    head.length = body.ByteSize();
    std::string str;
    body.SerializePartialToString(&str);
    QByteArray byteArray;
    byteArray.append((char*)&head, sizeof(head));
    QByteArray byteArray1 = byteArray + str.c_str();
    return byteArray1;
}

QByteArray PipeMessage::encodeUserContext(QJsonObject obj){
    Head head;
    head.version = PACKET_VERSION;
    head.type = 2;
    AIAssistant::userContext body;
    QJsonObject::Iterator it;
    auto map = body.context();
    for(it = obj.begin(); it != obj.end(); it++)
    {
        if (it.key() == "deviceType") {
            map.insert(std::make_pair("deviceType", it.value().toString().toStdString()));
        } else if (it.key() == "currentPage") {
            map.insert(std::make_pair("currentPage", it.value().toString().toStdString()));
        } else if (it.key() == "scanMode") {
            map.insert(std::make_pair("scanMode", it.value().toString().toStdString()));
        } else if (it.key() == "scanAccuracy") {
            map.insert(std::make_pair("scanAccuracy", it.value().toString().toStdString()));
        } else if (it.key() == "trackingMode") {
            map.insert(std::make_pair("trackingMode", it.value().toString().toStdString()));
        } else if (it.key() == "scanObject") {
            map.insert(std::make_pair("scanObject", it.value().toString().toStdString()));
        } else if (it.key() == "textureMapping") {
            map.insert(std::make_pair("textureMapping", it.value().toString().toStdString()));
        } else if (it.key() == "baseRemoval") {
            map.insert(std::make_pair("baseRemoval", it.value().toString().toStdString()));
        } else if (it.key() == "baseRemovalRate") {
            map.insert(std::make_pair("baseRemovalRate", it.value().toString().toStdString()));
        } else if (it.key() == "scanningDistance") {
            map.insert(std::make_pair("scanningDistance", it.value().toString().toStdString()));
        } else if (it.key() == "pointCloudFusingMethod") {
            map.insert(std::make_pair("pointCloudFusingMethod", it.value().toString().toStdString()));
        } else if (it.key() == "pointCloudFusionDistance") {
            map.insert(std::make_pair("pointCloudFusionDistance", it.value().toString().toStdString()));
        } else if (it.key() == "pointCloudIsolationRate") {
            map.insert(std::make_pair("pointCloudIsolationRate", it.value().toString().toStdString()));
        } else if (it.key() == "pointCloudOverlapDetectionDistance") {
            map.insert(std::make_pair("pointCloudOverlapDetectionDistance", it.value().toString().toStdString()));
        } else if (it.key() == "pointCloudSmoothFactor") {
            map.insert(std::make_pair("pointCloudSmoothFactor", it.value().toString().toStdString()));
        } else if (it.key() == "pointCloudSmoothtimes") {
            map.insert(std::make_pair("pointCloudSmoothtimes", it.value().toString().toStdString()));
        } else if (it.key() == "pointCloudDownsamplingType") {
            map.insert(std::make_pair("pointCloudDownsamplingType", it.value().toString().toStdString()));
        } else if (it.key() == "pointCloudSimplifyRate") {
            map.insert(std::make_pair("pointCloudSimplifyRate", it.value().toString().toStdString()));
        } else if (it.key() == "meshingQuality") {
            map.insert(std::make_pair("meshingQuality", it.value().toString().toStdString()));
        } else if (it.key() == "meshingHoleFillingAuto") {
            map.insert(std::make_pair("meshingHoleFillingAuto", it.value().toString().toStdString()));
        } else if (it.key() == "meshingHoleFillingRate") {
            map.insert(std::make_pair("meshingHoleFillingRate", it.value().toString().toStdString()));
        } else if (it.key() == "meshIsolationRate") {
            map.insert(std::make_pair("meshIsolationRate", it.value().toString().toStdString()));
        } else if (it.key() == "meshHoleFillingMethods") {
            map.insert(std::make_pair("meshHoleFillingMethods", it.value().toString().toStdString()));
        } else if (it.key() == "meshSmoothFactor") {
            map.insert(std::make_pair("meshSmoothFactor", it.value().toString().toStdString()));
        } else if (it.key() == "meshSmoothtimes") {
            map.insert(std::make_pair("meshSmoothtimes", it.value().toString().toStdString()));
        } else if (it.key() == "meshSimplifyRate") {
            map.insert(std::make_pair("meshSimplifyRate", it.value().toString().toStdString()));
        } else if (it.key() == "textureType") {
            map.insert(std::make_pair("textureType", it.value().toString().toStdString()));
        } else if (it.key() == "modelMergeType") {
            map.insert(std::make_pair("modelMergeType", it.value().toString().toStdString()));
        } else if (it.key() == "renderingState") {
            map.insert(std::make_pair("renderingState", it.value().toString().toStdString()));
        } else if (it.key() == "frames") {
            map.insert(std::make_pair("frames", it.value().toString().toStdString()));
        } else if (it.key() == "points") {
            map.insert(std::make_pair("points", it.value().toString().toStdString()));
        } else if (it.key() == "vertices") {
            map.insert(std::make_pair("vertices", it.value().toString().toStdString()));
        } else if (it.key() == "polygons") {
            map.insert(std::make_pair("polygons", it.value().toString().toStdString()));
        } else if (it.key() == "modelType") {
            map.insert(std::make_pair("modelType", it.value().toString().toStdString()));
        }
    }
    std::string str;
    body.SerializePartialToString(&str);
    QByteArray byteArray;
    byteArray.append((char*)&head, sizeof(head));
    QByteArray byteArray1 = byteArray + str.c_str();
    return byteArray1;
}

QByteArray PipeMessage::encodeCommand(QJsonObject obj){
    QByteArray byteArray;
    return byteArray;
}

QByteArray PipeMessage::encodeCommandResponse(QJsonObject obj){
    QByteArray byteArray;
    return byteArray;
}

QByteArray PipeMessage::encodeStatusRepost(QJsonObject obj){
    QByteArray byteArray;
    return byteArray;
}

Init PipeMessage::decodeInit(QByteArray& byteArray){
    Init init;
    return init;
}

InitResponse PipeMessage::decodeInitResponse(QByteArray& byteArray){
    Head head;
    memcpy(&head, byteArray.data(), sizeof(Head));
    AIAssistant::initResponse body;
    QByteArray bodyData = byteArray.mid(sizeof(Head), head.length);
    if (!body.ParseFromArray(bodyData.data(), bodyData.size())) {
        AIAssistant::initResponse();
    }
    InitResponse initResponse;
    initResponse.head = head;
    initResponse.body = body;
    return initResponse;
}

UserContext PipeMessage::decodeUserContext(QByteArray& byteArray){
    Head head;
    memcpy(&head, byteArray.data(), sizeof(Head));
    AIAssistant::userContext body;
    QByteArray bodyData = byteArray.mid(sizeof(Head), head.length);
    if (!body.ParseFromArray(bodyData.data(), bodyData.size())) {
        AIAssistant::userContext();
    }
    UserContext userContext;
    userContext.head = head;
    userContext.body = body;
    return userContext;
}

Command PipeMessage::decodeCommand(QByteArray& byteArray){
    Head head;
    memcpy(&head, byteArray.data(), sizeof(Head));
    AIAssistant::command body;
    QByteArray bodyData = byteArray.mid(sizeof(Head), head.length);
    if (!body.ParseFromArray(bodyData.data(), bodyData.size())) {
        AIAssistant::command();
    }
    Command command;
    command.head = head;
    command.body = body;
    return command;
}

CommandResponse PipeMessage::decodeCommandResponse(QByteArray& byteArray){
    Head head;
    memcpy(&head, byteArray.data(), sizeof(Head));
    AIAssistant::commandResponse body;
    QByteArray bodyData = byteArray.mid(sizeof(Head), head.length);
    if (!body.ParseFromArray(bodyData.data(), bodyData.size())) {
        AIAssistant::commandResponse();
    }
    CommandResponse commandResponse;
    commandResponse.head = head;
    commandResponse.body = body;
    return commandResponse;
}

StatusRepost PipeMessage::decodeStatusRepost(QByteArray& byteArray){
    Head head;
    memcpy(&head, byteArray.data(), sizeof(Head));
    AIAssistant::statusRepost body;
    QByteArray bodyData = byteArray.mid(sizeof(Head), head.length);
    if (!body.ParseFromArray(bodyData.data(), bodyData.size())) {
        AIAssistant::statusRepost();
    }
    StatusRepost statusRepost;
    statusRepost.head = head;
    statusRepost.body = body;
    return statusRepost;
}