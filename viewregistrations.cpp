#include "viewregistrations.h"
#include "ui_viewregistrations.h"
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
using namespace  std;

string agetIdFromName(const QString& filePath, string name) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Could not open file");
        return "FAIL";
    }

    QTextStream in(&file);

    // Skip the header line
    if (!in.atEnd()) {
        in.readLine();
    }

    // Now read the actual data
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');
        if (!fields.isEmpty()) {
            if(fields[0]==name){
                return fields[1].toStdString();
            }
        }
    }

    file.close();
}


void baloadCsvToTableView(const QString& filePath, QTableView* tableView, string val) {
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
            cout <<endl << (fields[1].toStdString() == val)<< endl;
            cout <<endl << (fields[1].toStdString()) << (val)<< endl;
            if(fields[1].toStdString()==val){
                cout<<endl<<"MATCH FOUND"<<endl;
                QList<QStandardItem*> items;
                for (const QString& field : fields) {
                    items.append(new QStandardItem(field));
                }
                model->appendRow(items);
            }

        }
    }

    file.close();
    tableView->setModel(model);
}








ViewRegistrations::ViewRegistrations(QWidget *parent, string value)
    : QDialog(parent)
    , ui(new Ui::ViewRegistrations)
{
    ui->setupUi(this);

    cout<<endl;
    cout<<value<<endl;
    baloadCsvToTableView("/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/grades.csv", ui->table, value);


}

ViewRegistrations::~ViewRegistrations()
{
    delete ui;
}
