#ifndef MUSICWINDOW_H
#define MUSICWINDOW_H
#include <mylyric.h>
#include <thread.h>
#include <aboutdialog.h>

#include <QMainWindow>
#include <QtWidgets>
#include <QMessageBox>
#include <QList>
#include <QPushButton>
#include <QTextBrowser>
#include <QTextStream>
#include <QFile>
#include <QSlider>
#include <QFileDialog>
#include <QDebug>
#include <QUrl>
#include <QTime>
#include <QListWidgetItem>
#include <QFrame>
#include <QMouseEvent>
#include <QWidget>
#include <QSizeGrip>
#include <QPalette>
#include <QSpacerItem>
#include <QMap>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMediaContent>
#include <QPixmap>
#include <QPixmapCache>
#include <QtWebKit>
#include <QWebView>
#include <QTextStream>
#include <QPoint>
#include <QMouseEvent>
#include <QAction>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSql>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDir>
#include <QTextCodec>
#include <QIODevice>
#include <ctime>
#include <QMenu>

class Thread;

namespace Ui {
class MusicWindow;
}

class MusicWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MusicWindow(QWidget *parent = 0);
    ~MusicWindow();

private slots:
    void show_Hide_Lyric();
    void warning();
    void adjustVolume();
    void nextSong();
    void lastSong();
    void setApearence();
    void doubleClickedPlay(QListWidgetItem *a);
    void setPlayRange(qint64 position);
    void setPlayPosition(qint64 position);
    void setSliderPosition();
    void playEvent();
    void updateTime(qint64);//更新时间
    void lyric_Sync(QMediaPlayer::State newState);
    void browserSet();//控制浏览器
    void setElement_BackGround();//给主窗口的界面元素添加背景图片以及设置透明度
    void markSong();//标记喜欢的歌曲
    void set_Mute();//设置静音
    void store_Show();//显示音乐库
    void favoriteSongs_Show();//显示喜爱的歌曲列表
    void browser_Show();//显示刷新浏览器
    void user_login();//在线用户登录(默认是本地帐户)
    void download_File();//下载
    void musicLoad();//载入硬盘的歌曲
    void upgradeData(int MaxNum,  QStringList songList, QStringList name, QFileInfoList list);//同步数据
    void playMode_Change();//播放模式变化
    void songsSearch();//打开搜索框
    void songs_Search(QString text);//搜索列表歌曲
    void clear_List();//清空列表
    void load_again();//重新载入
    void about();//关于

private:
    Ui::MusicWindow *ui;

    int MaxNum;
    int playNum;
    int last_PlayNum;
    int search_Last;
    bool mouse_Press;   //鼠标事件
    QPoint mouse_Point;
    QString LoginName;   //登录用户名
    QString LoginPassword;   //登陆密码
    QMediaPlayer *player;
    MyLyric *lyric;
    QMap<qint64, QString>lrc_Map;
    QStringList songList;
    QStringList searchList;
    QFileInfoList list;
    QStringList name;
    QString themeChoise;
    QString themeToUse;
    QPalette titlePalette;
    QAction *next_song;
    QAction *last_song;
    QAction *pause_song;
    QAction *small_window;
    QAction *full_window;
    QAction *search;
    QAction *showMini;
    QAction *clearList;
    QAction *quit;
    QAction *login;
    QAction *download;
    QAction *information;
    Thread  *thread;
    QString playMode;
    void resolve_lrc(const QString &source_File_Name);//歌词解析

public slots:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void contextMenuEvent(QContextMenuEvent *event);
};

#endif // MUSICWINDOW_H
