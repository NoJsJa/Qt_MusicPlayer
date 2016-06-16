#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint); //无边框窗口
    this->setWindowTitle(QString("登陆"));
    ui->sign_up_Button->setDisabled(true);
    ui->okButton->setDisabled(true);
    ui->idEdit->setToolTip(QString("less than 30"));
    ui->passwordEdit->setToolTip(QString("less than 30"));
    ui->sign_up_Button->setToolTip(QString("new user"));
    ui->label->setToolTip(tr("ID"));
    ui->label_2->setToolTip(tr("password"));

    palette = ui->idEdit->palette();
    palette.setColor(QPalette::Base, Qt::lightGray);
    ui->idEdit->setPalette(palette);
    ui->passwordEdit->setPalette(palette);

    palette = ui->label->palette();
    palette.setColor(QPalette::WindowText, Qt::white);
    ui->label->setPalette(palette);

    QPixmap pixmap("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/Images/Backgroud/dialog.png");
    palette.setBrush(backgroundRole(), QBrush(pixmap));
    this->setPalette(palette);

    // 界面信号槽连接
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(user_login()));
    connect(ui->sign_up_Button, SIGNAL(clicked()), this, SLOT(user_signUp()));
    connect(ui->idEdit, SIGNAL(textChanged(QString)), this, SLOT(check()));
    connect(ui->passwordEdit, SIGNAL(textChanged(QString)), this, SLOT(check()));
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::initializtion()
{
    QFile file("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/fiiles/user_previous.txt");
    if(!file.open(QFile::WriteOnly | QFile::Text)) //只写打开
    {
        QMessageBox::critical(0, tr("Tips"), tr("Local user file can not OPEN!"));
        return;
    }
    QTextStream out(&file); // 写入文件的内容

    out << "user" << endl;
    out << "123456" << endl;
    file.close();
}

void LoginDialog::check()
{
    if(!ui->idEdit->text().isEmpty() & !ui->passwordEdit->text().isEmpty())
    {
        ui->sign_up_Button->setEnabled(true);
        ui->okButton->setEnabled(true);
    }
    else
    {
        ui->sign_up_Button->setDisabled(true);
        ui->okButton->setEnabled(true);
    }

}

void LoginDialog::user_login()
{
    QString loginName;
    QString loginPassword;
    QSqlQuery query;

    query.exec("select * from music_user");
    query.first();
    qDebug() << query.lastError(); //查询错误的语句
    initializtion();    //初始化

    loginName = ui->idEdit->text();//临时变量
    loginPassword = ui->passwordEdit->text();

    while(1)
    {
        if(loginName == query.value(0).toString())
        {
            if(query.value(1) == loginPassword)
            {
                QMessageBox::information(0, tr("Tips"), tr("Login success!"));
                LoginName = ui->idEdit->text();
                LoginPassword = ui->passwordEdit->text();

                QFile file("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/fiiles/user.txt");
                 if(!file.open(QFile::WriteOnly | QFile::Text)) //只写打开
                 {
                     QMessageBox::critical(0, tr("Tips"), tr("Local user file can not OPEN!"));
                 }
                 else
                 {
                    QTextStream out(&file); // 写入文件的内容
                    out << LoginName << endl;
                    out << LoginPassword << endl;
                    file.close();
                 }

                QFile file_previous("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/fiiles/user_previous.txt");
                if(!file_previous.open(QFile::WriteOnly | QFile::Text)) //只写打开
                {
                    QMessageBox::critical(0, tr("Tips"), tr("Local user file can not OPEN!"));
                }
                else
                {
                    QTextStream out_previous(&file_previous); // 写入文件的内容

                    out_previous <<  LoginName << endl;
                    out_previous << LoginPassword << endl;
                    file_previous.close();
                }

                query.first();
                break;
            }
            else
            {
                QMessageBox::information(0, tr("Error"), tr("The ID does not match the password!"));
                query.first();
                ui->passwordEdit->clear();
                break;
            }
        }
        else
        {
            if(!query.next())
            {
                query.first();
                QMessageBox::information(0, tr("Error"), tr("No ID information in database"));
                break;
            }

        }
    }
    qDebug() << query.lastError(); //查询错误的语句

}

void LoginDialog::user_signUp()
{
    QString id;
    QString password;
    QSqlQuery query;

    query.exec("select * from music_user");
    query.first();
    qDebug() << query.lastError(); //查询错误的语句
     initializtion();    //初始化

    id = ui->idEdit->text();
    password = ui->passwordEdit->text();

    while(1)
    {
        if(query.value(0).toString() == id)
        {
            QMessageBox::information(0, tr("Error"), tr("The ID already exits"));
            ui->passwordEdit->clear();
            query.first();
            break;
        }
        if(!query.next())
        {
            query.exec(QString("insert into music_user values('%1', '%2')").arg(id).arg(password));
            QMessageBox::information(0, tr("Tips"), tr("Sign up complete!"));
            LoginName = id;
            LoginPassword = password;

            QFile file("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/fiiles/user.txt");
            if(!file.open(QFile::WriteOnly | QFile::Text)) //只写打开
            {
                QMessageBox::critical(0, tr("Tips"), tr("Local user file can not OPEN!"));
            }
            QTextStream out(&file); // 写入文件的内容
            out << LoginName << endl;
            out << LoginPassword << endl;
            file.close();

            QFile file_previous("C:/Qt/Qt5.4.1/projects/QT_MusicPlayer/fiiles/user_previous.txt");
            if(!file_previous.open(QFile::WriteOnly | QFile::Text)) //只写打开
            {
                QMessageBox::critical(0, tr("Tips"), tr("Local user file can not OPEN!"));
            }
            QTextStream out_previous(&file_previous); // 写入文件的内容
            out_previous << LoginName << endl;
            out_previous << LoginPassword << endl;
            file_previous.close();

            query.exec(QString("create table user_%1(marked_songs not null primary keyVARCHAR(30) , mark_number INTEGER not null)").arg(LoginName));
            query.first();
            break;
        }
    }
    qDebug() << query.lastError(); //查询错误的语句

}
