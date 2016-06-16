/*
 * 独立的线程类，将Main线程和download线程和载入音乐的线程拆分
 * 防止在读取数据库和本地音乐文件的时候，界面无响应，提高CPU利用率
 */

#ifndef THREAD
#define THREAD
#include "downloaddialog.h"
#include "musicwindow.h"
#include <QThread>

class Thread : public QThread
{
    Q_OBJECT

public:
    Thread(QObject *parent = 0);
    ~Thread(){}
    QFileInfoList list;
    int MaxNum;
    QStringList songList;
    QStringList name;

protected:
        void run();

private:
        void upgradeData();
        void loadMusic();

signals:
        void upgrade_Signal(int MaxNum,  QStringList songList, QStringList name, QFileInfoList list);
};

#endif // THREAD

