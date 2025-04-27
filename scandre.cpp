#include "scandre.h"
#include "ui_scandre.h"





#include <QTableView>
#include <QStandardItemModel>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QStandardItem>

#include <QMap>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>


void loadTimetableView(const QString& filePath, QTableView* tableView) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Could not open file");
        return;
    }

    QTextStream in(&file);

    QStringList headers;
    int dayIndex = -1;
    int timeIndex = -1;
    int courseIndex = -1;

    QSet<QString> uniqueTimes;
    QSet<QString> uniqueDays;
    QList<QStringList> rows;

    bool isFirstLine = true;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');

        if (isFirstLine) {
            headers = fields;

            for (int i = 0; i < headers.size(); ++i) {
                QString header = headers[i].trimmed().toLower();
                if (header.contains("day")) {
                    dayIndex = i;
                } else if (header.contains("time")) {
                    timeIndex = i;
                } else if (header.contains("course") || header.contains("name")) {
                    courseIndex = i;
                }
            }

            if (dayIndex == -1 || timeIndex == -1 || courseIndex == -1) {
                qWarning("CSV missing required columns");
                return;
            }

            isFirstLine = false;
        } else {
            if (fields.size() > qMax(qMax(dayIndex, timeIndex), courseIndex)) {
                rows.append(fields);
                uniqueTimes.insert(fields[timeIndex].trimmed());
                uniqueDays.insert(fields[dayIndex].trimmed());
            }
        }
    }
    file.close();

    QStringList sortedDays = uniqueDays.values();
    QStringList sortedTimes = uniqueTimes.values();
    sortedDays.sort();
    sortedTimes.sort();

    QStandardItemModel* model = new QStandardItemModel(tableView);
    model->setHorizontalHeaderLabels(sortedDays);
    model->setVerticalHeaderLabels(sortedTimes);

    QMap<QString, int> dayColumn;
    QMap<QString, int> timeRow;

    for (int i = 0; i < sortedDays.size(); ++i) {
        dayColumn[sortedDays[i]] = i;
    }
    for (int i = 0; i < sortedTimes.size(); ++i) {
        timeRow[sortedTimes[i]] = i;
    }

    model->setRowCount(sortedTimes.size());
    model->setColumnCount(sortedDays.size());

    for (const QStringList& fields : rows) {
        QString day = fields[dayIndex].trimmed();
        QString time = fields[timeIndex].trimmed();
        QString course = fields[courseIndex].trimmed();

        int col = dayColumn.value(day, -1);
        int row = timeRow.value(time, -1);

        if (row != -1 && col != -1) {
            QStandardItem* existingItem = model->item(row, col);
            if (existingItem) {
                QString existingText = existingItem->text();
                existingItem->setText(existingText + " & " + course);
            } else {
                QStandardItem* newItem = new QStandardItem(course);
                model->setItem(row, col, newItem);
            }
        }
    }

    tableView->setModel(model);
}




void aloadCsvToComboBox(const QString& filePath, QComboBox* comboBox) {
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
            comboBox->addItem(fields[0], QVariant(fields[1])); // adjust index based on needed column
        }
    }

    file.close();
}




// Helper function to map course name to course ID
QString getCourseId(const QString& courseName, const QString& coursesFilePath) {
    QFile file(coursesFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open courses file.";
        return "";
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(',');
        if (parts.size() >= 2 && parts[1].trimmed() == courseName) {
            return parts[0].trimmed();
        }
    }
    return "";
}

// Helper function to map student ID to student name
QString getStudentName(const QString& studentId, const QString& studentsFilePath) {
    QFile file(studentsFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open students file.";
        return "";
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(',');
        if (parts.size() >= 2 && parts[0].trimmed() == studentId) {
            return parts[1].trimmed();
        }
    }
    return "";
}

// Main function
QMap<QString, double> getStudentsGradesForCourse(const QString& courseName,
                                                 const QString& coursesFilePath,
                                                 const QString& studentsFilePath,
                                                 const QString& gradesFilePath)
{
    QMap<QString, double> studentsGrades;

    QString courseId = getCourseId(courseName, coursesFilePath);
    if (courseId.isEmpty()) {
        qDebug() << "Course not found.";
        return studentsGrades;
    }

    QFile file(gradesFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open grades file.";
        return studentsGrades;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(',');
        if (parts.size() >= 5) {
            QString studentId = parts[3].trimmed();
            QString gradeCourseId = parts[4].trimmed();
            double totalGrade = parts[2].trimmed().toDouble();
            if (gradeCourseId == courseId) {
                QString studentName = getStudentName(studentId, studentsFilePath);
                if (!studentName.isEmpty()) {
                    studentsGrades.insert(studentName, totalGrade);
                }
            }
        }
    }

    return studentsGrades;
}


ScAndRe::ScAndRe(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ScAndRe)
{
    ui->setupUi(this);
    loadTimetableView("/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/courses.csv", ui->sc);



    aloadCsvToComboBox("/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/courses.csv", ui->comboBox);
    aloadCsvToComboBox("/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/students.csv", ui->comboBox_2);



}

ScAndRe::~ScAndRe()
{
    delete ui;
}

void ScAndRe::on_genreco_clicked()
{
    QMap<QString, double> a =  getStudentsGradesForCourse(ui->comboBox->currentText(),
                                                         "/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/courses.csv",
                                                         "/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/students.csv",
                                                         "/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/grades.csv");
}

