#include "BorderContainer.h"

BorderContainer::BorderContainer(QWidget *parent,uint16_t minWindowHeight,uint16_t minWindowWidth, uint16_t borderSize)
{
    m_widget = parent;
    this->minWindowHeight = minWindowHeight; //最小窗口高度
    this->minWindowWidth = minWindowWidth;   //最小窗口宽度
    this->borderSize = borderSize;
    this->setParent(parent);
    InitBorder();
}

void BorderContainer::InitBorder()
{
    //上下左右的label，为了控制界面能够拖动拉伸
    labelLft = new FramessBorder(m_widget, L_BORDER, this);
    labelLft->setObjectName("FramessBorderLabel");
    labelLft->raise();


    labelRit = new FramessBorder(m_widget, R_BORDER, this);
    labelRit->setObjectName("FramessBorderLabel");
    labelRit->raise();

    labelBot = new FramessBorder(m_widget, B_BORDER, this);
    labelBot->setObjectName("FramessBorderLabel");
    labelBot->raise();

    labelTop = new FramessBorder(m_widget, T_BORDER, this);
    labelTop->setObjectName("FramessBorderLabel");
    labelTop->raise();


    labelRB = new FramessBorder(m_widget, RB_BORDER, this);
    labelRB->setObjectName("FramessBorderLabel");
    labelRB->raise();


    labelRT = new FramessBorder(m_widget, RT_BORDER, this);
    labelRT->setObjectName("FramessBorderLabel");
    labelRT->raise();


    labelLB = new FramessBorder(m_widget, LB_BORDER, this);
    labelLB->setObjectName("FramessBorderLabel");
    labelLB->raise();

    labelLT = new FramessBorder(m_widget, LT_BORDER, this);
    labelLT->setObjectName("FramessBorderLabel");
    labelLT->raise();

    DarwBorder();
}

void BorderContainer::setMinWindowSize(uint16_t minWindowWidth, uint16_t minWindowHeight)
{
    this->minWindowHeight = minWindowHeight; //最小窗口高度
    this->minWindowWidth = minWindowWidth;   //最小窗口宽度
}

void BorderContainer::setBorderSize(uint16_t borderSize)
{
    this->borderSize = borderSize;
}

//重绘边框
void BorderContainer::DarwBorder()
{
    labelLft->setGeometry(0,0,this->borderSize,m_widget->height());
    labelRit->setGeometry(m_widget->width()-this->borderSize,0,this->borderSize,m_widget->height());
    labelBot->setGeometry(0,m_widget->height()-this->borderSize,m_widget->width(),this->borderSize);
    labelTop->setGeometry(0,0,m_widget->width(),this->borderSize);
    labelRB->setGeometry(m_widget->width()-this->borderSize-1,m_widget->height()-this->borderSize-1,m_widget->width(),m_widget->height());
    labelRT->setGeometry(m_widget->width()-this->borderSize-1,0,m_widget->width(),this->borderSize+1);
    labelLB->setGeometry(0,m_widget->height()-this->borderSize-1,this->borderSize+1,m_widget->width());
    labelLT->setGeometry(0,0,this->borderSize+1,this->borderSize+1);
}

void BorderContainer::getLeftScaleEvent(QPoint movPoint)
{
    if((m_widget->pos().x()+movPoint.x())>(m_widget->pos().x()+m_widget->width()-minWindowWidth))
    {
        return;//保证拖动窗口左边界的时候，控件宽度至少有200
    }
    m_widget->setGeometry(m_widget->pos().x()+movPoint.x(),m_widget->pos().y(),m_widget->width()-movPoint.x(),m_widget->height());
    DarwBorder();
}

void BorderContainer::getRightScaleEvent(QPoint movPoint)
{
    if((m_widget->pos().x()+m_widget->width()+movPoint.x())<(m_widget->pos().x()+minWindowWidth))
    {
        return;//保证拖动窗口右边界的时候，控件宽度至少有200
    }
    m_widget->setGeometry(m_widget->pos().x(),m_widget->pos().y(),m_widget->width()+movPoint.x(),m_widget->height());
    DarwBorder();
}

void BorderContainer::getBottomScaleEvent(QPoint movPoint)
{
    if((m_widget->pos().y()+m_widget->height()+movPoint.y())<(m_widget->pos().y()+minWindowHeight))
    {
        return;//保证拖动窗口下边界的时候，控件高度至少有200
    }
    m_widget->setGeometry(m_widget->pos().x(),m_widget->pos().y(),m_widget->width(),m_widget->height()+movPoint.y());
    DarwBorder();
}

void BorderContainer::getTopScaleEvent(QPoint movPoint)
{
    if((m_widget->pos().y()+movPoint.y())>(m_widget->pos().y()+m_widget->height()-minWindowHeight))
    {
        return;//保证拖动窗口上边界的时候，控件高度至少有200
    }
    m_widget->setGeometry(m_widget->pos().x(),m_widget->pos().y()+movPoint.y(),m_widget->width(),m_widget->height()-movPoint.y());
    DarwBorder();
}

