#include "dialog.h"
#include "ui_dialog.h"
#include"qmessagebox.h"
#include"qtimer.h"
#include"qdebug.h"

Dialog::Dialog(int z,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    udpsocket=new QUdpSocket();
    tcpsocket=new QTcpSocket();
    udpsocket->bind(QHostAddress::Any,8888,QUdpSocket::ShareAddress);
    if(z==1)
    {
      QByteArray b;
      QDataStream out(&b, QIODevice::WriteOnly);
      QString s1=localname();
      QString s2=getip();
      int a=1;
      out<<a<<s1<<s2;
      udpsocket->writeDatagram(b,b.length(),QHostAddress::Broadcast, 8888);
    }
    connect(udpsocket, SIGNAL(readyRead()), this, SLOT(receive()));
}

Dialog::~Dialog()
{
    QByteArray array;
    QDataStream send(&array, QIODevice::WriteOnly);
    int a=6;

    send<<a<<localname()<<getip();
    udpsocket->writeDatagram(array,array.length(),QHostAddress::Broadcast, 8888);
    delete ui;
}


QString Dialog::localname()
{
    QString s=QHostInfo::localHostName();
    return s;
}

QString Dialog::getip()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
        if(address!=QHostAddress::LocalHost &&address.toIPv4Address())
            return address.toString();
    }
}

void Dialog::newconnect()
{
    QTcpSocket *newsock=server->nextPendingConnection();
    list.append(newsock);
    connect(newsock,SIGNAL(readyRead()),this,SLOT(readtcp(newsock)));
}

void Dialog::disconnect()
{
    serverconnect=0;
    ui->label_5->setText("(Connect to server automatically)");
}

void Dialog::serverbroad()
{
    QByteArray b;
    QDataStream out(&b, QIODevice::WriteOnly);
    int a=7;
    out<<a<<localname()<<getip();
    udpsocket->writeDatagram(b,b.length(),QHostAddress::Broadcast,8888);
}

void::Dialog::readtcp(QTcpSocket *sock)
{
    QByteArray b=sock->readAll(),c;
    QDataStream in(&b, QIODevice::ReadOnly);
    QString name,ip,text,fname,fpath;
    int a;
    in>>a;
    switch(a)
    {
    case 1:
        in>>name>>ip>>text;
        ui->textBrowser->setTextColor(Qt::blue);
        ui->textBrowser->setCurrentFont(QFont("Times New Roman",12));
        ui->textBrowser->append(" " +name+" "+ip+"  ");
        ui->textBrowser->append(text);
        if(serveron)
        {
           foreach(QTcpSocket *sock,list)
              sock->write(b);
        }
    case 2:
        in>>name>>ip>>fname>>c;
        fname=fname.right(fname.size()-fname.lastIndexOf('/')-1);
        fpath="./"+fname;
        QFile file(fpath);
        file.open(QFile::WriteOnly);
        file.write(c);
        ui->textBrowser->setTextColor(Qt::blue);
        ui->textBrowser->setCurrentFont(QFont("Times New Roman",10));
        ui->textBrowser->append(" " +name+" "+ip+"send file:  "+fname);
        if(serveron)
        {
           foreach(QTcpSocket *sock,list)
              sock->write(b);
        }
    }
}

