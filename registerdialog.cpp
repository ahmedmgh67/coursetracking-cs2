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

    if (!in.atEnd()) {
        in.readLine();
    }

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');
        if (!fields.isEmpty()) {
            comboBox->addItem(fields[0], QVariant(fields[1]));
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
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qWarning("Could not open file for appending");
        return false;
    }

    QTextStream out(&file);

    QString line = rowData.join(',');

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
    QString coursesFile = "/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/courses.csv";


    QString newCourseDay, newCourseTime;
    {
        QFile courseFile(coursesFile);
        if (!courseFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            ui->resultlabel->setText("Error: Cannot open courses file.");
            return;
        }
        QTextStream courseIn(&courseFile);

        QStringList courseHeaders = courseIn.readLine().split(',');
        int idIndex = -1, dayIndex = -1, timeIndex = -1;
        for (int i = 0; i < courseHeaders.size(); ++i) {
            QString header = courseHeaders[i].trimmed().toLower();
            if (header.contains("id")) idIndex = i;
            if (header.contains("day")) dayIndex = i;
            if (header.contains("time")) timeIndex = i;
        }

        if (idIndex == -1 || dayIndex == -1 || timeIndex == -1) {
            ui->resultlabel->setText("Error: Missing fields in courses file.");
            return;
        }

        while (!courseIn.atEnd()) {
            QString line = courseIn.readLine();
            QStringList fields = line.split(',');
            if (fields.size() > qMax(qMax(idIndex, dayIndex), timeIndex)) {
                if (fields[idIndex].trimmed() == QString::fromStdString(courseid)) {
                    newCourseDay = fields[dayIndex].trimmed();
                    newCourseTime = fields[timeIndex].trimmed();
                    break;
                }
            }
        }
        courseFile.close();
    }

    if (newCourseDay.isEmpty() || newCourseTime.isEmpty()) {
        ui->resultlabel->setText("Error: Course schedule not found.");
        return;
    }


    QFile file(gradesFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->resultlabel->setText("Error: Cannot open grades file.");
    }

    QTextStream in(&file);
    int sameStudentCount = 0;
    bool recordExists = false;
    bool scheduleConflict = false;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');

        if (fields.size() < 2)
            continue;

        QString existingStudent = fields[0].trimmed();
        QString existingCourse = fields[1].trimmed();

        if (existingStudent == QString::fromStdString(sid)) {
            ++sameStudentCount;

            if (existingCourse == QString::fromStdString(courseid)) {
                recordExists = true;
                break;
            }

            QFile courseFile(coursesFile);
            if (courseFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream courseIn(&courseFile);
                QStringList courseHeaders = courseIn.readLine().split(',');
                int idIndex = -1, dayIndex = -1, timeIndex = -1;
                for (int i = 0; i < courseHeaders.size(); ++i) {
                    QString header = courseHeaders[i].trimmed().toLower();
                    if (header.contains("id")) idIndex = i;
                    if (header.contains("day")) dayIndex = i;
                    if (header.contains("time")) timeIndex = i;
                }

                while (!courseIn.atEnd()) {
                    QString courseLine = courseIn.readLine();
                    QStringList courseFields = courseLine.split(',');
                    if (courseFields.size() > qMax(qMax(idIndex, dayIndex), timeIndex)) {
                        if (courseFields[idIndex].trimmed() == existingCourse) {
                            QString existingDay = courseFields[dayIndex].trimmed();
                            QString existingTime = courseFields[timeIndex].trimmed();

                            if (existingDay == newCourseDay && existingTime == newCourseTime) {
                                scheduleConflict = true;
                            }
                            break;
                        }
                    }
                }
                courseFile.close();
            }
        }
    }
    file.close();

    if (recordExists) {
        ui->resultlabel->setText("Registration Failed: Already registered for this course.");
        return;
    }
    if (sameStudentCount >= 3) {
        ui->resultlabel->setText("Registration Failed: A student cannot register for more than 3 courses.");
        return;
    }
    if (scheduleConflict) {
        ui->resultlabel->setText("Registration Failed: Schedule conflict with another registered course.");
        return;
    }

    QStringList newRow;
    newRow << QString::fromStdString(ng.getStudent())
           << QString::fromStdString(ng.getCourse())
           << QString::fromStdString(std::to_string(ng.getMid1()))
           << QString::fromStdString(std::to_string(ng.getMid2()))
           << QString::fromStdString(std::to_string(ng.getFinal()));

    bappendRowToCsv(gradesFile, newRow);
    ui->resultlabel->setText("SUCCESS!");

    this->hide();
}



