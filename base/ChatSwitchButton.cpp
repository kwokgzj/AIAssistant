#include "ChatSwitchButton.h"
#include <QStyleOption>
#include <QPainter>

ChatSwitchButton::ChatSwitchButton(QWidget *parent)
    :QPushButton(parent)
{

}

ChatSwitchButton::~ChatSwitchButton()
{

}

void ChatSwitchButton::setChoose(bool bChoose)
{
    if(bChoose == m_bIsChoose) return;
    m_bIsChoose = bChoose;

    emit chooseChanged();
    this->style()->unpolish(this);
    this->style()->polish(this);
    this->update();
}
