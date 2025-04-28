#ifndef SCANDRE_H
#define SCANDRE_H

#include <QDialog>
#include <tableview.h>

namespace Ui {
class ScAndRe;
}

class ScAndRe : public QDialog
{
    Q_OBJECT

public:
    explicit ScAndRe(QWidget *parent = nullptr);
    ~ScAndRe();

private slots:
    void on_genreco_clicked();

private:
    Ui::ScAndRe *ui;
    TableView* tvw;
};

#endif // SCANDRE_H
