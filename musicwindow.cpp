#include "musicwindow.h"
#include "mylyric.h"
#include "ui_musicwindow.h"
#include "logindialog.h"
#include "downloaddialog.h"
#include "Windows.h"

MusicWindow::MusicWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MusicWindow)
{
    ui->setupUi(this);
    playNum = 1;
    last_PlayNum = 0;
    search_Last = 0;
    player = new QMediaPlayer;
    lyric = new MyLyric(this);
    playMode = "order";
    ui->volumeSlider->setValue(player->volume());
    thread = new Thread(this); //线程实例化
    //窗口初始化
    setElement_BackGround();

    //检测音乐播放进度，同时调整播放滑块
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(updateTime(qint64))); 
    connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(setPlayRange(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(setPlayPosition(qint64)));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(lyric_Sync(QMediaPlayer::State)));

    //调整播放进程
    connect(ui->timeSlider, SIGNAL(sliderMoved(int)), this, SLOT(setSliderPosition()));
    connect(ui->volumeSlider, SIGNAL(sliderMoved(int)), this, SLOT(adjustVolume()));
    connect(ui->browserHideButton, SIGNAL(clicked()), this, SLOT(browserSet()));
    connect(ui->setApearenceButton, SIGNAL(clicked()), this, SLOT(setApearence()));
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(load_again()));
    connect(ui->lyricButton, SIGNAL(clicked()), this, SLOT(show_Hide_Lyric()));
    connect(ui->nextButton, SIGNAL(clicked()), this, SLOT(nextSong()));
    connect(ui->lastButton, SIGNAL(clicked()), this, SLOT(lastSong()));
    connect(ui->playButton, SIGNAL(clicked()), this, SLOT(playEvent()));
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(doubleClickedPlay(QListWidgetItem *)));
    connect(ui->quitButton, SIGNAL(clicked()), this, SLOT(warning()));
    connect(ui->markButton, SIGNAL(clicked()), this, SLOT(markSong()));
    connect(ui->setApearenceButton, SIGNAL(clicked()), this, SLOT(setApearence()));
    connect(ui->muteButton, SIGNAL(clicked()), this, SLOT(set_Mute()));
    connect(ui->storeButton, SIGNAL(clicked()), this, SLOT(store_Show()));
    connect(ui->favoriteButton, SIGNAL(clicked()), this, SLOT(favoriteSongs_Show()));
    connect(ui->browserButton, SIGNAL(clicked()), this, SLOT(browser_Show()));
    connect(ui->loginButton, SIGNAL(clicked()), this, SLOT(user_login()));
    connect(ui->downloadButton, SIGNAL(clicked()), this, SLOT(download_File()));
    connect(ui->modeButton, SIGNAL(clicked()), this, SLOT(playMode_Change()));
    connect(ui->smallButton, SIGNAL(clicked()), this, SLOT(showMinimized()));
    connect(ui->searchEdit, SIGNAL(textChanged(QString)), this, SLOT(songs_Search(QString)));

    //子线程
    connect(thread, SIGNAL(upgrade_Signal(int, QStringList, QStringList, QFileInfoList)), this,
            SLOT(upgradeData(int, QStringList, QStringList, QFileInfoList)), Qt::DirectConnection );

    //载入音乐
    musicLoad();
    Sleep(1000);
}

MusicWindow::~MusicWindow()
{
    delete ui;
}

