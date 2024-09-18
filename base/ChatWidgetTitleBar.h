#ifndef CHATWIDGETTITLEBAR_H
#define CHATWIDGETTITLEBAR_H

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QKeyEvent>

class ChatWidgetTitleBar : public QFrame
{
    Q_OBJECT

public:
    ChatWidgetTitleBar(QWidget *parent = nullptr);
    ~ChatWidgetTitleBar();

    void setTitleName(QString str);
    void setIsHideCloseBtn(bool isHide);

    void showCloseBtn(bool show);
    void showBackBtn(bool show);

private:
    void updataLanguage();

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void changeEvent(QEvent* event) override;

signals:
    void sigClose();
    void sigBackBtnClicked();

private:
    bool m_bIsPressed{false};
    QPoint m_startMovePos;

    QLabel* m_titleLabel{ nullptr };
    QPushButton* m_closeBtn{ nullptr };
    QPushButton* m_backBtn{ nullptr };
};

#endif // CHATWIDGETTITLEBAR_H
