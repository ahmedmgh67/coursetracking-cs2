#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QPushButton>
#include<QLabel>





#include <QPushButton>
#include <QGridLayout>
#include <QWidget>
#include <QIcon>

void setupGridButtons(QMainWindow* mainWindow) {

}




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QMainWindow* mainWindow = this;
    QPushButton* button1 = new QPushButton(QIcon(":/images/icon1.png"), "View Students");
    QPushButton* button2 = new QPushButton(QIcon(":/images/icon2.png"), "View Grades");
    QPushButton* button3 = new QPushButton(QIcon(":/images/icon3.png"), "Register Student to Course");
    QPushButton* button4 = new QPushButton(QIcon(":/images/icon4.png"), "Schedule and Reports");

    QSize iconSize(32, 32); // Icon size
    button1->setIconSize(iconSize);
    button2->setIconSize(iconSize);
    button3->setIconSize(iconSize);
    button4->setIconSize(iconSize);

    // Create grid layout
    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(button1, 0, 0);
    gridLayout->addWidget(button2, 0, 1);
    gridLayout->addWidget(button3, 1, 0);
    gridLayout->addWidget(button4, 1, 1);

    // Main vertical layout
    QVBoxLayout* mainLayout = new QVBoxLayout;

    // Add a title label
    QLabel* titleLabel = new QLabel("Course Tracker App");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 32px; font-weight: bold; padding: 20px;");
    QLabel* subtitleLabel = new QLabel("By: Ahmed Mostafa & Fady Latif \n Under supervision of Prof. Tamer EL-batt");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("font-size: 22px;  padding: 0px;");

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(subtitleLabel);
    mainLayout->addLayout(gridLayout);

    // Central widget
    QWidget* centralWidget = new QWidget;
    centralWidget->setLayout(mainLayout);
    mainWindow->setCentralWidget(centralWidget);

    // Set window minimum size
    mainWindow->setMinimumSize(800, 600);

    centralWidget->setLayout(gridLayout);
    mainWindow->setCentralWidget(centralWidget);

    // Set window minimum size
    mainWindow->setMinimumSize(800, 600);

    // Apply style
    mainWindow->setStyleSheet(R"(
        QMainWindow {
            background-color: #f0f2f5;
        }

        QPushButton {
            background-color: #0078D7;
            color: white;
            border: none;
            border-radius: 12px;
            padding: 20px;
            font-size: 20px;
            text-align: center; /* Align text to the left beside icon */
        }

        QPushButton:hover {
            background-color: #005a9e;
        }

        QPushButton:pressed {
            background-color: #004578;
        }
QLabel {
            color: #333333;
        }
    )");




    QObject::connect(button1, &QPushButton::clicked, [this]() {
        MainWindow::vsw.show();

    });

    QObject::connect(button2, &QPushButton::clicked, [mainWindow, this]() {
        MainWindow::vcw.show();
    });

    QObject::connect(button3, &QPushButton::clicked, [mainWindow, this]() {
        MainWindow::rdw.show();
    });

    QObject::connect(button4, &QPushButton::clicked, [mainWindow, this]() {
        MainWindow::scw.show();
    });}

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

