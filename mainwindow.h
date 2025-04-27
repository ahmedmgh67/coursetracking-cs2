#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "viewstudents.h"
#include "viewcourses.h"
#include "registerDialog.h"
#include "scandre.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_viewSButton_clicked();

    void on_viewCButton_clicked();

    void on_registerButton_clicked();

    void on_viewRButtton_clicked();

private:
    Ui::MainWindow *ui;
    ViewStudents vsw;
    ViewCourses vcw;
    registerDialog rdw;
    ScAndRe scw;

};
#endif // MAINWINDOW_H
