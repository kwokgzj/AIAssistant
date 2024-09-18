#include <QApplication>
#include <QThreadPool>
#include <QFile>
#include <QFontDatabase>
#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QDir>
#include <iostream>
#include "ChatDialog.h"
#include "lang/LanguageMgr.h"
#include "utils/RevoScanConfig.h"
// #include "../ui/lang/language.h"
// #include "utils/AIAssistant.pipeMsg.pb.h"

// #include "lang/LanguageMgr.h"
// #include "utils/RevoScanConfig.h"

bool runOnce()
{
    // 程序运行互斥，包括RevoScan5.0与4.0的互斥
    QString sharedMemoryKey = "Revo Scan AIAssistant";
    // 用信号量加锁，防止同时点击2次App
    static QSystemSemaphore sema("RevoScan AIAssistant single app key", 1, QSystemSemaphore::Open);
    sema.acquire();
#ifdef Q_OS_MACOS
    // 在macos下程序崩溃或强制退出，不会释放共享内存，先attach、detach一次释放上次崩溃遗留的共享内存。
    static QSharedMemory tryMem(sharedMemoryKey);
    if (tryMem.attach())
    {
        tryMem.detach();
    }
#endif
    static QSharedMemory sharedMemory(sharedMemoryKey);
    if (sharedMemory.attach())
    {
        sema.release();
        // rvInfo << "AIAssistant is already running.";
        return false;
    }
    sharedMemory.create(1);
    sema.release();
    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 以下两名字不能随意修改，会影响保存路径等内容
    QCoreApplication::setApplicationName("AIAssistant");
    // 去掉公司
    QCoreApplication::setOrganizationName("Chishine");
    if(!runOnce())
    return 0;
    // 加载语言包
    // LanguageMgr::switchLanguage(RevoScanConfig::getLanguage());

    // QFile file("D:/temp/ConversionTool/skin/conversionTool.qss");
    // QFile file(":/skin/AIAssistant.qss");

    QString qssPath = QCoreApplication::applicationDirPath() + QDir::separator() + "skin" + QDir::separator() + "AIAssistant.qss";
    std::cout << "zjGuo test fontfilePath --- " << qssPath.toStdString() << std::endl;
    QFile file(qssPath);
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    a.setStyleSheet(styleSheet);
    file.close();
    QThreadPool::globalInstance()->setMaxThreadCount(1);
    // 字体 SourceHanSansSC-Medium
    QString fontfilePath = QCoreApplication::applicationDirPath() + QDir::separator() + "res" + QDir::separator() + "font" + QDir::separator();
    std::cout << "zjGuo test fontfilePath --- " << fontfilePath.toStdString() << std::endl;
    int nIndex = QFontDatabase::addApplicationFont(fontfilePath + "SourceHanSansCN-Medium.ttf");
    if (nIndex != -1)
    {
        QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));
        if (strList.count() > 0)
        {
            QFont fontThis(strList.at(0));
            fontThis.setPixelSize(14);
            a.setFont(fontThis);
        }
    }
    QFontDatabase::addApplicationFont(fontfilePath + "SourceHanSansCN-Regular.ttf");
    QFontDatabase::addApplicationFont(fontfilePath + "HarmonyOS_Sans_Regular.ttf");
    ChatDialog w;
    w.setWindowFlag(Qt::Window);
    w.show();
    return a.exec();
    // AIAssistant::pipeMsg msg;
    // msg.set_id(101);
    // msg.set_str("hello");
}
