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
            comboBox->addItem(fields[0], QVariant(fields[1])); // adjust index based on needed column
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
    out  << line<<"\n";

    file.close();
    return true;
}

void registerDialog::on_registerButton_clicked()
{
    string courseid = getIdFromName("/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/courses.csv", ui->coursecombo->currentText().toStdString());
    string sid = getIdFromName("/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/students.csv", ui->studentcombo->currentText().toStdString());
    grade ng = grade(0, 0, 0, sid, courseid);
    QString gradesFile = "/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/grades.csv";

    // Open grades.csv and verify conditions
    QFile file(gradesFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //QMessageBox::critical(this, "Error", "Failed to open grades.csv");
        return;
    }

    QTextStream in(&file);
    int sameStudentCount = 0;
    bool recordExists = false;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');

        if (fields.size() < 2)
            continue; // Defensive check

        QString existingStudent = fields[0].trimmed();
        QString existingCourse = fields[1].trimmed();

        if (existingStudent == QString::fromStdString(sid)) {
            ++sameStudentCount;

            if (existingCourse == QString::fromStdString(courseid)) {
                recordExists = true;
                break; // No need to check further
            }
        }
    }
    file.close();

    // Handle violations
    if (recordExists) {
        ui->resultlabel->setText("Registration Failed. This student is already registered for this course.");

        // QMessageBox::warning(this, "Registration Failed", "This student is already registered for this course.");
        return;
    }
    if (sameStudentCount >= 3) {
        ui->resultlabel->setText("Registration Failed. A student cannot register for more than 3 courses.");
        // QMessageBox::warning(this, "Registration Failed", "A student cannot register for more than 3 courses.");
        return;
    }

    // Proceed with registration
    QStringList newRow;
    newRow << QString::fromStdString(ng.getStudent())
           << QString::fromStdString(ng.getCourse())
           << QString::fromStdString(std::to_string(ng.getMid1()))
           << QString::fromStdString(std::to_string(ng.getMid2()))
           << QString::fromStdString(std::to_string(ng.getFinal()));

    bappendRowToCsv(gradesFile, newRow);
    ui->resultlabel->setText("SUCCESS!");

    this->hide();

    //QMessageBox::information(this, "Success", "Student registered successfully.");
}