//设置全局界面元素以及部件透明度
void MusicWindow::setElement_BackGround()
{
    //文件夹下面的图片
    QPixmap fit_Button;
    QPixmap small_Button(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/small.png"));
    QPixmap mode_Button(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/order.png"));
    QPixmap play_Button(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/pause1.png"));
    QPixmap last_Button(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/last1.png"));
    QPixmap next_Button(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/next1.png"));
    QPixmap add_Button(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/open.png"));
    QPixmap theme_Button(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/theme1.png"));
    QPixmap mark_Button(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/open.png"));
    QPixmap close_Button(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/close.png"));
    QPixmap browser_Button(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/browser.png"));
    QPixmap login_Button(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/login.png"));
    QPixmap speaker_Button(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/speaker.png"));
    QPixmap download_Button(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/download.png"));

    //设置表格背景、初始化
    titlePalette.setColor(QPalette::Base, Qt::gray);

    ui->storeView->setPalette(titlePalette);
    ui->favoriteView->setPalette(titlePalette);
    ui->storeView->setStyleSheet("background-color:qconicalgradient(cx:0.5, cy:0.5, angle:0, stop:0.368182 rgba(198,198,198,75))");
    ui->storeView->hide();
    ui->webView->hide();
    ui->favoriteView->setStyleSheet("background-color:qconicalgradient(cx:0.5, cy:0.5, angle:0, stop:0.368182 rgba(198,198,198,75))");
    ui->favoriteView->hide();
    ui->browserButton->hide();
    ui->storeButton->hide();
    ui->favoriteButton->hide();

    //设置界面按钮的图片以及图标大小
    fit_Button = small_Button.scaled(163,163).scaled(21, 21, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->smallButton->setIcon(QIcon(fit_Button));
    ui->smallButton->setIconSize(QSize(21, 21));
    ui->smallButton->setFlat(true);
    ui->smallButton->setStyleSheet("border: 0px");
    ui->smallButton->setToolTip(QString("最小化窗口"));

    fit_Button = mode_Button.scaled(163,163).scaled(31, 31, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->modeButton->setIcon(QIcon(fit_Button));
    ui->modeButton->setIconSize(QSize(31, 31));
    ui->modeButton->setFlat(true);
    ui->modeButton->setStyleSheet("border: 0px");
    ui->modeButton->setToolTip(QString("顺序播放"));

    fit_Button = play_Button.scaled(163,163).scaled(41, 41, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->playButton->setIcon(QIcon(fit_Button));
    ui->playButton->setIconSize(QSize(41, 41));
    ui->playButton->setFlat(true);
    ui->playButton->setStyleSheet("border: 0px");
    ui->playButton->setToolTip(QString("播放/暂停"));

    fit_Button = last_Button.scaled(163,163).scaled(41, 41, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->lastButton->setIcon(QIcon(fit_Button));
    ui->lastButton->setIconSize(QSize(41, 41));
    ui->lastButton->setFlat(true);
    ui->lastButton->setStyleSheet("border: 0px");
    ui->lastButton->setToolTip(QString("上一曲Ctrl+L"));

    fit_Button = next_Button.scaled(163,163).scaled(41, 41, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->nextButton->setIcon(QIcon(fit_Button));
    ui->nextButton->setIconSize(QSize(41, 41));
    ui->nextButton->setFlat(true);
    ui->nextButton->setStyleSheet("border: 0px");
    ui->nextButton->setToolTip(QString("下一曲Ctrl+R"));

    fit_Button = add_Button.scaled(163,163).scaled(36, 36, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->addButton->setIcon(QIcon(fit_Button));
    ui->addButton->setIconSize(QSize(36, 36));
    ui->addButton->setFlat(true);
    ui->addButton->setStyleSheet("border: 0px");
    ui->addButton->setToolTip(QString("add songs"));

    fit_Button = theme_Button.scaled(163,163).scaled(36, 36, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->setApearenceButton->setIcon(QIcon(fit_Button));
    ui->setApearenceButton->setIconSize(QSize(36, 36));
    ui->setApearenceButton->setFlat(true);
    ui->setApearenceButton->setStyleSheet("border: 0px");
    ui->setApearenceButton->setToolTip(QString("更换主题"));

    fit_Button = mark_Button.scaled(163,163).scaled(27, 27, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->markButton->setIcon(QIcon(fit_Button));
    ui->markButton->setIconSize(QSize(27, 27));
    ui->markButton->setFlat(true);
    ui->markButton->setStyleSheet("border: 0px");
    ui->markButton->setToolTip(QString("move to favorite"));

    fit_Button = close_Button.scaled(163,163).scaled(29, 29, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->quitButton->setIcon(QIcon(fit_Button));
    ui->quitButton->setIconSize(QSize(29, 29));
    ui->quitButton->setFlat(true);
    ui->quitButton->setStyleSheet("border: 0px");
    ui->quitButton->setToolTip(QString("close "));

    fit_Button = browser_Button.scaled(163,163).scaled(34, 34, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->browserHideButton->setIcon(QIcon(fit_Button));
    ui->browserHideButton->setIconSize(QSize(34, 34));
    ui->browserHideButton->setFlat(true);
    ui->browserHideButton->setStyleSheet("border: 0px");
    ui->browserHideButton->setToolTip(QString("浏览网页"));

    fit_Button = login_Button.scaled(163,163).scaled(39, 30, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->loginButton->setIcon(QIcon(fit_Button));
    ui->loginButton->setIconSize(QSize(39, 30));
    ui->loginButton->setFlat(true);
    ui->loginButton->setStyleSheet("border: 0px");
    ui->loginButton->setToolTip(QString("登陆"));

    fit_Button = speaker_Button.scaled(163,163).scaled(35, 35,  Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->muteButton->setIcon(QIcon(fit_Button));
    ui->muteButton->setIconSize(QSize(35, 35));
    ui->muteButton->setFlat(true);
    ui->muteButton->setStyleSheet("border: 0px");
    ui->muteButton->setToolTip(QString("静音"));

    fit_Button = download_Button.scaled(163,163).scaled(25,25,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    ui->downloadButton->setIcon(QIcon(fit_Button));
    ui->downloadButton->setIconSize(QSize(25,25));
    ui->downloadButton->setFlat(true);
    ui->downloadButton->setStyleSheet("border:0px");
    ui->downloadButton->setToolTip(QString(" 下载"));

    ui->lyricButton->setToolTip(QString("歌词"));
    titlePalette = ui->browserButton->palette();
    titlePalette.setColor(QPalette::ButtonText, Qt::white);

    ui->browserButton->setToolTip(QString("网络"));
    ui->browserButton->setPalette(titlePalette);
    ui->browserButton->setStyleSheet("background-color:qconicalgradient(cx:0.5, cy:0.5, angle:0, stop:0.368182 rgba(198,198,198,75))");

    ui->storeButton->setToolTip(QString("mosic store"));
    ui->storeButton->setPalette(titlePalette);
    ui->storeButton->setStyleSheet("background-color:qconicalgradient(cx:0.5, cy:0.5, angle:0, stop:0.368182 rgba(198,198,198,75))");
    ui->favoriteButton->setToolTip(QString("我喜欢的歌曲"));
    ui->favoriteButton->setPalette(titlePalette);
    ui->favoriteButton->setStyleSheet("background-color:qconicalgradient(cx:0.5, cy:0.5, angle:0, stop:0.368182 rgba(198,198,198,75))");

    titlePalette = ui->musicTitle->palette();
    titlePalette.setColor(QPalette::WindowText, Qt::white);
    ui->musicTitle->setPalette(titlePalette);
    ui->lyricButton->setStyleSheet("background-color:qconicalgradient(cx:0.5, cy:0.5, angle:0, stop:0.368182 rgba(198,198,198,75))");
    ui->volumeSlider->setStyleSheet("background-color:qconicalgradient(cx:0.5, cy:0.5, angle:0, stop:0.368182 rgba(198,198,198,75))");
    ui->volumeSlider->setTickPosition(QSlider::TicksRight);

    titlePalette = ui->label->palette();
    titlePalette.setColor(QPalette::WindowText, Qt::white);
    ui->label->setPalette(titlePalette);

    titlePalette = ui->playStatusText->palette();
    titlePalette.setColor(QPalette::WindowText, Qt::white);
    ui->playStatusText->setPalette(titlePalette);

    titlePalette = ui->listWidget->palette();
    titlePalette.setColor(QPalette::Base, Qt::lightGray);
    ui->listWidget->setPalette(titlePalette);
    //StyleSheet字符串用来设置模版样式
    ui->listWidget->setStyleSheet("background-color:qconicalgradient(cx:0.5, cy:0.5, angle:0, stop:0.368182 rgba(198,198,198,75))");

    //设置主窗口
    this->setWindowOpacity(0.9);
    this->setWindowFlags(Qt::FramelessWindowHint); //无边框窗口
    this->setWindowTitle("Qt MusicPlayer");
    this->setWindowIcon(QIcon("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/title.png"));
    themeChoise = "pure_zise.png";
    themeToUse = "C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Backgroud/" + themeChoise;
    QPixmap pixmap(/*QString::fromUtf8*/themeToUse);//文件夹下面的图片
    titlePalette = this->palette();
    titlePalette.setBrush(backgroundRole(), QBrush(pixmap));
    this->setPalette(titlePalette);

    //设置全局快捷键动作
    search = new QAction( QString("搜索歌曲"),this);
    search->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));
    search->setIcon(QIcon("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/search.png"));
    this->addAction(search);
    ui->searchEdit->hide();
    this->connect(search, SIGNAL(triggered()), this, SLOT(songsSearch()));

    last_song = new QAction(ui->lastButton);
    last_song->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Left));
    ui->lastButton->addAction(last_song);
    ui->lastButton->connect(last_song, SIGNAL(triggered()), this, SLOT(lastSong()));

    next_song = new QAction(ui->nextButton);
    next_song->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Right));
    ui->nextButton->addAction(next_song);
    ui->nextButton->connect(next_song, SIGNAL(triggered()), this, SLOT(nextSong()));

    pause_song = new QAction(ui->playButton);
    pause_song->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Space));
    ui->playButton->addAction(pause_song);
    ui->playButton->connect(pause_song, SIGNAL(triggered()), this, SLOT(playEvent()));

    small_window = new QAction(this);
    small_window->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));
    this->addAction(small_window);
    this->connect(small_window, SIGNAL(triggered()), this, SLOT(showMinimized()));

    full_window = new QAction(ui->browserHideButton);
    full_window->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));
    ui->browserHideButton->addAction(full_window);
    ui->browserHideButton->connect(full_window, SIGNAL(triggered()), this, SLOT(browserSet()));

    showMini = new QAction( QString("最小化"),this);
    showMini->setIcon(QIcon("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/mini.png"));
    connect(showMini, SIGNAL(triggered()), this, SLOT(showMinimized()));

    quit = new QAction(QString("close"), this);
    quit->setIcon(QIcon("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/close.png"));
    connect(quit, SIGNAL(triggered()), this, SLOT(warning()));

    login = new QAction(QString("登陆"), this);
    login->setIcon(QIcon("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/login.png"));
    connect(login, SIGNAL(triggered()), this, SLOT(user_login()));

    clearList  = new QAction(QString("清空列表"), this);
    clearList->setIcon(QIcon("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/clear.png"));
    connect(clearList, SIGNAL(triggered()), this, SLOT(clear_List()));

    download = new QAction(QString("下载歌曲"), this);
    download->setIcon(QIcon("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/download.png"));
    connect(download,SIGNAL(triggered()), this, SLOT(download_File()));

    information = new QAction(QString("关于"), this);
    information->setIcon(QIcon("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/information.png"));
    connect(information, SIGNAL(triggered()), this, SLOT(about()));
}


void MusicWindow::download_File()
{
    DownLoadDialog download;
     download.exec();
}

void MusicWindow::playMode_Change()
{
    QPixmap fit_Button;

    if(playMode == "order")
    {
        playMode = "shuffle";
        qDebug() << playMode;
        QPixmap mode_Button(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/shuffle.png"));
        fit_Button = mode_Button.scaled(163,163).scaled(31, 31, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        ui->modeButton->setIcon(QIcon(fit_Button));
        ui->modeButton->setIconSize(QSize(31, 31));
        ui->modeButton->setFlat(true);
        ui->modeButton->setStyleSheet("border: 0px");
        ui->modeButton->setToolTip(QString("随机播放"));
    }

    else
    {
        if(playMode == "shuffle")
         {
             playMode = "order";
             qDebug() << playMode;
             QPixmap mode_Button(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/order.png"));
             fit_Button = mode_Button.scaled(163,163).scaled(31, 31, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
             ui->modeButton->setIcon(QIcon(fit_Button));
             ui->modeButton->setIconSize(QSize(31, 31));
             ui->modeButton->setFlat(true);
             ui->modeButton->setStyleSheet("border: 0px");
             ui->modeButton->setToolTip(QString("顺序播放"));
         }
    }
}

void MusicWindow::upgradeData(int MaxNum, QStringList songList, QStringList name, QFileInfoList list)
{

    this->MaxNum = MaxNum;//最大歌曲
    this->songList = songList;//地址信息和mp3文件名链表
    this->name = name;//文件名链表

    if(this->list != list)
    {
        this->list = list;
        int j = 0;
        for(int i = 0; i< this->list.size();++i)
        {
            if(this->list.at(i).fileName().right(3) == "mp3")
            {
                ui->listWidget->addItem(this->list.at(i).fileName());
                ui->listWidget->item(j)->setToolTip(this->list.at(i).fileName());
                j++;
            }
        }
    }

}

//启动线程载入歌曲
void MusicWindow::musicLoad()
{
    thread->start();
}

void MusicWindow::clear_List()
{
    for( int i = 0; ui->listWidget->item(i)!= NULL; i)
    {
        ui->listWidget->takeItem(i);
    }
    player->stop();
}

void MusicWindow::load_again()
{
    int j = 0;
    for(int i = 0; i< this->list.size();++i)
    {
        if(this->list.at(i).fileName().right(3) == "mp3")
        {
            ui->listWidget->addItem(this->list.at(i).fileName());
            ui->listWidget->item(j)->setToolTip(this->list.at(i).fileName());
            j++;
        }
    }
}

//打开搜索框
void MusicWindow::songsSearch()
{
    if(ui->searchEdit->isHidden())
    {
        ui->searchEdit->show();
    }
    else
    {
        ui->searchEdit->hide();
        return;
    }
}

//搜索列表歌曲
void MusicWindow::songs_Search(QString text)
{
    ui->listWidget->item(search_Last)->setTextColor(Qt::black);
    if(text.length() >= 2)
    {
        int row = 0;
        for(row ; ui->listWidget->item(row) != NULL; row++)
        {
            if(ui->listWidget->item(row)->text().contains(text))
            {
                ui->listWidget->setCurrentRow(row);
                search_Last = row;
                ui->listWidget->item(row)->setTextColor(Qt::red);
                break;
             }
         }
     }
}


//标记喜欢的歌曲
void MusicWindow::markSong()
{
    QString loginName = "1";
    QString loginName2 = "2";
    QString markedSong = ui->musicTitle->text();
    int mark_Number = 0;
    QSqlQuery query1;

    QFile file("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/fiiles/user.txt");
    if(!file.open(QFile::ReadOnly | QFile::Text)) //只读打开
    {
        QMessageBox::critical(0, tr("Tips"), tr("Local user file can not OPEN!"));
        return;
    }
    QTextStream in(&file);
    loginName = in.readLine();
    file.close();

    QFile file_previous("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/fiiles/user_previous.txt");
    if(!file_previous.open(QFile::ReadOnly | QFile::Text)) //只读打开
    {
        QMessageBox::critical(0, tr("Tips"), tr("Local user file can not OPEN!"));
        return;
    }
    QTextStream in_previous(&file_previous);
    loginName2 = in_previous.readLine();
    file_previous.close();

    if((loginName == loginName2) &(loginName != ""))
    {
        LoginName = loginName;
    }
    else
    {
        QMessageBox::information(0, tr("Tips"), tr("You can mark this song as your favorite after you have logined!"));
        return;
    }
    qDebug() << query1.lastError(); //查询错误的语句
    //判断是否已经标记了歌曲
    query1.exec(QString("select * from user_%1 where marked_songs = '%2'").arg(LoginName).arg(markedSong));
    query1.first();
    qDebug() << query1.lastError(); //查询错误的语句
    if(!query1.isValid())
    {
        QPixmap fit_Button;
        QPixmap mark_Button(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/mark.png"));
        fit_Button = mark_Button.scaled(163,163).scaled(31, 31, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        ui->markButton->setIcon(QIcon(fit_Button));
        ui->markButton->setIconSize(QSize(31, 31));
        ui->markButton->setFlat(true);
        ui->markButton->setStyleSheet("border: 0px");
    }
    else
    {
        QMessageBox::information(0, tr("Tips"), tr("You have marked this song before, Not Again"));
        return;
    }
    qDebug() << query1.lastError(); //查询错误的语句
    //更新数据库
    query1.exec(QString("select * from music_songs where songName = '%1'").arg(markedSong));
    query1.first();
    qDebug() << query1.lastError(); //查询错误的语句
    mark_Number = query1.value(1).toInt();
    mark_Number += 1;
    query1.exec(QString("update music_songs set marked_number =  %1  where songName = '%2' ").arg(mark_Number).arg(markedSong));

    qDebug() << query1.lastError(); //查询错误的语句
    query1.exec(QString("select * from user_%1 where marked_songs = '%2'").arg(LoginName).arg(markedSong));
    query1.first();
    qDebug() << query1.lastError(); //查询错误的语句
    if(!query1.isValid())
    {
        qDebug() << query1.lastError(); //查询错误的语句
        query1.exec(QString("select * from user_%1 ").arg(LoginName));
        query1.first();
        query1.exec(QString("insert into user_%1 values('%1', %2)").arg(markedSong).arg(mark_Number));
    }
    else
    {
        qDebug() << query1.lastError(); //查询错误的语句
        query1.exec(QString("update user_%1 set mark_number = %2  where mark_songs = '%3' ").arg(LoginName).arg(mark_Number).arg(markedSong));
    }
}

//设置静音
void MusicWindow::set_Mute()
{
    QPixmap fit_Button;
    if(!player->isMuted())
    {
        player->setMuted(true);
        QPixmap speaker_Button(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/mute.png"));
        fit_Button = speaker_Button.scaled(163,163).scaled(35, 35, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        ui->muteButton->setIcon(QIcon(fit_Button));
        ui->muteButton->setIconSize(QSize(35, 35));
        ui->muteButton->setFlat(true);
        ui->muteButton->setStyleSheet("border: 0px");
    }
    else
    {
        player->setMuted(false);
        QPixmap speaker_Button(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/speaker.png"));
        fit_Button = speaker_Button.scaled(163,163).scaled(35, 35, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        ui->muteButton->setIcon(QIcon(fit_Button));
        ui->muteButton->setIconSize(QSize(35, 35));
        ui->muteButton->setFlat(true);
        ui->muteButton->setStyleSheet("border: 0px");
    }
}

//在线用户登录
void MusicWindow::user_login()
{
    LoginDialog login;
    login.exec();
}

//点击左侧浏览器图标
void MusicWindow::browserSet()
{
     ui->webView->load(QUrl("http://www.sou360.com/")); //加载网页
    if(!ui->browserButton->isHidden())
    {
        ui->webView->hide();
        ui->storeView->hide();
        ui->favoriteView->hide();
        ui->browserButton->hide();
        ui->favoriteButton->hide();
        ui->storeButton->hide();
        this->resize(380, 657);
    }
    else
    {
        ui->webView->show();
        ui->browserButton->show();
        ui->favoriteButton->show();
        ui->storeButton->show();
        this->resize(1136, 657);
    }
}

//点击浏览器按钮
void MusicWindow::browser_Show()
{
    titlePalette = ui->browserButton->palette();
    titlePalette.setColor(QPalette::ButtonText, Qt::red);
    ui->browserButton->setPalette(titlePalette);
    titlePalette.setColor(QPalette::ButtonText,Qt::white);
    ui->storeButton->setPalette(titlePalette);
    ui->favoriteButton->setPalette(titlePalette);

    ui->webView->load(QUrl("http://www.sou360.com/")); //加载网页
     if(ui->webView->isHidden())
     {
         ui->storeView->hide();
         ui->favoriteView->hide();
         ui->webView->show();
     }
     else
         ui->webView->show();

}

//点击音乐库按钮
void MusicWindow::store_Show()
{
    QSqlQueryModel *model = new QSqlQueryModel;

    model->setQuery("select * from music_songs");
    model->setHeaderData(0, Qt::Horizontal, tr("The name of songs"));
    model->setHeaderData(1, Qt::Horizontal, tr("The number of mark"));
    ui->storeView->setModel(model);
    ui->storeView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter); //表头居中
    ui->storeView->setColumnWidth(0, 355);//设置固定列宽
    ui->storeView->setColumnWidth(1, 355);

    titlePalette = ui->storeButton->palette();
    titlePalette.setColor(QPalette::ButtonText, Qt::red);
    ui->storeButton->setPalette(titlePalette);
    titlePalette.setColor(QPalette::ButtonText,Qt::white);
    ui->browserButton->setPalette(titlePalette);
    ui->favoriteButton->setPalette(titlePalette);

    ui->webView->hide();
    ui->favoriteView->hide();
    ui->storeView->show();

}

//点击喜爱歌曲按钮
void MusicWindow::favoriteSongs_Show()
{
    QString loginName = "1";
    QString loginName2 = "2";
    QFile file("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/fiiles/user.txt");
    QSqlQueryModel *model = new QSqlQueryModel;

    if(!file.open(QFile::ReadOnly | QFile::Text)) //只读打开
    {
        QMessageBox::critical(0, tr("Tips"), tr("Local user file can not OPEN!"));
        return;
    }
    QTextStream in(&file);
    loginName = in.readLine();
    file.close();

    QFile file_previous("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/fiiles/user_previous.txt");
    QTextStream in_previous(&file_previous);
    if(!file_previous.open(QFile::ReadOnly | QFile::Text)) //只读打开
    {
        QMessageBox::critical(0, tr("Tips"), tr("Local user file can not OPEN!"));
        return;
    }

    loginName2 = in_previous.readLine();
    file_previous.close();

    if((loginName == loginName2) & (! loginName.isEmpty()))
    {
        LoginName = loginName;
    }
    else
    {
        QMessageBox::information(0, tr("Tips"), tr("You can check your favorite songs after you have logined!"));
        return;
    }

    model->setQuery(QString("select * from user_%1").arg(LoginName));
    model->setHeaderData(0, Qt::Horizontal, tr("The name of songs"));
    model->setHeaderData(1, Qt::Horizontal, tr("The number of mark"));
    ui->favoriteView->setModel(model);
    ui->favoriteView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter); //表头居中
    ui->favoriteView->setColumnWidth(0, 355);//设置固定列宽
    ui->favoriteView->setColumnWidth(1, 355);

    titlePalette = ui->favoriteButton->palette();
    titlePalette.setColor(QPalette::ButtonText, Qt::red);
    ui->favoriteButton->setPalette(titlePalette);
    titlePalette.setColor(QPalette::ButtonText,Qt::white);
    ui->storeButton->setPalette(titlePalette);
    ui->browserButton->setPalette(titlePalette);

    ui->webView->hide();
    ui->storeView->hide();
    ui->favoriteView->show();
}

//动态调整窗口控件timeSlider的位置
void MusicWindow::setPlayRange(qint64 position)
{
    ui->timeSlider->setRange(0, position);
}

//调整滑块位置
void MusicWindow::setPlayPosition(qint64 position)
{
    ui->timeSlider->setSliderPosition(position);
}

//通过滑块进行音量调节
void MusicWindow::adjustVolume()
{
    player->setVolume(ui->volumeSlider->value());
    if(ui->volumeSlider->sliderPosition() == 0)
    {
        player->setMuted(true);
        QPixmap fit_Button;
        QPixmap speaker_Button(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/mute.png"));
        fit_Button = speaker_Button.scaled(163,163).scaled(35, 35, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        ui->muteButton->setIcon(QIcon(fit_Button));
        ui->muteButton->setIconSize(QSize(35, 35));
        ui->muteButton->setFlat(true);
        ui->muteButton->setStyleSheet("border: 0px");

    }
    else
    {
        player->setMuted(false);
        QPixmap fit_Button;
        QPixmap speaker_Button(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/speaker.png"));
        fit_Button = speaker_Button.scaled(163,163).scaled(35, 35, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        ui->muteButton->setIcon(QIcon(fit_Button));
        ui->muteButton->setIconSize(QSize(35, 35));
        ui->muteButton->setFlat(true);
        ui->muteButton->setStyleSheet("border: 0px");
    }

}

//通过滑块调整播放进程
void MusicWindow::setSliderPosition()
{
    player->setPosition(ui->timeSlider->sliderPosition());
}

//全局背景图片设置
void MusicWindow::setApearence()
{
    while(1)
    {
        QPixmapCache::clear();
        if(themeChoise.compare("pure_zise.png") == 0)
         {
             themeChoise = "full_blue.png";
             themeToUse = "C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Backgroud/" + themeChoise;
             break;
         }
        if(themeChoise.compare("full_blue.png") == 0)
         {
             themeChoise = "full_glass.png";
             themeToUse = "C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Backgroud/" + themeChoise;
             break;
         }
         if(themeChoise.compare("full_glass.png") == 0)
         {
             themeChoise = "full_glass2.png";
             themeToUse = "C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Backgroud/" + themeChoise;
             break;
         }
         if(themeChoise.compare("full_glass2.png") == 0)
         {
             themeChoise = "full_green.png";
             themeToUse = "C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Backgroud/" + themeChoise;
             break;
         }
         if(themeChoise.compare("full_green.png") == 0)
         {
             themeChoise = "full_sea.png";
             themeToUse = "C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Backgroud/" + themeChoise;
             break;
         }
         if(themeChoise.compare("full_sea.png") == 0)
         {
             themeChoise = "pure_zise.png";
             themeToUse = "C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Backgroud/" + themeChoise;
             break;
         }
         break;
    }
    QPixmapCache::clear();
    QPixmap pixmap(/*QString::fromUtf8(*/themeToUse);//文件夹下面的图片
    QPalette palette = this->palette();//获取当前面板
    palette.setBrush(backgroundRole(), QBrush(pixmap));
    setPalette(palette);

}

//播放下一曲目
void MusicWindow::nextSong()
{
    if(ui->listWidget->item(0) == NULL)
    {
        return;
    }
    ui->listWidget->item(last_PlayNum)->setTextColor(Qt::black);
    lyric->stop_Lyric_Mask();

    if(playMode == "order")
    {
        if(MaxNum >= 0)
        {
            if(MaxNum == playNum)
            {
                playNum = 0;
                last_PlayNum = playNum;
                ui->listWidget->setCurrentRow(playNum);
                ui->musicTitle->setText(ui->listWidget->item(playNum)->text());
                ui->listWidget->item(playNum)->setTextColor(Qt::white);
                player->setMedia(QMediaContent(QUrl(songList.at(playNum)))); //读取目录地址，目录存在链表中
                player->play();
            }
            else
            {
                playNum += 1;
                last_PlayNum = playNum;
                ui->listWidget->setCurrentRow(playNum);
                ui->musicTitle->setText(ui->listWidget->item(playNum)->text());
                ui->listWidget->item(playNum)->setTextColor(Qt::white);
                player->setMedia(QMediaContent(QUrl(songList.at(playNum))));
                player->play();
            }
        }
        else
           return;
    }
    else
    {
        int i = 0;
        if(MaxNum > 0)
        {
            srand((unsigned) time(0));
            i = rand() % (MaxNum + 1);
        }
        playNum = i;
        last_PlayNum = playNum;
        ui->listWidget->setCurrentRow(playNum);
        ui->musicTitle->setText(ui->listWidget->item(playNum)->text());
        ui->listWidget->item(playNum)->setTextColor(Qt::white);
        player->setMedia(QMediaContent(QUrl(songList.at(playNum))));
        player->play();

    }

}

//播放上一曲目
void MusicWindow::lastSong()
{
    if(ui->listWidget->item(0) == NULL)
    {
        return;
    }
    lyric->stop_Lyric_Mask();
    ui->listWidget->item(last_PlayNum)->setTextColor(Qt::black);
    if(playMode == "order")
    {
        if(MaxNum >= 0)
        {
            if(playNum == 0)
            {
                playNum = MaxNum;
                last_PlayNum = playNum;
                ui->listWidget->setCurrentRow(playNum);
                ui->musicTitle->setText(ui->listWidget->item(playNum)->text());
                ui->listWidget->item(playNum)->setTextColor(Qt::white);
                player->setMedia(QMediaContent(QUrl(songList.at(playNum))));
                player->play();

            }
            else
            {
                playNum -= 1;
                last_PlayNum = playNum;
                ui->listWidget->setCurrentRow(playNum);
                ui->musicTitle->setText(ui->listWidget->item(playNum)->text());
                ui->listWidget->item(playNum)->setTextColor(Qt::white);
                player->setMedia(QMediaContent(QUrl(songList.at(playNum))));
                player->play();
            }
        }
    }
    else
    {
        int i = 0;
        if(MaxNum > 0)
        {
            srand((unsigned) time(0));
            i = rand() % (MaxNum + 1);
        }
        playNum = i;
        last_PlayNum = playNum;
        ui->listWidget->setCurrentRow(playNum);
        ui->musicTitle->setText(ui->listWidget->item(playNum)->text());
        ui->listWidget->item(playNum)->setTextColor(Qt::white);
        player->setMedia(QMediaContent(QUrl(songList.at(playNum))));
        player->play();
    }

}

//弹出退出警告信息
void MusicWindow::warning()
{
    QMessageBox warningBox;
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312")); //防止乱码
    warningBox.setText(tr("退出后保留后台播放吗？"));
    warningBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    warningBox.setDefaultButton(QMessageBox::Yes);
    int msg = warningBox.exec();
    switch (msg) {
    case QMessageBox::Yes:
        this->showMinimized();
        break;
    case QMessageBox::No:
        this->close();
        exit(0);
        break;
    }
}

//双击播放列表歌曲名播放事件
void MusicWindow::doubleClickedPlay(QListWidgetItem *a)
{
    int x = ui->listWidget->row(a);
    playNum = x;
    if(playNum != last_PlayNum)
    {
        ui->listWidget->item(last_PlayNum)->setTextColor(Qt::black);
    }
    last_PlayNum = playNum;
    player->setMedia(QMediaContent(QUrl(songList.at(x))));
    player->play();
    ui->musicTitle->setText(ui->listWidget->item(playNum)->text());
    ui->listWidget->item(playNum)->setTextColor(Qt::white);
    ui->playStatusText->setText("Playing");

}

//解析歌词的函数
void MusicWindow::resolve_lrc(const QString &source_File_Name)
{
    lrc_Map.clear();
    if(source_File_Name.isEmpty())
    {
        return;
    }
    QString file_Name = source_File_Name;
    //更改音频文件的后缀为lrc
    QString lrc_File_Name = "C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/MusicResources/" + file_Name.remove(file_Name.right(3)) + "lrc";

    QFile file(lrc_File_Name);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream lyricText(&file);  //文本流不会乱码
    QString lyric_Text = lyricText.readAll();
    file.close();
    //将歌词按行分解为歌词列表
    QStringList lyricLines = lyric_Text.split("\n");

    //这个是时间标签的格式[00:05.54]
    //正则表达式d{2}表示匹配2个数字
    QRegExp rx("\\[\\d{2}:\\d{2}\\.\\d{2}\\]");

    //foreach 语句为数组或对象集合中的每个元素重复一个嵌入语句组。
    //foreach 语句用于循环访问集合以获取所需信息，但不应用于更改集合内容以避免产生不可预知的副作用
    foreach(QString oneLine, lyricLines)
    {
        QString temp = oneLine;
        temp.replace(rx, "");
        //用空字符串替换正则表达式中所匹配的地方,这样就去掉了歌词的时间标签，获得了歌词文本

        // 然后依次获取当前行中的所有时间标签，并分别与歌词文本存入QMap中
        //indexIn()为返回第一个匹配的位置，如果返回为-1，则表示没有匹配成功
        //正常情况下pos后面应该对应的是歌词文件
        int pos = rx.indexIn(oneLine, 0);
        while(pos != -1)
        {
            QString cap = rx.cap(0);  //返回第零个表达式匹配的内容
            //将时间标签转化为时间数值，毫秒为单位
            QRegExp regexp;
            regexp.setPattern("\\d{2}(?=:)");
            regexp.indexIn(cap);
            int minute = regexp.cap(0). toInt();
            regexp.setPattern("\\d{2}(?=\\.)");
            regexp.indexIn(cap);
            int second = regexp.cap(0).toInt();
            regexp.setPattern("\\d{2}(?=\\])");
            regexp.indexIn(cap);
            int millisecond = regexp.cap(0).toInt();
            qint64 totalTime = minute * 60000 + second * 1000 +
                    millisecond * 10;
            lrc_Map.insert(totalTime, temp);
            pos += rx.matchedLength();
            pos = rx.indexIn(oneLine, pos);//匹配全部
        }
    }

    if(lrc_Map.empty())
    {
        lyric->setText("找不到歌词文件!");

        return;
    }
}


//更新界面时间显示的模块
void MusicWindow::updateTime(qint64 time)
{
    ui->timeSlider->setValue(player->duration());
    ui->timeSlider->setSliderPosition(time);
    qint64 total_Time_value = player->duration();  //获取歌曲总时间
    QTime total_Time(0, (total_Time_value / 60000) % 60,
                     (total_Time_value / 1000) % 60);
    QTime current_Time(0, (time / 60000) % 60,
                       (time / 1000) % 60);
    QString str = current_Time.toString("mm:ss                                ") + total_Time.toString("mm:ss");
    ui->label->setText(str);
    //获取即时的歌词
    if(!lrc_Map.isEmpty())
    {
        qint64 previous = 0;
        qint64 later = 0;
        foreach (qint64 value, lrc_Map.keys())
        {
            if(time >= value)
                previous = value;
            else
            {
                later = value;
                break;
            }

        }
        //达到最后一行将later设置成歌曲总时间
        if(later == 0)
            later = total_Time_value;
        QString current_Lyric = lrc_Map.value(previous);
        //新的一行歌词重新显示歌词的遮蔽mask
        if(current_Lyric != lyric->text())
        {
            lyric->setText(current_Lyric);
            qint64 interval_Time = later - previous;
            lyric->start_Lyric_Mask(interval_Time);
        }
    }else
    {
        lyric->setText(tr("没有歌词文件!"));
    }

    //判断是否播放完，自动播放下一曲
    QTime maxtime(0, (player->duration() / 60000) % 60,
                  ((player->duration() / 1000) % 60 ));
    QTime null(0, 0, 0);

    if((time == total_Time_value) & (maxtime != null))
    {
        nextSong();
    }

}

//歌词同步
void MusicWindow::lyric_Sync(QMediaPlayer::State newState)
{
    QPixmap fit_Button;
    switch(newState)
    {
        case QMediaPlayer::PlayingState:{
            ui->playStatusText->setText(tr("Playing"));
            resolve_lrc(ui->listWidget->item(playNum)->text());
            QPixmap play_Button(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/play1.png"));
            fit_Button = play_Button.scaled(163,163).scaled(41, 41, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            ui->playButton->setIcon(QIcon(fit_Button));
            ui->playButton->setIconSize(QSize(41, 41));
            ui->playButton->setFlat(true);
            ui->playButton->setStyleSheet("border: 0px");
            break;
        }

        case QMediaPlayer::PausedState:{
            ui->playStatusText->setText(tr("Pause"));
            if(!lrc_Map.isEmpty())
            {
                lyric->stop_Lyric_Mask();
            }
            QPixmap pause_Button1(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/pause1.png"));
            fit_Button = pause_Button1.scaled(163,163).scaled(41, 41, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            ui->playButton->setIcon(QIcon(fit_Button));
            ui->playButton->setIconSize(QSize(41, 41));
            ui->playButton->setFlat(true);
            ui->playButton->setStyleSheet("border: 0px");
            break;
        }
        case QMediaPlayer::StoppedState:{
            ui->playStatusText->setText(tr("Stop"));
            if(!lrc_Map.isEmpty())
            {
                lyric->stop_Lyric_Mask();
            }
            QPixmap pause_Button1(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/pause1.png"));
            fit_Button = pause_Button1.scaled(163,163).scaled(41, 41, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            ui->playButton->setIcon(QIcon(fit_Button));
            ui->playButton->setIconSize(QSize(41, 41));
            ui->playButton->setFlat(true);
            ui->playButton->setStyleSheet("border: 0px");
            break;
        }
    }
}


//点击播放按钮
void MusicWindow::playEvent()
{
    QPixmap fit_Button;
    if(player->state() == QMediaPlayer::PlayingState)
    {
        player->pause();

        QPixmap pause_Button1(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/pause1.png"));
        fit_Button = pause_Button1.scaled(163,163).scaled(41, 41, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        ui->playButton->setIcon(QIcon(fit_Button));
        ui->playButton->setIconSize(QSize(41, 41));
        ui->playButton->setFlat(true);
        ui->playButton->setStyleSheet("border: 0px");

    }
    else
    {
        if(player->state() == QMediaPlayer::StoppedState)
        {
            return;
        }
        else
        {
            player->play();

            QPixmap play_Button(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/play1.png"));
            fit_Button = play_Button.scaled(163,163).scaled(41, 41, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            ui->playButton->setIcon(QIcon(fit_Button));
            ui->playButton->setIconSize(QSize(41, 41));
            ui->playButton->setFlat(true);
            ui->playButton->setStyleSheet("border: 0px");
        }

    }
}

//歌词显示控制
void MusicWindow::show_Hide_Lyric()
{
    if(lyric->isHidden())
        lyric->show();
    else
    {
        lyric->hide();
    }
}

void MusicWindow::about()
{
    AboutDialog about;
    about.exec();
}

void MusicWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        mouse_Point = event->pos();
        mouse_Press = true;
    }
}

void MusicWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(mouse_Press)
    {
        QPoint move_position = event->globalPos();
        this->move(move_position - mouse_Point);
    }
}

void MusicWindow::mouseReleaseEvent(QMouseEvent *)
{
    mouse_Press = false;
}

//右键弹出下拉菜单的事件
void MusicWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = new QMenu;
    menu->addSeparator();//列表项的横杠
    menu->addSeparator();
    menu->addAction(login);
    menu->addSeparator();
    menu->addSeparator();
    menu->addAction(clearList);
    menu->addSeparator();
    menu->addSeparator();
    menu->addAction(search);
    menu->addSeparator();
    menu->addSeparator();
    menu->addAction(download);
    menu->addSeparator();
    menu->addSeparator();
    menu->addAction(information);
    menu->addSeparator();
    menu->addSeparator();
    menu->addAction(showMini);
    menu->addSeparator();
    menu->addSeparator();
    menu->addAction(quit);
    menu->addSeparator();
    menu->addSeparator();

    menu->exec(event->globalPos());//获取坐标

    delete menu;//关闭窗口
}
