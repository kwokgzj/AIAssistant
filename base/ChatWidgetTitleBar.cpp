#include "ChatWidgetTitleBar.h"
// #include "lang/language.h"

#include <QHBoxLayout>
#include <QMouseEvent>

// using namespace rv;

ChatWidgetTitleBar::ChatWidgetTitleBar(QWidget *parent) : QFrame(parent)
{
	this->setAttribute(Qt::WA_TranslucentBackground);

	QHBoxLayout* titleLayout = new QHBoxLayout(this);

	m_backBtn = new QPushButton(this);
    m_backBtn->setFixedSize(24, 24);
    m_backBtn->setObjectName("ChatDialogBackBtn");
    m_backBtn->setFocusPolicy(Qt::NoFocus);
    m_titleLabel = new QLabel(this);
    // m_titleLabel->setText(LAN_AI_ASSISTANT);
    m_titleLabel->setText("AI助理");
    m_titleLabel->setStyleSheet("QLabel{border: null;font-size: 16px;background-color:transparent;color: #000000;}");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_closeBtn = new QPushButton(this);
    m_closeBtn->setFixedSize(24, 24);
    m_closeBtn->setObjectName("ChatDialogCloseBtn");
    m_closeBtn->setFocusPolicy(Qt::NoFocus);
    titleLayout->addSpacing(20);
    titleLayout->addWidget(m_backBtn);
    titleLayout->setAlignment(m_backBtn, Qt::AlignLeft);
    titleLayout->addWidget(m_titleLabel);
    titleLayout->addWidget(m_closeBtn);
    titleLayout->setAlignment(m_closeBtn, Qt::AlignRight);
    titleLayout->addSpacing(20);

	connect(m_backBtn, SIGNAL(clicked()), SIGNAL(sigBackBtnClicked()));
	connect(m_closeBtn, SIGNAL(clicked()), SIGNAL(sigClose()));

	this->setLayout(titleLayout);

    updataLanguage();
}

ChatWidgetTitleBar::~ChatWidgetTitleBar()
{

}

void ChatWidgetTitleBar::setTitleName(QString str)
{
	m_titleLabel->setText(str);
}

void ChatWidgetTitleBar::setIsHideCloseBtn(bool isHide)
{
	if (isHide)
	{
		m_closeBtn->hide();
	}
	else
	{
		m_closeBtn->show();
	}
}

void ChatWidgetTitleBar::showCloseBtn(bool show)
{
    if(show){
		m_closeBtn->show();
	}else{
		m_closeBtn->hide();
    }
}

void ChatWidgetTitleBar::mousePressEvent(QMouseEvent* event)
{
	m_bIsPressed = true;
	m_startMovePos = event->globalPos();

	return QFrame::mousePressEvent(event);
}

void ChatWidgetTitleBar::mouseMoveEvent(QMouseEvent* event)
{
    if (m_bIsPressed && ((event->buttons()&Qt::LeftButton) == Qt::LeftButton))
    {
        if (parentWidget())
        {
            QPoint movePoint = event->globalPos() - m_startMovePos;
            QPoint widgetPos = this->parentWidget()->pos();
            m_startMovePos = event->globalPos();
            parentWidget()->move(widgetPos.x() + movePoint.x(), widgetPos.y() + movePoint.y());
        }
    }
	return QFrame::mouseMoveEvent(event);
}

void ChatWidgetTitleBar::mouseReleaseEvent(QMouseEvent* event)
{
	m_bIsPressed = false;
	return QFrame::mouseReleaseEvent(event);
}

void ChatWidgetTitleBar::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        updataLanguage();
    }

    return QFrame::changeEvent(event);
}

void ChatWidgetTitleBar::updataLanguage()
{
    // m_titleLabel->setText(LAN_AI_ASSISTANT);
    // m_closeBtn->setToolTip(LAN_CONFIGURE);
	// m_backBtn->setToolTip(LAN_QUESTION);
    m_titleLabel->setText("AI助理");
    m_closeBtn->setToolTip("关闭");
	m_backBtn->setToolTip("");
}

void ChatWidgetTitleBar::showBackBtn(bool show)
{
	if(show){
		m_backBtn->show();
	}else{
		m_backBtn->hide();
    }
}
