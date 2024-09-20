#include "ChatDialog.h"
#include <QDateTime>
#include <QDebug>
#include <QPainter>
#include <QWebEngineSettings>
#include <QWebChannel>
#include <QUuid>
#include <QDesktopServices>
#include <iostream>
#include <QFile>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QApplication>
#include <QDir>
// #include "../ui/lang/language.h"
ChatDialog::ChatDialog(QWidget *parent) :
    QDialog(parent)
{
    this->setAttribute(Qt::WA_TranslucentBackground);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window | Qt::WindowMaximizeButtonHint);
    setMouseTracking(true);
    resize(356, 744);
    setMinimumWidth(356);
    setMinimumHeight(386);
    init();
}

ChatDialog::~ChatDialog()
{
}

void ChatDialog::init(){
    // 创建网格布局管理器
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setObjectName("mainLayout");
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    this->setLayout(mainLayout);

    m_titleWidget = new ChatWidgetTitleBar(this);
    m_titleWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_titleWidget->setFixedHeight(48);
    m_titleWidget->setObjectName("ChatDialogTitleWidget");
    connect(m_titleWidget, &ChatWidgetTitleBar::sigClose, this, &QDialog::close);
    mainLayout->addWidget(m_titleWidget);

    m_WebView = new QWebEngineView(this);
    m_WebView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_WebView->setObjectName("ChatWebEngineView");
    m_WebView->setAttribute(Qt::WA_TranslucentBackground);
    // QFile file(":/AIAssistantPage/index.html");
    QString htmlFilePath = QCoreApplication::applicationDirPath() + QDir::separator() + "AIAssistantPage" + QDir::separator() + "index.html";
    // QString htmlFilePath = file.fileName();
    std::cout << "zjGuo test htmlFilePath --- " << htmlFilePath.toStdString() << std::endl;
    // size_t pos = htmlFilePath.lastIndexOf('/');
    // if (pos != -1) {
    //     // 截取斜杠之前的部分
    //     htmlFilePath = htmlFilePath.left(pos) + "/index.html";
    // }
    m_WebView->load(QUrl::fromUserInput(htmlFilePath));
    m_WebView->setZoomFactor(1.0);
    // m_WebView->page()->settings()->setAttribute(QWebEngineSettings::ShowScrollBars, false);
    m_WebView->show();
    mainLayout->addWidget(m_WebView);
    connect(m_WebView, &QWebEngineView::loadFinished, [&]{
        QTimer::singleShot(200, [&]{
                helloMessage();
            });
    });

    QWebChannel *channel = new QWebChannel(m_WebView->page());
    channel->registerObject(QStringLiteral("CoreId"), Core::getInstance());
    m_WebView->page()->setWebChannel(channel);

    // 输入框窗口
    QWidget* textEditWidget = new QWidget(this);
    textEditWidget->setFixedHeight(45);
    textEditWidget->setMinimumWidth(324);
    textEditWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    textEditWidget->setObjectName("ChatTextEditWidget");
    QHBoxLayout* textEditLayout = new QHBoxLayout(textEditWidget);
    textEditLayout->setContentsMargins(0, 0, 0, 0);
    textEditWidget->setLayout(textEditLayout);
    
    m_lineEdit = new ChatLineEdit(this);
    m_lineEdit->setContentsMargins(0, 0, 0, 0);
    m_lineEdit->setFixedHeight(40);
    m_lineEdit->setMinimumWidth(215);
    m_lineEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    m_lineEdit->setObjectName("ChatLineEdit");
    m_lineEdit->setAttribute(Qt::WA_MacShowFocusRect, false);
    m_lineEdit->setPlaceholderText("LAN_AI_ASSISTANT_DEFAULT_INPUT_TEXT");
    connect(m_lineEdit, &ChatLineEdit::returnPressed, [&]{
        if(m_lineEdit->text() != ""){
            m_sendBtn->click();
        }
    });
    connect(m_lineEdit, &ChatLineEdit::sigTextToEmpty, [&]{
        if(!m_isGeneratingMsg){
            m_sendBtn->setCheckable(false);
            m_sendBtn->setEnabled(false);
            m_sendBtn->setToolTip("LAN_SEND");
        }
    });
    connect(m_lineEdit, &ChatLineEdit::sigTextInput, [&]{
        if(!m_isGeneratingMsg){
            m_sendBtn->setChoose(true);
            m_sendBtn->setToolTip("LAN_SEND");
            m_sendBtn->setEnabled(true);
            m_sendBtn->setCheckable(true);
        }
    });

    m_attachmentBtn = new QPushButton(this);
    m_attachmentBtn->setContentsMargins(0, 0, 0, 0);
    m_attachmentBtn->setFixedSize(20, 20);
    m_attachmentBtn->setObjectName("ChatAttachmentBtnBtn");
    m_attachmentBtn->setToolTip("LAN_ATTACHMENT");
    QLabel* splitLabel = new QLabel(this);
    splitLabel->setAlignment(Qt::AlignVCenter);
    splitLabel->setAlignment(Qt::AlignHCenter);
    splitLabel->setText("|");
    splitLabel->setFixedSize(24, 20);
    splitLabel->setStyleSheet("QLabel{border: null;font-size: 16px;background-color:transparent;color: #D9D9D9;}");

    m_sendBtn = new ChatSwitchButton(this);
    m_sendBtn->setContentsMargins(0, 0, 0, 0);
    m_sendBtn->setFixedSize(20, 20);
    m_sendBtn->setObjectName("ChatSendBtn");
    m_sendBtn->setToolTip("LAN_SEND");
    m_sendBtn->setEnabled(false);
    connect(m_sendBtn, &QPushButton::clicked, this, &ChatDialog::slotSendBtnClicked);

    textEditLayout->addSpacing(16);
    textEditLayout->addWidget(m_lineEdit);
    textEditLayout->addWidget(m_attachmentBtn);
    textEditLayout->addWidget(splitLabel);
    textEditLayout->addWidget(m_sendBtn);
    textEditLayout->addSpacing(16);

    // 重新发送窗口
    QWidget* resendBtnWiget = new QWidget(this);
    resendBtnWiget->setFixedSize(324, 45);
    resendBtnWiget->setObjectName("ChatResendBtnWidget");
    QHBoxLayout* resendBtnLayout = new QHBoxLayout();
    resendBtnLayout->setContentsMargins(0, 0, 0, 0);
    resendBtnWiget->setLayout(resendBtnLayout);
    m_resendBtn = new QPushButton(this);
    m_resendBtn->setContentsMargins(0, 0, 0, 0);
    m_resendBtn->setFixedSize(200, 45);
    m_resendBtn->setObjectName("ChatResendBtn");
    m_resendBtn->setText("LAN_RESEND");
    resendBtnLayout->addWidget(m_resendBtn);
    resendBtnLayout->setAlignment(m_resendBtn, Qt::AlignHCenter);
    connect(m_resendBtn, &QPushButton::clicked, this, &ChatDialog::slotReSendBtnClicked);

    m_stackedWidget = new QStackedWidget(this);
    m_stackedWidget->setObjectName("ChatStackedWidget");
    m_stackedWidget->setFixedHeight(45);
    m_stackedWidget->setMinimumWidth(324);
    m_stackedWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    
    m_stackedWidget->addWidget(textEditWidget);
    m_stackedWidget->addWidget(resendBtnWiget);
    m_stackedWidget->setCurrentIndex(0);

    mainLayout->addSpacing(4);
    QHBoxLayout* stackWidgetLayout = new QHBoxLayout();
    stackWidgetLayout->addSpacing(16);
    stackWidgetLayout->addWidget(m_stackedWidget);
    stackWidgetLayout->addSpacing(16);
    mainLayout->addLayout(stackWidgetLayout);
    mainLayout->addSpacing(16);

    m_chatmanager = new ChatbaseManager();
    connect(m_chatmanager, &ChatbaseManager::sigPostReply, this, &ChatDialog::slotReplyMessage);
    connect(m_chatmanager, &ChatbaseManager::sigPostReplyFailed, this, &ChatDialog::slotReplyFailed);
    connect(Core::getInstance(), &Core::toQtJsonObject, this, &ChatDialog::slotWebToQtJsonObject);
    connect(Core::getInstance(), &Core::clearContext, this, &ChatDialog::slotClearContext);

    m_borderContainer = new BorderContainer(this, 386, 356);
    m_pipeManager = new PipeManager("revoscan");
}

