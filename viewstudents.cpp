#include "viewstudents.h"
#include "ui_viewstudents.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QTableView>
#include <QStandardItemModel>
#include <QFile>
#include <QTextStream>
#include <QStandardItem>




void loadCsvToTableView(const QString& filePath, QTableView* tableView) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Could not open file");
        return;
    }

    QStandardItemModel* model = new QStandardItemModel(tableView); // Parent is tableView
    QTextStream in(&file);

    bool isFirstLine = true;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');

        if (isFirstLine) {
            // Set headers from first line
            model->setHorizontalHeaderLabels(fields);
            isFirstLine = false;
        } else {
            QList<QStandardItem*> items;
            for (const QString& field : fields) {
                items.append(new QStandardItem(field));
            }
            model->appendRow(items);
        }
    }

    file.close();
    tableView->setModel(model);
}



ViewStudents::ViewStudents(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ViewStudents)
{
    ui->setupUi(this);




    loadCsvToTableView("/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/students.csv", ui->tableView);




}

ViewStudents::~ViewStudents()
{
    delete ui;
}

void ViewStudents::goToRegistrations(string row){
    vrw = new ViewRegistrations(this, row);
    vrw->show();

}

void ViewStudents::on_tableView_clicked(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    int row = index.row();

    QString value = index.model()->index(row, 0).data().toString();
    qDebug() << "Clicked row:" << row << "Value:" << value<<"Data     "<< index.data();


    goToRegistrations((index.model()->index(row, 1).data().toString()).toStdString());
}

