#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"dialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->setStyleSheet("background-image:url(E:/invisible.jpg)");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
  this->close();
  Dialog *chat=new Dialog(1,this);
  chat->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    this->close();
    Dialog *chat=new Dialog(0,this);
    chat->show();
}
