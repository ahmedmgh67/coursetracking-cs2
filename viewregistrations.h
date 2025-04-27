#ifndef VIEWREGISTRATIONS_H
#define VIEWREGISTRATIONS_H

#include <QDialog>
#include "editgrades.h"

namespace Ui {
class ViewRegistrations;
}

class ViewRegistrations : public QDialog
{
    Q_OBJECT

public:
    explicit ViewRegistrations(QWidget *parent = nullptr, std::string val = "");
    ~ViewRegistrations();

private slots:
    void on_table_clicked(const QModelIndex &index);

private:
    Ui::ViewRegistrations *ui;
    EditGrades* edw;
};

#endif // VIEWREGISTRATIONS_H
