#ifndef LANGUAGEMGR_H
#define LANGUAGEMGR_H

#include <QMap>
#include <QString>
#include <QCoreApplication>
#include <QDir>



class LanguageMgr
{
public:
    LanguageMgr();

    static QMap<QString, QString> getLanguage(const QString& path=QCoreApplication::applicationDirPath() + QDir::separator() +"lang", const QString & prefix="RevoScan");
    static void switchLanguage(const QString& locale, const QString & prefix="RevoScan", const QString & path=QCoreApplication::applicationDirPath() + QDir::separator() +"lang");
    static QString getCurrentPrefix(); 
    static QString getCurrentLanguageName();

private:
    static QString g_languageCurrentLocale;
//    static void retranslateUi(class QObject* parent);
};

#endif // LANGUAGEMGR_H
