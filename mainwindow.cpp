#include "mainwindow.h"
#include "ui_mainwindow.h"


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

void MainWindow::on_viewSButton_clicked()
{
    vsw.show();
}


void MainWindow::on_viewCButton_clicked()
{
    vcw.show();
}


void MainWindow::on_registerButton_clicked()
{
    rdw.show();
}


void MainWindow::on_viewRButtton_clicked()
{
    scw.show();
}