void ChatDialog::slotSendBtnClicked()
{
    if(!m_isGeneratingMsg && m_sendBtn->choose()){
        QString msg = m_lineEdit->text();
        QJsonObject obj;
        QUuid id = QUuid::createUuid();
        QString strId = id.toString();
        obj.insert("uuid", strId);
        obj.insert("user", "user");
        obj.insert("context", msg);
        emit Core::getInstance()->toWebJsonObject(obj);
        m_messageVec.push_back(obj);
        // for(int i = 0; i < m_messageVec.size(); i++){
        //     std::cout << "zjGuo test messages[" << i << "]  ---- uuid : " << m_messageVec[i].value("uuid").toString().toStdString() << " , user : " << m_messageVec[i].value("user").toString().toStdString() << " , context : " << m_messageVec[i].value("context").toString().toStdString() << std::endl;
        // }
        m_lineEdit->clear();
        // m_chatmanager->sendMessage(m_messageVec);
        m_chatmanager->sendMessage1(obj);
        waitingReplyMessage();
        m_isGeneratingMsg = true;
        changeSendBtn();
    }else if(m_isGeneratingMsg && !m_sendBtn->choose()){
        QJsonObject obj;
        obj.insert("uuid", m_generatingMsgUUID);
        obj.insert("user", "assistant");
        obj.insert("context", "");
        obj.insert("done", true);
        emit Core::getInstance()->toWebJsonObject(obj);
        m_isGeneratingMsg = false;
        m_chatmanager->stopWaitingReply();
        m_generatingMsg = "";
        changeSendBtn();
    }
}