void Dialog::receive()
{
        while(udpsocket->hasPendingDatagrams())
        {
            QByteArray b,fb;
            b.resize(udpsocket->pendingDatagramSize());
            udpsocket->readDatagram(b.data(),b.size());
            QDataStream in(&b, QIODevice::ReadOnly);
            int x;
            in>>x;
            QString name,ip,text,fname,fpath;
            int rowcount;
            switch(x)
            {
            case 7://broadcast of server
                if(!serverconnect&&!serveron)
                {
                    in>>name>>ip;
                    tcpsocket->connectToHost(name,8192);
                    connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(readtcp(tcpsocket)));
                    connect(tcpsocket,SIGNAL(disconnected()),this,SLOT(disconnect()));
                    qDebug()<<"\n  1";
                    serverconnect=1;
                    ui->label_5->setText("server <"+name+"> connected");
                }
                break;
            case 6:
                in>>name>>ip;
                rowcount=ui->tableWidget->findItems(name, Qt::MatchExactly).first()->row();
                ui->tableWidget->removeRow(rowcount);
                ui->textBrowser->setTextColor(Qt::gray);
                ui->textBrowser->setCurrentFont(QFont("Times New Roman", 10));
                ui->textBrowser->append(tr("%1 %2 offline！").arg(name).arg(ip));
                ui->label->setText(tr("在线人数：%1").arg(ui->tableWidget->rowCount()));
                break;
            case 2:
                in>>name>>ip>>text;
                ui->textBrowser->setTextColor(Qt::blue);
                ui->textBrowser->setCurrentFont(QFont("Times New Roman",12));
                ui->textBrowser->append(" " +name+" "+ip+"  ");
                ui->textBrowser->append(text);
                break;
            case 1:
                in>>name>>ip;
                QTableWidgetItem *user = new QTableWidgetItem(name);
                QTableWidgetItem *address = new QTableWidgetItem(ip);
                ui->tableWidget->insertRow(0);
                ui->tableWidget->setItem(0,0,user);
                ui->tableWidget->setItem(0,1,address);
                ui->textBrowser->setTextColor(Qt::green);
                ui->textBrowser->setCurrentFont(QFont("Times New Roman",10));
                ui->textBrowser->append(name+" "+ip+" online");
                ui->label->setText(tr("在线人数：%1").arg(ui->tableWidget->rowCount()));
                break;
            }
            if(x==5)
            {
                in>>name>>ip>>fname>>fb;
                fname=fname.right(fname.size()-fname.lastIndexOf('/')-1);
                fpath="./"+fname;
                QFile file(fpath);
                file.open(QFile::ReadWrite);
                file.write(fb.data(),fb.size());
                Dialogprivate  *privatechat = new Dialogprivate(2,this);
                privatechat->count=0;
                privatechat->oname=name;
                privatechat->oip=ip;
                privatechat->text1=text;
                privatechat->rfile=fname;
                privatechat->show();
            }
             if(x==4)
                 {
                 in>>name>>ip>>text;
                 Dialogprivate  *privatechat = new Dialogprivate(1,this);
                 privatechat->count=0;
                 privatechat->oname=name;
                 privatechat->oip=ip;
                 privatechat->text1=text;
                 privatechat->show();          
                 }
             if(x==3)
             {
                 in>>name>>ip>>fname>>fb;
                 fname=fname.right(fname.size()-fname.lastIndexOf('/')-1);
                 fpath="./"+fname;
                 QFile file(fpath);
                 file.open(QFile::WriteOnly);
                 file.write(fb.data(),fb.size());
                 ui->textBrowser->setTextColor(Qt::blue);
                 ui->textBrowser->setCurrentFont(QFont("Times New Roman",10));
                 ui->textBrowser->append(" " +name+" "+ip+"send file:  "+fname);
             }
        }}

void Dialog::on_fontComboBox_currentFontChanged(const QFont &f)//font
{
    ui->textEdit_2->setCurrentFont(f);
    ui->textEdit_2->setFocus();
}

void Dialog::on_comboBox_currentIndexChanged(int index)//size
{
    ui->textEdit_2->setFontPointSize(index);
    ui->textEdit_2->setFocus();
}

void Dialog::on_toolButton_3_clicked(bool checked)//bond
{
    if(checked)
        ui->textEdit_2->setFontWeight(QFont::Bold);
    else
        ui->textEdit_2->setFontWeight(QFont::Normal);
    ui->textEdit_2->setFocus();
}

void Dialog::on_toolButton_clicked(bool checked)//italic
{
    ui->textEdit_2->setFontItalic(checked);
    ui->textEdit_2->setFocus();
}

void Dialog::on_toolButton_2_clicked(bool checked)
{
    ui->textEdit_2->setFontUnderline(checked);
    ui->textEdit_2->setFocus();
}

