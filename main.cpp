#include "logindialog.h"
#include "musicwindow.h"
#include <QApplication>

#include <QPixmap>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //程序启动画面
    QPixmap pixmap("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/start.png");
    QSplashScreen splash(pixmap);
    splash.show();
    a.processEvents();//使程序在显示启动画面的同时能够响应鼠标等其它时间
    //连接数据库
    QSqlDatabase  db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Qt/Qt5.4.1/projects/databases/music.db");
    if(!db.open())
    {
        QMessageBox::information(0, QObject::tr("Error"), QObject::tr("连接出错"));
    }
    QSqlQuery sql_query;
    if(!sql_query.exec("SELECT * FROM music_user"))
    {
        qDebug() << sql_query.lastError();
    }
    else
    {
        qDebug() << "音乐库已经连接！";
    }

    MusicWindow w;
    w.resize(QSize(380, 657));
    w.show();
    splash.finish(&w);

    return a.exec();
}
