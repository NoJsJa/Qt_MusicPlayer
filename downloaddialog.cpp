#include "downloaddialog.h"
#include "ui_downloaddialog.h"

DownLoadDialog::DownLoadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DownLoadDialog)
{
    ui->setupUi(this);

    ui->progressBar->hide();

    QPalette titlePalette;
    this->setWindowFlags(Qt::FramelessWindowHint); //无边框窗口
    QPixmap pixmap("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Backgroud/download.png");//文件夹下面的图片
    this->resize(QSize(406,129));
    titlePalette = this->palette();
    titlePalette.setBrush(backgroundRole(), QBrush(pixmap));
    this->setPalette(titlePalette);

    ui->statusButton->setStyleSheet("background-color:qconicalgradient(cx:0.5, cy:0.5, angle:0, stop:0.368182 rgba(198,198,198,75))");
    titlePalette = ui->label_title->palette();
    titlePalette.setColor(QPalette::WindowText, Qt::white);
    titlePalette.setColor(QPalette::Base, Qt::lightGray);
    ui->label_url->setPalette(titlePalette);
    ui->label_progress->setPalette(titlePalette);
    ui->label_title->setPalette(titlePalette);

    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->downloadButton, SIGNAL(clicked()), this, SLOT(on_downloadButton_clicked()));
}

DownLoadDialog::~DownLoadDialog()
{
    delete ui;
}

void DownLoadDialog::httpReadyRead()
{
    if(file)
        file->write(reply->readAll());
}

void DownLoadDialog::updateProgressBar(qint64 bytesRead, qint64 totalBytes)
{
    ui->progressBar->setMaximum(totalBytes);
    ui->progressBar->setValue(bytesRead);
}

void DownLoadDialog::httpFinished()
{
    QPixmap fit_Button;
    QPixmap download_Button(QString::fromUtf8("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Icon/download_complete.png"));
    fit_Button = download_Button.scaled(163,163).scaled(31, 31, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->statusButton->setIcon(QIcon(fit_Button));
    ui->statusButton->setIconSize(QSize(31, 31));
    ui->statusButton->setFlat(true);
    ui->statusButton->setStyleSheet("border: 0px");
    ui->statusButton->setToolTip(QString("下载完成"));
    ui->progressBar->hide();
    this->resize(406, 129);

    file->flush();      //用于在文件打开时将数据写入文件，文件在写之前是先写在缓冲之中的
    file->close();
    reply->deleteLater();
    reply = 0;
    delete file;  //清除内存
    file = 0;
}

void DownLoadDialog::startRequest(QUrl url)
{
    reply = manager.get(QNetworkRequest(url));
    connect(reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(updateProgressBar(qint64, qint64)));
    connect(reply, SIGNAL(finished()), this, SLOT(httpFinished()));
}

void DownLoadDialog::on_downloadButton_clicked()
{

    this->resize(QSize(406, 182));
    url = ui->lineEdit_url->text();
    QFileInfo info(url.path());
    QString fileName(info.fileName());

    file = new QFile("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/download/" + fileName);
    if(!file->open(QIODevice::WriteOnly))
    {
        qDebug() << "file open error!" << endl;
        delete file;
        file = 0;
        this->close();
        return;
    }
    startRequest(url);
    ui->progressBar->setValue(0);
    ui->progressBar->show();
}