void Dialog::on_toolButton_4_clicked(bool checked)
{
    QColor color = QColorDialog::getColor(color, this);
    if (color.isValid())
    {
        ui->textEdit_2->setTextColor(color);
        ui->textEdit_2->setFocus();
    }
}

void Dialog::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    //if(ui->tableWidget->item(index.row(),0)->text()!=localname()&&ui->tableWidget->item(index.row(),2)->text()!=getip())
     //{
         Dialogprivate  *privatechat = new Dialogprivate(0,this);
         privatechat->setWindowTitle("private chat:"+ui->tableWidget->item(index.row(),0)->text());
         privatechat->show();
         privatechat->oname=ui->tableWidget->item(index.row(),0)->text();
         privatechat->oip=ui->tableWidget->item(index.row(),1)->text();
    // }
}

void Dialog::on_pushButton_clicked()
{
    QByteArray b;
    int a=1;
    QDataStream out(&b, QIODevice::WriteOnly);
    if(serverconnect==1)
    {
        out<<a<<localname()<<getip()<<(ui->textEdit_2->toHtml());
        tcpsocket->write(b);
    }
    else
        if(serveron==1)
        {
            foreach(QTcpSocket* tcpsock,list)
                tcpsock->write(b);
            ui->textBrowser->setTextColor(Qt::blue);
            ui->textBrowser->setCurrentFont(QFont("Times New Roman",10));
            ui->textBrowser->append(" " +localname()+" "+getip()+ui->textEdit_2->toHtml());
        }
        else
        {
            int a=2;
            out<<a<<localname()<<getip()<<(ui->textEdit_2->toHtml());
            udpsocket->writeDatagram(b,b.length(),QHostAddress::Broadcast, 8888);
        }
}


void Dialog::on_pushButton_4_clicked()
{
    QFile file(filename);
    if(!file.open(QFile::ReadWrite))
    {
         QMessageBox::warning(this,tr("WARNING"),tr("No file selected!"),QMessageBox::Ok);
    }
    else
    {
        QByteArray b=file.readAll();
        int a=3;
        QByteArray b1;
        QDataStream out(&b1, QIODevice::WriteOnly);
        out<<a<<localname()<<getip()<<filename<<b;
        udpsocket->writeDatagram(b1,b1.length(),QHostAddress::Broadcast, 8888);
    }
}

void Dialog::on_pushButton_2_clicked()
{
    filename=QFileDialog::getOpenFileName(this);
}

void Dialog::on_pushButton_3_clicked()
{
    if(!serverconnect&&!serveron)
    {
        QByteArray b;
        QDataStream out(&b, QIODevice::WriteOnly);
        int a=7;
        timer=new QTimer();
        out<<a<<localname()<<getip();        
        server=new QTcpServer();
        serveron=1;
        server->listen(QHostAddress::Any,8192);
        connect(server,SIGNAL(newConnection()),this,SLOT(newconnect()));
        timer->start(500);
        connect(timer,SIGNAL(timeout()),this,SLOT(serverbroad()));
        udpsocket->writeDatagram(b,b.length(),QHostAddress::Broadcast,8888);
    }
    else
        QMessageBox::warning(this,tr("WARNING"),tr("A server already exists!"),QMessageBox::Ok);
}

void Dialog::on_pushButton_5_clicked()
{
    QFile file(filename);
    QByteArray b;
    int a=2;
    QDataStream out(&b, QIODevice::WriteOnly);
    out<<a<<localname()<<getip()<<file.readAll();
    if(!file.open(QFile::ReadWrite))
    {
         QMessageBox::warning(this,tr("WARNING"),tr("No file selected!"),QMessageBox::Ok);
    }
    if(serverconnect)
    {
       tcpsocket->write(b);
    }
    else
        if(serveron)
        {
            foreach(QTcpSocket *sock,list)
                sock->write(b);
        }
        else
          QMessageBox::warning(this,tr("WARNING"),tr("NO TCP CONNECTION"),QMessageBox::Ok);
}

