#include "scandre.h"
#include "ui_scandre.h"





#include <QTableView>
#include <QStandardItemModel>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QStandardItem>

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

    // Sort days and times
    QStringList sortedDays = uniqueDays.values();
    QStringList sortedTimes = uniqueTimes.values();
    sortedDays.sort();
    sortedTimes.sort();

    // Setup the model
    QStandardItemModel* model = new QStandardItemModel(tableView);
    model->setHorizontalHeaderLabels(sortedDays);
    model->setVerticalHeaderLabels(sortedTimes);

    // Map for fast lookup
    QMap<QString, int> dayColumn;
    QMap<QString, int> timeRow;

    for (int i = 0; i < sortedDays.size(); ++i) {
        dayColumn[sortedDays[i]] = i;
    }
    for (int i = 0; i < sortedTimes.size(); ++i) {
        timeRow[sortedTimes[i]] = i;
    }

    // Fill the table
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
ScAndRe::ScAndRe(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ScAndRe)
{
    ui->setupUi(this);
    loadTimetableView("/Users/ahmedgamal/UNI/CS2/coursetracking-cs2/courses.csv", ui->sc);

}

ScAndRe::~ScAndRe()
{
    delete ui;
}
