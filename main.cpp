#include "mainwindow.h"

#include <QApplication>
#include <QSplashScreen>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap("images/splash.png"));
    splash->show();

    MainWindow w;
    w.show();

    QTimer::singleShot(2500, splash,SLOT(close()));
    QTimer::singleShot(2500,&w,SLOT(show()));

    return a.exec();
}
