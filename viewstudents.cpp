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


    // listView = new QListView(ui->listView);
    // model = new QStringListModel(this);

    // QStringList list;


    // QFile file("/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/students.csv");
    // if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //     qWarning("Could not open file");
    //     return;
    // }

    // QTextStream in(&file);

    // // Skip the header line
    // if (!in.atEnd()) {
    //     in.readLine();
    // }

    // // Now read the actual data
    // while (!in.atEnd()) {
    //     QString line = in.readLine();
    //     QStringList fields = line.split(',');
    //     if (!fields.isEmpty()) {
    //         list << fields[0] + " - "+fields[1];
    //     }
    // }

    // file.close();



    // model->setStringList(list);

    // listView->setModel(model);



}

ViewStudents::~ViewStudents()
{
    delete ui;
}
