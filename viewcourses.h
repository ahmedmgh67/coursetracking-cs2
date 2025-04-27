#ifndef VIEWCOURSES_H
#define VIEWCOURSES_H
#include "viewregistrations.h"
#include <QDialog>
#include <string>

namespace Ui {
class ViewCourses;
}

class ViewCourses : public QDialog
{
    Q_OBJECT

public:
    explicit ViewCourses(QWidget *parent = nullptr);
    ~ViewCourses();
    void onRowClicked(const QModelIndex& index);
    void goToRegistrations(std::string row);

private:
    Ui::ViewCourses *ui;
    ViewRegistrations* vrw;
};

#endif // VIEWCOURSES_H
