#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QMessageBox>
#include <QDialog>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSql>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDebug>
#include <QPixmap>
#include <QTextCodec>
#include <QTextStream>
#include <QFile>
#include <QIODevice>
#include <QMouseEvent>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
    QString LoginName;   //登录用户名
    QString LoginPassword;   //登陆密码


private:
    Ui::LoginDialog *ui;

    QPalette palette;
    void  initializtion();

private slots:
    void user_login();
    void user_signUp();
    void check();
};

#endif // LOGINDIALOG_H
