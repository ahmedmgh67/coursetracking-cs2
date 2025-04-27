#ifndef VIEWSTUDENTS_H
#define VIEWSTUDENTS_H

#include <QDialog>
#include <QListView>
#include <QStringListModel>

namespace Ui {
class ViewStudents;
}

class ViewStudents : public QDialog
{
    Q_OBJECT

public:
    explicit ViewStudents(QWidget *parent = nullptr);
    ~ViewStudents();

private:
    Ui::ViewStudents *ui;
    QListView *listView;
    QStringListModel *model;
};

#endif // VIEWSTUDENTS_H