void BorderContainer::getRBScaleEvent(QPoint movPoint)
{
    if((m_widget->pos().y()+m_widget->height()+movPoint.y())<(m_widget->pos().y()+minWindowHeight)
            || (m_widget->pos().x()+m_widget->width()+movPoint.x())<(m_widget->pos().x()+minWindowWidth))
    {
        return;//保证拖动窗口上边界的时候，控件高度和宽度至少有200
    }
    m_widget->setGeometry(m_widget->pos().x(),m_widget->pos().y(),m_widget->width()+movPoint.x(),m_widget->height());
    m_widget->setGeometry(m_widget->pos().x(),m_widget->pos().y(),m_widget->width(),m_widget->height()+movPoint.y());
    DarwBorder();
}

void BorderContainer::getRTScaleEvent(QPoint movPoint)
{
    if((m_widget->pos().x()+m_widget->width()+movPoint.x())<(m_widget->pos().x()+minWindowWidth)
            || (m_widget->pos().y()+movPoint.y())>(m_widget->pos().y()+m_widget->height()-minWindowHeight) )
    {
        return;//保证拖动窗口上边界的时候，控件高度和宽度至少有200
    }
    m_widget->setGeometry(m_widget->pos().x(),m_widget->pos().y()+movPoint.y(),m_widget->width(),m_widget->height()-movPoint.y());
    m_widget->setGeometry(m_widget->pos().x(),m_widget->pos().y(),m_widget->width()+movPoint.x(),m_widget->height());
    DarwBorder();
}

void BorderContainer::getLTScaleEvent(QPoint movPoint)
{
    if((m_widget->pos().x()+movPoint.x())>(m_widget->pos().x()+m_widget->width()-minWindowWidth)
            || (m_widget->pos().y()+movPoint.y())>(m_widget->pos().y()+m_widget->height()-minWindowHeight) )
    {
        return;//保证拖动窗口上边界的时候，控件高度和宽度至少有200
    }
    m_widget->setGeometry(m_widget->pos().x()+movPoint.x(),m_widget->pos().y(),m_widget->width()-movPoint.x(),m_widget->height());
    m_widget->setGeometry(m_widget->pos().x(),m_widget->pos().y()+movPoint.y(),m_widget->width(),m_widget->height()-movPoint.y());
    DarwBorder();
}

void BorderContainer::getLBScaleEvent(QPoint movPoint)
{
    if((m_widget->pos().x()+movPoint.x())>(m_widget->pos().x()+m_widget->width()-minWindowWidth)
            || (m_widget->pos().y()+m_widget->height()+movPoint.y())<(m_widget->pos().y()+minWindowHeight))
    {
        return;//保证拖动窗口上边界的时候，控件高度和宽度至少有200
    }
    m_widget->setGeometry(m_widget->pos().x()+movPoint.x(),m_widget->pos().y(),m_widget->width()-movPoint.x(),m_widget->height());
    m_widget->setGeometry(m_widget->pos().x(),m_widget->pos().y(),m_widget->width(),m_widget->height()+movPoint.y());
    DarwBorder();
}

BorderContainer::FramessBorder::FramessBorder(QWidget *parent, BorderType type, BorderContainer * contex)
{
    this->setParent(parent);
    KeepDrag = false;
    MyType = type;
    container = contex;
    if(MyType == L_BORDER || MyType == R_BORDER)
    {
        setCursor(Qt::SizeHorCursor);
    }
    else if(MyType == T_BORDER || MyType == B_BORDER)
    {
        setCursor(Qt::SizeVerCursor);
    }
    else if(MyType == LT_BORDER || MyType == RB_BORDER)
    {
        setCursor(Qt::SizeFDiagCursor);
    }
    else if(MyType == LB_BORDER || MyType == RT_BORDER)
    {
        setCursor(Qt::SizeBDiagCursor);
    }
    else
    {
        setCursor(Qt::ArrowCursor);
    }
}

//鼠标点击事件
void BorderContainer::FramessBorder::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        KeepDrag = true;
        mousePointOld = ev->globalPos();
    }
    return QWidget::mousePressEvent(ev);
}

//鼠标移动事件
void BorderContainer::FramessBorder::mouseMoveEvent(QMouseEvent *ev)
{
   if(KeepDrag)
   {
       const QPoint position = ev->globalPos() - mousePointOld;

       //调用外部类接口
       switch (MyType) {
       case L_BORDER: //左
           container->getLeftScaleEvent(position);
           break;
       case R_BORDER: //右
           container->getRightScaleEvent(position);
           break;
       case T_BORDER: //上
           container->getTopScaleEvent(position);
           break;
       case B_BORDER: //上
           container->getBottomScaleEvent(position);
           break;
       case RB_BORDER: //右下
           container->getRBScaleEvent(position);
           break;
       case RT_BORDER: //右上
           container->getRTScaleEvent(position);
           break;
       case LB_BORDER: //左下
           container->getLBScaleEvent(position);
           break;
       case LT_BORDER: //左上
           container->getLTScaleEvent(position);
           break;
       default:
           break;
       }
       mousePointOld = ev->globalPos();
   }
   return QWidget::mouseMoveEvent(ev);
}

//鼠标释放事件
void BorderContainer::FramessBorder::mouseReleaseEvent(QMouseEvent *ev)
{
    Q_UNUSED(ev)
    KeepDrag = false;
    return QWidget::mouseReleaseEvent(ev);
}
