#ifndef DOWNLOADDIALOG_H
#define DOWNLOADDIALOG_H

#include <QDialog>
#include <QFileInfo>
#include <QUrl>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class DownLoadDialog;
}

class DownLoadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DownLoadDialog(QWidget *parent = 0);
    ~DownLoadDialog();

private:
    Ui::DownLoadDialog *ui;

    QNetworkAccessManager manager;
    QNetworkReply *reply;
    QUrl url;
    QFile *file;

private slots:
    void httpFinished();
    void httpReadyRead();
    void updateProgressBar(qint64, qint64);
    void startRequest(QUrl url);
    void  on_downloadButton_clicked();
};

#endif // DOWNLOADDIALOG_H
