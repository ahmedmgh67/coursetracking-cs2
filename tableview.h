#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QDialog>

namespace Ui {
class TableView;
}

class TableView : public QDialog
{
    Q_OBJECT

public:
    explicit TableView(QWidget *parent, QMap<QString, QList<double>>);
    ~TableView();

private slots:
    void on_pushButton_clicked();

private:
    Ui::TableView *ui;
    QMap<QString, QList<double>> thismap;
};

#endif // TABLEVIEW_H
