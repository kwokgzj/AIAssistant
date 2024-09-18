#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include <QLineEdit>
// #include "ChatMessage.h"
#include <QWebEngineView>
#include <QMessageBox>
#include <QStackedWidget>
#include <QJsonObject>
#include <QJsonArray>
#include "chat/ChatBaseManger.h"
#include "base/ChatWidgetTitleBar.h"
#include "base/ChatSwitchButton.h"
#include "base/BorderContainer.h"
/**
 * @brief  Qt和Web端交互的中介单例类
 */
class Core : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)  // 定义一个属性，javascript可以读取属性值

public:
    enum CoreEnum
    {
        Value1 = 100,
        Value2
    };
    Q_ENUM(CoreEnum)    // 使用Q_ENUM标记的枚举，javascript可以直接访问

public:
    static Core* getInstance()
    {
        static Core core;
        return &core;
    }

    int value() {return m_value;}
    void setValue(int v) {m_value = v;}

signals:
    void valueChanged();
    /**
     * @brief     Qt发送给Web的信号
     * @param str
     */
    void toWebBool(bool value);
    void toWebDouble(double value);
    void toWebString(QString value);
    void toWebJsonArray(QJsonArray value);
    void toWebJsonObject(QJsonObject value);

    /**
     * @brief     Web发送给Qt的信号
     * @param str
     */
    void toQtBool(bool value);
    void toQtDouble(double value);
    void toQtString(QString value);
    void toQtJsonArray(QJsonArray value);
    void toQtJsonObject(QJsonObject value);
    void clearContext();

public slots:
    /**
     * @brief     Web端需要调用Qt槽函数来传递，必须声明为public slots，否则web找不到
     * @param str
     */
    void on_toQtBool(bool value)              {emit toQtBool(value);}
    void on_toQtDouble(double value)          {emit toQtDouble(value);}
    void on_toQtString(QString value)         {emit toQtString(value);}
    void on_toQtJsonArray(QJsonArray value)   {emit toQtJsonArray(value);}
    void on_toQtJsonObject(QJsonObject value) {emit toQtJsonObject(value);}
    void on_clearContext() {emit clearContext();}

    /**
     * @brief        定义一个带有返回值的槽函数，javascript调用该函数后可以获取返回值
     * @param value
     * @return
     */
    // QString on_returnValue(int value);
private:
    int m_value = 10;
};

class ChatLineEdit : public QLineEdit {
    Q_OBJECT
public:
    ChatLineEdit(QWidget *parent = nullptr) : QLineEdit(parent) {}
public:
signals:
    void sigTextToEmpty();
    void sigTextInput();
protected:
    void focusInEvent(QFocusEvent *event) override {
        setStyleSheet("color: #000000;");
        QLineEdit::focusInEvent(event);
    }
    void focusOutEvent(QFocusEvent *event) override {
        setStyleSheet("color: #8C8C8C;");
        QLineEdit::focusOutEvent(event);
    }
    void paintEvent(QPaintEvent *event) override{
        if(m_isEmpty){
            if(this->text() != ""){
                emit sigTextInput();
                m_isEmpty = false;
            }
        }else{
            if(this->text() == ""){
                emit sigTextToEmpty();
                m_isEmpty = true;
            }
        }
        return QLineEdit::paintEvent(event);
    }
private:
    bool m_isEmpty = true;
};

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = 0);
    ~ChatDialog();
    void init();
    void helloMessage();
protected:
    void resizeEvent(QResizeEvent *event) override;
    virtual void paintEvent(QPaintEvent* event) override;
private:
    // 发送消息后的loding
    void waitingReplyMessage();
    void changeSendBtn();
private slots:
    void slotSendBtnClicked();
    /**
     * @param done 是否传输完成
    */
    void slotReplyMessage(QString message, bool done);
    void slotWebToQtJsonObject(QJsonObject value);
    void slotReplyFailed();
    void slotReSendBtnClicked();
    // 清除上下文
    void slotClearContext();
private:
    ChatWidgetTitleBar* m_titleWidget{ nullptr };
    QWebEngineView* m_WebView{ nullptr };
    ChatLineEdit* m_lineEdit{ nullptr };
    QPushButton* m_attachmentBtn{ nullptr };
    ChatSwitchButton* m_sendBtn{ nullptr };
    ChatbaseManager* m_chatmanager{ nullptr };
    // ChatMessage* m_messageW{ nullptr };
    std::vector<QJsonObject> m_messageVec;
    bool m_isGeneratingMsg = false;
    QString m_generatingMsgUUID;
    QString m_generatingMsg;
    QPushButton* m_resendBtn{ nullptr };
    QStackedWidget* m_stackedWidget{ nullptr };
    BorderContainer* m_borderContainer{ nullptr };
};

#endif // CHATDIALOG_H