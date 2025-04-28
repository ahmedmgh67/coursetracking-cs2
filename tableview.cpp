#include "tableview.h"
#include "ui_tableview.h"
#include <QStandardItemModel>
#include <QTableView>
#include <QStandardItem>
#include <QMap>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QDebug>
using namespace std;

void displayFullGradesInTableView(const QMap<QString, QList<double>>& map, QTableView* tableView) {
    auto* model = new QStandardItemModel(map.size(), 4, tableView); // 4 columns: Name + 3 grades
    model->setHeaderData(0, Qt::Horizontal, "Student Name");
    model->setHeaderData(1, Qt::Horizontal, "Midterm 1");
    model->setHeaderData(2, Qt::Horizontal, "Midterm 2");
    model->setHeaderData(3, Qt::Horizontal, "Final Exam");

    int row = 0;
    for (auto it = map.constBegin(); it != map.constEnd(); ++it, ++row) {
        QStandardItem* nameItem = new QStandardItem(it.key());
        QStandardItem* midterm1Item = new QStandardItem(QString::number(it.value().value(0), 'f', 2));
        QStandardItem* midterm2Item = new QStandardItem(QString::number(it.value().value(1), 'f', 2));
        QStandardItem* finalExamItem = new QStandardItem(QString::number(it.value().value(2), 'f', 2));

        model->setItem(row, 0, nameItem);
        model->setItem(row, 1, midterm1Item);
        model->setItem(row, 2, midterm2Item);
        model->setItem(row, 3, finalExamItem);
    }

    tableView->setModel(model);
    tableView->resizeColumnsToContents();
}
TableView::TableView(QWidget *parent, QMap<QString, QList<double>> map)
    : QDialog(parent)
    , ui(new Ui::TableView), thismap(map)
{
    ui->setupUi(this);
    displayFullGradesInTableView(map, ui->tableView);

}


TableView::~TableView()
{
    delete ui;
}
bool exportGradesToCSV(const QMap<QString, QList<double>>& gradesMap, const QString& outputPath) {
    QFile file(outputPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file for writing:" << outputPath;
        return false;
    }

    QTextStream out(&file);

    // Write header
    out << "Student Name,Midterm 1,Midterm 2,Final Exam, Total\n";

    // Write data
    for (auto it = gradesMap.constBegin(); it != gradesMap.constEnd(); ++it) {
        out << "\"" << it.key() << "\","
            << QString::number(it.value().value(0), 'f', 2) << ","
            << QString::number(it.value().value(1), 'f', 2) << ","
            << QString::number(it.value().value(2), 'f', 2) << ","
            << QString::number(it.value().value(0)+it.value().value(1)+it.value().value(2), 'f', 2) << "\n";
    }

    file.close();
    return true;
}
void TableView::on_pushButton_clicked()
{
    exportGradesToCSV(thismap, "/Users/ahmedgamal/Downloads/report_.csv");
}

