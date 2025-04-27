#ifndef EDITGRADES_H
#define EDITGRADES_H

#include <QDialog>

namespace Ui {
class EditGrades;
}

class EditGrades : public QDialog
{
    Q_OBJECT

public:
    explicit EditGrades(QWidget *parent = nullptr, QString = NULL, QString =NULL);
    ~EditGrades();

private slots:
    void on_pushButton_clicked();

private:
    Ui::EditGrades *ui;
    QString cid;
    QString sid;
};

#endif // EDITGRADES_H
