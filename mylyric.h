#ifndef MYLYRIC
#define MYLYRIC

#include <QWidget>
#include <QtWidgets>
#include <QTimer>
#include <QLabel>
#include <QLinearGradient>
#include <QFont>
#include <QPoint>
#include <QPainter>
#include <QPaintEvent>
#include <QMenu>
#include <QTextCodec>

class MyLyric : public QLabel
{
    Q_OBJECT
public:
    explicit MyLyric(QWidget *parent = 0);
    void start_Lyric_Mask(qint64 interval);
    void stop_Lyric_Mask();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void timeOut();

private:
    QLinearGradient linear_Gradient;
    QLinearGradient mask_Linear_Gradient;
    QFont font;
    QTimer *timer;
    qreal lrc_Mask_width;
    qreal lrc_Mask_Interval;
    QPoint offset;
    bool mouse_Press;   //鼠标事件
    QPoint mouse_Point;
};

#endif // MYLYRIC

