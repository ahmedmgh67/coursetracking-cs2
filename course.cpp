#include "course.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>

course::course(string n, string i, string t, string d):name(n), id(i), startTime(t), day(d) {}

string course::getName(){return name;}

string course::getID(){return id;}

string course::getStartTime(){return startTime;}

string course::getDay(){return day;}

course course::getById(string name){
            QFile file("filePath");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning("Could not open file");
            return course();
        }

        QTextStream in(&file);

        if (!in.atEnd()) {
            in.readLine();
        }

        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(',');
            if (!fields.isEmpty()) {
                return course(fields[0].toStdString(), fields[1].toStdString(), fields[2].toStdString(), fields[3].toStdString());
            }
        }

        file.close();


}