void ChatDialog::resizeEvent(QResizeEvent *event)
{
    // Q_UNUSED(event);


    // m_lineEdit->resize(this->width() - 20, this->height() - 20);
    // m_lineEdit->move(10, 10);

    // m_sendBtn->move(m_lineEdit->width()+m_lineEdit->x() - m_sendBtn->width() - 10,
    //                      m_lineEdit->height()+m_lineEdit->y() - m_sendBtn->height() - 10);


    // for(int i = 0; i < m_listWidget->count(); i++) {
    //     ChatMessage* messageW = (ChatMessage*)m_listWidget->itemWidget(m_listWidget->item(i));
    //     QListWidgetItem* item = m_listWidget->item(i);

    //     dealMessage(messageW, item, messageW->text(), messageW->time(), messageW->userType());
    // }
}

void ChatDialog::slotReplyMessage(QString message, bool done){
    if(m_isGeneratingMsg){
        // 解决发送时无网，发送后有网，返回200+空数据的bug
        if(message == "" && done && m_generatingMsg == ""){
            slotReplyFailed();
            return;
        }
        QJsonObject obj;
        obj.insert("uuid", m_generatingMsgUUID);
        obj.insert("user", "assistant");
        obj.insert("context", message);
        obj.insert("done", done);
        emit Core::getInstance()->toWebJsonObject(obj);
        m_isGeneratingMsg = !done;
        if(done){
            QJsonObject obj1;
            obj1.insert("uuid", m_generatingMsgUUID);
            obj1.insert("user", "assistant");
            obj1.insert("context", m_generatingMsg);
            m_messageVec.push_back(obj1);
            m_generatingMsg = "";
            changeSendBtn();
        }else{
            m_generatingMsg += message;
        }
    }
}

void ChatDialog::waitingReplyMessage(){
    QJsonObject obj;
    m_generatingMsgUUID = QUuid::createUuid().toString();
    obj.insert("uuid", m_generatingMsgUUID);
    obj.insert("user", "assistant");
    obj.insert("context", "");
    obj.insert("done", false);
    emit Core::getInstance()->toWebJsonObject(obj);
    m_isGeneratingMsg = true;
}

