#include "thread.h"

Thread::Thread(QObject *parent):QThread(parent)
{
    MaxNum = -1;
}

void Thread::run()
{
        loadMusic();
}

//载入音乐
void Thread::loadMusic()
{
    QSqlQuery query1;

    query1.exec("select * from music_songs");
    QString dirname = "C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/MusicResources";
    query1.last();
    qDebug() << query1.lastError(); //查询错误的语句

    //类QDir提供了对目录结构和它们的内容的访问方式。
    //QDir用来操作路径名，访问关于路径和文件的信息，操作基础的文件系统，还可以用来访问Qt的资源系统。
    QDir qd("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/MusicResources");  //设置绝对路径
    qd.setFilter(QDir::Files|QDir::Hidden|QDir::NoSymLinks);
    qd.setSorting(QDir::Size|QDir::Reversed);
    list = qd.entryInfoList();
    for(int i=0;i< list.size();++i)
    {
        if(list.at(i).fileName().right(3) == "mp3")
        {
            songList += dirname + "/" + list.at(i).fileName();
            MaxNum += 1;
            name += list.at(i).fileName();
            query1.exec(QString("insert into music_songs values('%1', 0)").arg(list.at(i).fileName()));  //更新数据库
        }
    }

    upgradeData();
}

void Thread::upgradeData()
{
    emit upgrade_Signal(MaxNum, songList, name, list);
}
