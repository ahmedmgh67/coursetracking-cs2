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


    if (!in.atEnd()) {
        in.readLine();
    }


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

    QStandardItemModel* model = new QStandardItemModel(tableView);
    QTextStream in(&file);

    bool isFirstLine = false;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');
        QStringList afields = {"Student ID", "Course ID", "Midterm 1", "Midterm 2", "Final", "Total"};

        model->setHorizontalHeaderLabels(afields);
        if (isFirstLine) {
        } else {
            if(QString::fromStdString(val).startsWith("C")){
                if(fields[1].toStdString()==val){
                    QList<QStandardItem*> items;
                    for (const QString& field : fields) {
                        if(field.toDouble()){

                            QString total =  QString::number(field.toDouble(), 'f', 2);
                            items.append(new QStandardItem(total));
                        } else {
                            items.append(new QStandardItem(field));

                        }                    }
                    QString total =  QString::number(fields[2].toDouble()+fields[3].toDouble()+fields[4].toDouble(), 'f', 2);
                    items.append(new QStandardItem (total));                    model->appendRow(items);
                }
            } else {
                if(fields[0].toStdString()==val){
                    QList<QStandardItem*> items;
                    for (const QString& field : fields) {
                        if(field.toDouble()){

                            QString total =  QString::number(field.toDouble(), 'f', 2);
                            items.append(new QStandardItem(total));
                        } else {
                            items.append(new QStandardItem(field));

                        }

                    }


                    QString total =  QString::number(fields[2].toDouble()+fields[3].toDouble()+fields[4].toDouble(), 'f', 2);
                    items.append(new QStandardItem (total));
                    model->appendRow(items);
                }
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

void ViewRegistrations::on_table_clicked(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    int row = index.row();

    QString sid = index.model()->index(row, 0).data().toString();
    QString cid = index.model()->index(row, 1).data().toString();

    edw = new EditGrades(this, sid, cid);
    edw->show();

}

