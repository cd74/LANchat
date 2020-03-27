#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<QtNetwork>
#include<QUdpSocket>
#include<QTcpSocket>
#include<QtNetwork/QHostInfo>
#include <QFileDialog>
#include"qstring.h"
#include <QFile>
#include <QMessageBox>
#include<qcolordialog.h>
#include<dialogprivate.h>
#include"qtimer.h"

class QUdpsocket;
class QFile;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(int a,QWidget *parent = 0);
    ~Dialog();

private slots:
    //void on_pushButton_4_clicked();
    void receive();

    void readtcp(QTcpSocket *sock);

    void newconnect();

    void disconnect();

    void serverbroad();

    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_comboBox_currentIndexChanged(int index);

    void on_toolButton_3_clicked(bool checked);

    void on_toolButton_clicked(bool checked);

    void on_toolButton_2_clicked(bool checked);

    void on_toolButton_4_clicked(bool checked);

    void on_tableWidget_doubleClicked(const QModelIndex &index);

    void on_pushButton_clicked();
    
    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();


    void on_pushButton_5_clicked();

private:
    Ui::Dialog *ui;
    QString filename;
    QFile *localfile;
    QByteArray sendout;
    QUdpSocket *udpsocket;
    QTcpServer *server;
    QTcpSocket *tcpsocket;
    QString getip();
    QString localname();
    QList<QTcpSocket*> list;
    QTimer *timer;
    int serveron=0,serverconnect=0;
    void send();
    void sendfile();
};

#endif // DIALOG_H
