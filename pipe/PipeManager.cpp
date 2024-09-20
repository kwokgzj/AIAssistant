#include "PipeManager.h"
#include <thread>

#define BUF_SIZE 4096
#define EXAMP_PIPE_SEND   "\\\\.\\PIPE\\revoscanSend"
#define EXAMP_PIPE_RECEIVE   "\\\\.\\PIPE\\revoscanReceive"

PipeManager::PipeManager(QString pipeName, QObject *parent) : QObject(parent){
    
    // std::string strA = (EXAMP_PIPE + pipeName + "send").toStdString();
    // std::string strB = (EXAMP_PIPE + pipeName + "receive").toStdString();
    // const char* charA = strA.c_str();
    // const char* charB = strA.c_str();
    // size_t lenA = std::mbstowcs(nullptr, charA, 0) + 1;
    // size_t lenB = std::mbstowcs(nullptr, charB, 0) + 1;
    // wchar_t* pipeNameA = new wchar_t[lenA];
    // wchar_t* pipeNameB = new wchar_t[lenB];
    // std::mbstowcs(pipeNameA, charA, lenA);
    // std::mbstowcs(pipeNameB, charB, lenB);

#ifdef WIN32
    m_sendPipe = NULL;
    char  szBuffer[BUF_SIZE] = { 0 };
    DWORD dwReturn = 0;
    // 打开可用的命名管道 , 并与服务器端进程进行通信  
    m_sendPipe = CreateNamedPipe(
        EXAMP_PIPE_SEND,
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE |
        PIPE_READMODE_MESSAGE |
        PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES,
        BUF_SIZE,
        BUF_SIZE,
        0,
        NULL);
    if (m_sendPipe == INVALID_HANDLE_VALUE)
    {
        std::cout << "zjGuo test Open sendPipe Error" << std::endl;
    }
    std::cout << "zjGuo test sendPipe Wait for send message" << std::endl;

    // 开启接收管道
    std::thread([&](){
        // 创建命名管道
    m_receivePipe = NULL;
    if (!WaitNamedPipe(EXAMP_PIPE_RECEIVE, NMPWAIT_USE_DEFAULT_WAIT))
    {
        std::cout << "zjGuo test No receivePipe Accessible" << std::endl;
    }
    // 打开可用的命名管道 , 并与服务器端进程进行通信  
    m_receivePipe = CreateFile(EXAMP_PIPE_RECEIVE, GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_EXISTING, 0, NULL);
    if (m_receivePipe == INVALID_HANDLE_VALUE)
    {
        std::cout << "zjGuo test Open receivePipe Error" << std::endl;
    }
    std::cout << "zjGuo test receivePipe Wait for message" << std::endl;

    while(true){
        memset(szBuffer, 0, BUF_SIZE);
        if (ReadFile(m_receivePipe, szBuffer, BUF_SIZE, &dwReturn, NULL))
        {
            szBuffer[dwReturn] = '\0';
            std::cout << "zjGuo test server read size --- " << strlen(szBuffer) << std::endl;
            std::cout << "zjGuo test server read --- " << szBuffer << std::endl;
        }else{
            std::cout << "zjGuo test server Read Failed" << std::endl;
        }
    }
    }).detach();

#endif
}

PipeManager::~PipeManager(){
    CloseHandle(m_sendPipe);
    CloseHandle(m_receivePipe);
}