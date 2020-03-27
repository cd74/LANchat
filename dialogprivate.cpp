#include "dialogprivate.h"
#include "ui_dialogprivate.h"
#include"qdebug.h"

Dialogprivate::Dialogprivate(int a,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogprivate)
{
    ui->setupUi(this);
    prisocket=new QUdpSocket();
    prisocket->bind(QHostAddress(oip),9999);
    connect(prisocket, SIGNAL(readyRead()), this, SLOT(receive()));
    switch (a)
    {
    case 2:
        ui->textBrowser->setTextColor(Qt::blue);
        ui->textBrowser->setCurrentFont(QFont("Times New Roman",12));
        ui->textBrowser->append(" " +oname+" "+oip+"sendfile:"+rfile);
        ui->textBrowser->append(text1);
        break;
    case 1:
        ui->textBrowser->setTextColor(Qt::blue);
        ui->textBrowser->setCurrentFont(QFont("Times New Roman",12));
        ui->textBrowser->append(" " +oname+" "+oip+"  ");
        ui->textBrowser->append(text1);
        break;
    }
}

Dialogprivate::~Dialogprivate()
{
    delete ui;
}

QString myname()
{
    QString s=QHostInfo::localHostName();
    return s;
}

QString myip()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
        if(address!=QHostAddress::LocalHost &&address.toIPv4Address())
            return address.toString();
    }
    return 0;
}

void Dialogprivate::receive()
{
    QByteArray b;
    b.resize(prisocket->pendingDatagramSize());
    prisocket->readDatagram(b.data(),b.size());
    QDataStream in(&b, QIODevice::ReadOnly);
    in>>oname>>oip>>text1;
    ui->textBrowser->setTextColor(Qt::red);
    ui->textBrowser->setCurrentFont(QFont("Times New Roman",12));
    ui->textBrowser->append(" " +oname+" "+oip+"  ");
    ui->textBrowser->append(text1);

}

void Dialogprivate::on_fontComboBox_currentFontChanged(const QFont &f)//font
{
    ui->textEdit_2->setCurrentFont(f);
    ui->textEdit_2->setFocus();
}

void Dialogprivate::on_comboBox_currentIndexChanged(int index)//size
{
    ui->textEdit_2->setFontPointSize(index);
    ui->textEdit_2->setFocus();
}

void Dialogprivate::on_toolButton_3_clicked(bool checked)//bond
{
    if(checked)
        ui->textEdit_2->setFontWeight(QFont::Bold);
    else
        ui->textEdit_2->setFontWeight(QFont::Normal);
    ui->textEdit_2->setFocus();
}

void Dialogprivate::on_toolButton_clicked(bool checked)//italic
{
    ui->textEdit_2->setFontItalic(checked);
    ui->textEdit_2->setFocus();
}

void Dialogprivate::on_toolButton_2_clicked(bool checked)
{
    ui->textEdit_2->setFontUnderline(checked);
    ui->textEdit_2->setFocus();
}

void Dialogprivate::on_toolButton_4_clicked(bool checked)
{
    QColor color = QColorDialog::getColor(color, this);
    if (color.isValid())
    {
        ui->textEdit_2->setTextColor(color);
        ui->textEdit_2->setFocus();
    }
}


void Dialogprivate::on_pushButton_2_clicked()
{
    QByteArray b;
    QDataStream out(&b, QIODevice::WriteOnly);
    int a=4;
    if(count==1)
    {
        out<<a<<myname()<<myip()<<(ui->textEdit_2->toHtml());
        prisocket->writeDatagram(b,b.length(),QHostAddress(oip),8888);
    }
    else
    {
        out<<myname()<<myip()<<(ui->textEdit_2->toHtml());
        prisocket->writeDatagram(b,b.length(),QHostAddress(oip),9999);
    }
    ui->textBrowser->setTextColor(Qt::blue);
    ui->textBrowser->setCurrentFont(QFont("Times New Roman",12));
    ui->textBrowser->append(" " +myname()+" "+myip()+"  ");
    ui->textBrowser->append(ui->textEdit_2->toHtml());
    count=2;
}

void Dialogprivate::on_pushButton_5_clicked()
{
    filename=QFileDialog::getOpenFileName(this);
}

void Dialogprivate::on_pushButton_3_clicked()
{
    QFile file(filename);
    if(!file.open(QFile::ReadWrite))
    {
         QMessageBox::warning(this,tr("WARNING"),tr("No file selected!"),QMessageBox::Ok);
    }
    else
    {
        QByteArray b=file.readAll();
        int a=5;
        QByteArray b1;
        QDataStream out(&b1, QIODevice::WriteOnly);
        out<<a<<myname()<<myip()<<filename;
        if(countf==1)
            {
            prisocket->writeDatagram(b1,b1.length(),QHostAddress(oip), 8888);
            prisocket->writeDatagram(b,b.length(),QHostAddress(oip), 8888);
            }
        else
        {
            prisocket->writeDatagram(b1,b1.length(),QHostAddress(oip), 9999);
            prisocket->writeDatagram(b,b.length(),QHostAddress(oip), 9999);
        }
    }
    count=2;
}
