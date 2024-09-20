#ifndef PIPEMANAGER_H
#define PIPEMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#ifdef WIN32
#include <Windows.h>
#else
#endif
#include "pipeMessage.h"

class PipeManager : public QObject
{
	Q_OBJECT
public:
	explicit PipeManager(QString pipeName, QObject *parent = 0);
	virtual ~PipeManager();

// public:
//     bool sendMessage(std::vector<QJsonObject> messageVec);
// private:
//     void startReceive();
private:
	#ifdef WIN32
    HANDLE m_sendPipe;
    HANDLE m_receivePipe;
    #else
    #endif
    // PipeMessage* m_msgManager;
};

#endif // PIPEMANAGER_H
