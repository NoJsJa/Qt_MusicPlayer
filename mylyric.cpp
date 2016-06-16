#include "mylyric.h"

MyLyric::MyLyric(QWidget *parent):
    QLabel(parent)
{
    //设置无边框窗口
    setWindowFlags(Qt::Window  | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setText(tr("没有歌词!"));
    //固定显示区域
    setMaximumSize(1400, 60);
    setMinimumSize(1400, 60);
    //歌词线性渐变填充
    linear_Gradient.setStart(0, 10);
    linear_Gradient.setFinalStop(0, 40);
    linear_Gradient.setColorAt(0.1, QColor(14, 179, 255));
    linear_Gradient.setColorAt(0.5, QColor(114, 232, 255));
    linear_Gradient.setColorAt(0.9, QColor(14, 179, 255));
    //mask的线性渐变填充
    mask_Linear_Gradient.setStart(0, 20);
    mask_Linear_Gradient.setFinalStop(0, 40);
    mask_Linear_Gradient.setColorAt(0.1, QColor(222, 54, 4));
    mask_Linear_Gradient.setColorAt(0.5, QColor(255, 72, 16));
    mask_Linear_Gradient.setColorAt(0.9, QColor(222, 54, 4));
    //设置字体
    font.setFamily("Times New Roman");
    font.setBold(true);
    font.setPointSize(30);
    //定时器
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeOut()));
    lrc_Mask_width = 0;
    lrc_Mask_Interval = 0;
}

void MyLyric::start_Lyric_Mask(qint64 interval)
{
    //30毫秒更新一次mask的宽度
    qreal count = interval / 30;
    lrc_Mask_Interval = 1400 / count;  //1400是自定义的显示部件label的像素宽度，这个变量的单位是毫秒
    lrc_Mask_width = 0; //mask(歌词遮蔽，初值为零，mask从左到右依次递增，呈现动态变化的式样)

    //QTimer类提供了定时器信号和单触发定时器
    timer->start(30); //开启定时器
}

void MyLyric::stop_Lyric_Mask()
{
    timer->stop();//停止定时器
    lrc_Mask_width = 0;
    update(); //update函数更新部件状态，不用手动调用，系统会调用，详细见API
}

void MyLyric::timeOut()
{
    lrc_Mask_width += lrc_Mask_Interval;  //更新歌词绘制宽度
    update();
}

void MyLyric::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setFont(font);

    //绘制底部黑色文字 & 左对齐
    painter.setPen(QColor(0, 0, 0, 200));
    painter.drawText(1, 1, 1400, 60, Qt::AlignLeft, text());
    //绘制线性渐变文字
    painter.setPen(QPen(linear_Gradient, 0));
    painter.drawText(0, 0, 1400, 60, Qt::AlignLeft, text());
    //绘制歌词mask
    painter.setPen(QPen(mask_Linear_Gradient, 0));
    painter.drawText(0, 0, lrc_Mask_width, 60, Qt::AlignLeft, text());

}

void MyLyric::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        mouse_Point = event->pos();
        mouse_Press = true;
    }
}

void MyLyric::mouseMoveEvent(QMouseEvent *event)
{
    if(mouse_Press)
    {
        QPoint move_position = event->globalPos();
        this->move(move_position - mouse_Point);
    }
}

void MyLyric::mouseReleaseEvent(QMouseEvent *)
{
    mouse_Press = false;
}
