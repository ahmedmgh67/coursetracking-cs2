#include "viewcourses.h"
#include "ui_viewcourses.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QTableView>
#include <QStandardItemModel>
#include <QFile>
#include <QTextStream>
#include <QStandardItem>
#include <iostream>
#include<cstring>
#include"viewregistrations.h"
using namespace std;


void aloadCsvToTableView(const QString& filePath, QTableView* tableView) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Could not open file");
        return;
    }

    QStandardItemModel* model = new QStandardItemModel(tableView);
    QTextStream in(&file);

    bool isFirstLine = true;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');

        if (isFirstLine) {
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






ViewCourses::ViewCourses(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ViewCourses)
{
    ui->setupUi(this);

    aloadCsvToTableView("/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/courses.csv", ui->tableView);

    connect(ui->tableView, &QTableView::clicked, this, &ViewCourses::onRowClicked);


}

void ViewCourses::goToRegistrations(string row){
    vrw = new ViewRegistrations(this, row);
    vrw->show();

}
void ViewCourses::onRowClicked(const QModelIndex& index) {
    if (!index.isValid())
        return;

    int row = index.row();

    QString value = index.model()->index(row, 0).data().toString();
    qDebug() << "Clicked row:" << row << "Value:" << value<<"Data     "<< index.data();

    goToRegistrations((index.model()->index(row, 1).data().toString()).toStdString());
}

ViewCourses::~ViewCourses()
{
    delete ui;
}
