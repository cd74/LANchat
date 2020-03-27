#ifndef DIALOGPRIVATE_H
#define DIALOGPRIVATE_H

#include <QDialog>
#include<QtNetwork>
#include<QUdpSocket>
#include<QtNetwork/QHostInfo>
#include <QFileDialog>
#include"qstring.h"
#include <QFile>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include<qcolordialog.h>

namespace Ui {
class Dialogprivate;
}

class Dialogprivate : public QDialog
{
    Q_OBJECT

public:
    QString oname;
    QString oip;
    QString text1;
    QString rfile,filename;
    int count=1,countf=1;
    explicit Dialogprivate(int a,QWidget *parent = 0);
    ~Dialogprivate();

private slots:
    //void on_pushButton_4_clicked();
    void receive();

    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_comboBox_currentIndexChanged(int index);

    void on_toolButton_3_clicked(bool checked);

    void on_toolButton_clicked(bool checked);

    void on_toolButton_2_clicked(bool checked);

    void on_toolButton_4_clicked(bool checked);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::Dialogprivate *ui;
    QUdpSocket *prisocket;
};

#endif // DIALOGPRIVATE_H