void ChatDialog::paintEvent(QPaintEvent *event){
    QLinearGradient linearGradient(QPointF(0, 0),QPointF(356, 0));
    linearGradient.setColorAt(0, QColor(220, 234, 245));
    linearGradient.setColorAt(0.5, QColor(237, 240, 230));
    linearGradient.setColorAt(1, QColor(249, 226, 225));
    linearGradient.setSpread(QGradient::PadSpread);
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(linearGradient); //添加画刷
    painter.drawRoundedRect(0, 0, this->width(), 72, 12, 12); //绘制矩形
    
    QBrush brush(QColor(255, 255, 255)); //画刷
    painter.setBrush(brush); //添加画刷
    painter.drawRoundedRect(0, 48, this->width(), this->height() - 48, 12, 12); //绘制矩形
}

void ChatDialog::slotWebToQtJsonObject(QJsonObject value){
    if(value.value("type") == "link"){
        QDesktopServices::openUrl(QUrl(value.value("data").toString()));
    }
}

void ChatDialog::helloMessage(){
    QJsonObject obj;
    obj.insert("uuid", QUuid::createUuid().toString());
    obj.insert("user", "assistant");
    obj.insert("context", "LAN_AI_ASSISTANT_HELLO");
    obj.insert("done", true);
    emit Core::getInstance()->toWebJsonObject(obj);
}


void ChatDialog::slotClearContext(){
    m_chatmanager->stopWaitingReply();
    m_messageVec.clear();
    // delete m_chatmanager;
    // m_chatmanager = new ChatbaseManager();
    m_isGeneratingMsg = false;
    changeSendBtn();
}

void ChatDialog::slotReplyFailed(){
    if(m_isGeneratingMsg){
        if(m_generatingMsg == ""){
            QJsonObject obj;
            obj.insert("uuid", m_generatingMsgUUID);
            obj.insert("user", "assistant");
            obj.insert("context", "LAN_AI_ASSISTANT_NETWORK_NERROR");
            obj.insert("done", true);
            emit Core::getInstance()->toWebJsonObject(obj);
        }else{
            QJsonObject obj;
            obj.insert("uuid", m_generatingMsgUUID);
            obj.insert("user", "assistant");
            obj.insert("context", "");
            obj.insert("done", true);
            emit Core::getInstance()->toWebJsonObject(obj);
            QJsonObject obj1;
            obj1.insert("uuid", QUuid::createUuid().toString());
            obj1.insert("user", "assistant");
            obj.insert("context", "LAN_AI_ASSISTANT_NETWORK_NERROR");
            obj1.insert("done", true);
            emit Core::getInstance()->toWebJsonObject(obj1);
        }
        m_generatingMsg = "";
        m_isGeneratingMsg = false;
        changeSendBtn();
        m_stackedWidget->setCurrentIndex(1);
    }
}

void ChatDialog::slotReSendBtnClicked()
{
    if(!m_isGeneratingMsg){
        QString msg = m_messageVec.back().value("context").toString();
        QJsonObject obj;
        QUuid id = QUuid::createUuid();
        QString strId = id.toString();
        obj.insert("uuid", strId);
        obj.insert("user", "user");
        obj.insert("context", msg);
        emit Core::getInstance()->toWebJsonObject(obj);
        m_messageVec.push_back(obj);
        // m_chatmanager->sendMessage(m_messageVec);
        m_chatmanager->sendMessage1(obj);
        waitingReplyMessage();
        m_isGeneratingMsg = true;
        m_stackedWidget->setCurrentIndex(0);
        changeSendBtn();
    }
}

void ChatDialog::changeSendBtn(){
    if(m_isGeneratingMsg){
        m_sendBtn->setEnabled(true);
        m_sendBtn->setChoose(false);
        m_sendBtn->setToolTip("LAN_STOP_TRANSFMIT");
    }
    else if(m_lineEdit->text() == ""){
        m_sendBtn->setEnabled(false);
        m_sendBtn->setCheckable(false);
        m_sendBtn->setToolTip("LAN_SEND");
    }else{
        m_sendBtn->setEnabled(true);
        m_sendBtn->setChoose(true);
        m_sendBtn->setToolTip("LAN_SEND");
        m_sendBtn->setCheckable(true);
    }
}

void ChatDialog::setPipeName(QString name){
    m_pipeName = name;
}