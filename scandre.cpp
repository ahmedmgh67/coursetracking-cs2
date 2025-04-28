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

    // Custom day order starting from Sunday
    QStringList weekOrder = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

    // Sort days according to weekOrder
    QStringList sortedDays;
    for (const QString& day : weekOrder) {
        if (uniqueDays.contains(day)) {
            sortedDays.append(day);
        }
    }

    QStringList sortedTimes = uniqueTimes.values();
    sortedTimes.sort(); // normal time sorting

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
                newItem->setTextAlignment(Qt::AlignCenter);
                model->setItem(row, col, newItem);
            }
        }
    }

    tableView->setModel(model);
    tableView->resizeColumnsToContents();
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




#include <QMap>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>

// Helper: Get course ID from course name
QString getCourseId(const QString& courseName, const QString& coursesFilePath) {
    QFile file(coursesFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open courses file.";
        return "";
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(',');
        if ( parts[0].trimmed() == courseName) {
            return parts[1].trimmed();
        }
    }
    return "";
}

// Helper: Get student name from student ID
QString getStudentName(const QString& studentId, const QString& studentsFilePath) {
    QFile file(studentsFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open students file.";
        return "";
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(',');
        if (parts[1].trimmed() == studentId) {
            return parts[0].trimmed();
        }
    }
    return "";
}




QMap<QString, QList<double>> getStudentsGradesForCourse(const QString& courseName,
                                                        const QString& coursesFilePath,
                                                        const QString& studentsFilePath,
                                                        const QString& gradesFilePath)
{
    QMap<QString, QList<double>> studentsGrades;

    QString courseId = getCourseId(courseName, coursesFilePath);
    if (courseId.isEmpty()) {
        qWarning() << "Course not found.";
        //return studentsGrades;
    }

    QFile file(gradesFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open grades file.";
        return studentsGrades;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(',');
        if (parts.size() >= 5) {
            QString studentId = parts[0].trimmed();
            QString gradeCourseId = parts[1].trimmed();
            double midterm1 = parts[2].trimmed().toDouble();
            double midterm2 = parts[3].trimmed().toDouble();
            double finalExam = parts[4].trimmed().toDouble();

            if (gradeCourseId == courseId) {
                QString studentName = getStudentName(studentId, studentsFilePath);
                if (!studentName.isEmpty()) {
                    QList<double> grades = { midterm1, midterm2, finalExam };
                    studentsGrades.insert(studentName, grades);
                }
            }
        }
    }

    return studentsGrades;
}





QString getStudentId(const QString& studentName, const QString& studentsFilePath) {
    QFile file(studentsFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open students file.";
        return "";
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(',');
        if (parts[0].trimmed() == studentName) {
            return parts[1].trimmed();
        }
    }
    return "";
}

// Helper: Get course name from course ID
QString getCourseName(const QString& courseId, const QString& coursesFilePath) {
    QFile file(coursesFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open courses file.";
        return "";
    }
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(',');
        if (parts[1].trimmed() == courseId) {
            return parts[0].trimmed();
        }
    }
    return "";
}



QMap<QString, QList<double>> getCoursesGradesForStudent(const QString& studentName,
                                                        const QString& studentsFilePath,
                                                        const QString& coursesFilePath,
                                                        const QString& gradesFilePath)
{
    QMap<QString, QList<double>> coursesGrades;

    QString studentId = getStudentId(studentName, studentsFilePath);
    if (studentId.isEmpty()) {
        qWarning() << "Student not found.";
        //return coursesGrades;
    }

    QFile file(gradesFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open grades file.";
        return coursesGrades;
    }

    QTextStream in(&file);
    double f =0;
    int count=0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(',');
        if (parts.size() >= 5) {
            QString gradeStudentId = parts[0].trimmed();
            QString courseId = parts[1].trimmed();
            double midterm1 = parts[2].trimmed().toDouble();
            double midterm2 = parts[3].trimmed().toDouble();
            double finalExam = parts[4].trimmed().toDouble();

            if (gradeStudentId == studentId) {
                QString courseName = getCourseName(courseId, coursesFilePath);
                if (!courseName.isEmpty()) {
                    QList<double> grades = { midterm1, midterm2, finalExam };
                    coursesGrades.insert(courseName, grades);
                    f+=finalExam;
                    count++;
                }
            }
        }
    }
    QList<double> agrades = {0,0,(f/count)};
    coursesGrades.insert("Overall Average Grade", agrades);

    return coursesGrades;
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
    QMap<QString, QList<double>> a =  getStudentsGradesForCourse(ui->comboBox->currentText(),
                                                         "/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/courses.csv",
                                                         "/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/students.csv",
                                                         "/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/grades.csv");

    tvw = new TableView(this, a);
    tvw->show();


}


void ScAndRe::on_genrest_clicked()
{
    QMap<QString, QList<double>> a =  getCoursesGradesForStudent(ui->comboBox_2->currentText(),
                                                                "/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/students.csv",
                                                                "/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/courses.csv",
                                                                "/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/grades.csv");

    tvw = new TableView(this, a);
    tvw->show();
}

