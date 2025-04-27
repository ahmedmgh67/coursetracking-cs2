#include "registerdialog.h"
#include "ui_registerdialog.h"

#include <QComboBox>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include "grade.h"
#include <iostream>
#include <QMessageBox>
void loadCsvToComboBox(const QString& filePath, QComboBox* comboBox) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Could not open file");
        return;
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
            comboBox->addItem(fields[0]); // adjust index based on needed column
        }
    }

    file.close();
}
string getIdFromName(const QString& filePath, string name) {
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

registerDialog::registerDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::registerDialog)
{
    ui->setupUi(this);
    loadCsvToComboBox("/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/courses.csv",ui->coursecombo);
    loadCsvToComboBox("/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/students.csv",ui->studentcombo);

}

registerDialog::~registerDialog()
{
    delete ui;
}



bool bappendRowToCsv(const QString& filePath, const QStringList& rowData) {
    QFile file(filePath);
    if (!file.open(QIODevice::Append | QIODevice::Text)) { // Open in append mode
        qWarning("Could not open file for appending");
        return false;
    }

    QTextStream out(&file);

    // Join the rowData list into a comma-separated string
    QString line = rowData.join(',');

    // Append a newline and the data
    out << "\n" << line;

    file.close();
    return true;
}

void registerDialog::on_registerButton_clicked()
{
    string courseid = getIdFromName("/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/courses.csv", ui->coursecombo->currentText().toStdString());
    string sid = getIdFromName("/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/students.csv", ui->studentcombo->currentText().toStdString());
    grade ng = grade(0,0,0,sid, courseid);
    this->hide();
    //QMessageBox::information(nullptr, QString::fromStdString( "Success"), QString::fromStdString( "Successfully Regsitered Student"));
    QStringList newRow;
    newRow << QString::fromStdString( ng.getStudent()) << QString::fromStdString( ng.getCourse() )<< QString::fromStdString(to_string( ng.getMid1())) << QString::fromStdString(to_string(ng.getMid2())) << QString::fromStdString(to_string(ng.getFinal()));
    bappendRowToCsv("/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/grades.csv", newRow);
}

