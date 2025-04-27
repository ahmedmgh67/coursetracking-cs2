#ifndef VIEWSTUDENTS_H
#define VIEWSTUDENTS_H

#include <QDialog>
#include <QListView>
#include <QStringListModel>
#include "viewregistrations.h"
using namespace std;

namespace Ui {
class ViewStudents;
}

class ViewStudents : public QDialog
{
    Q_OBJECT

public:
    explicit ViewStudents(QWidget *parent = nullptr);
    ~ViewStudents();

private slots:
    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::ViewStudents *ui;
    QListView *listView;
    QStringListModel *model;
    ViewRegistrations* vrw;
    void goToRegistrations(string row);

};

#endif // VIEWSTUDENTS_H
