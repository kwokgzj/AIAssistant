#ifndef CHATSWITCHBUTTON_H
#define CHATSWITCHBUTTON_H

#include <QPushButton>
#include <QStyle>

class ChatSwitchButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(bool choose READ choose WRITE setChoose NOTIFY chooseChanged)

public:
    ChatSwitchButton(QWidget *parent = nullptr);
    ~ChatSwitchButton();

    bool choose() const { return m_bIsChoose;}
    void setChoose(bool bChoose);

signals:
    void chooseChanged();

private:
    bool m_bIsChoose{ false };
};
#endif // CHATSWITCHBUTTON_H
