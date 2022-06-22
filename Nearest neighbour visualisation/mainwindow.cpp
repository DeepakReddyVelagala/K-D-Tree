#include "mainwindow.h"
#include "./ui_mainwindow.h"
//#include "kdtree.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    //kdtree tree;
   //tree.setModal(true);
    //tree.exec();
    hide();
    tree = new kdtree(this);
    tree->show();
}

