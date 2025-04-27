#include "student.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>

student::student(string n, string i): name(n), id(i) {}

string student::getName(){return name;}

string student::getID(){return id;}


student student::getById(string name){
    QFile file("filePath");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Could not open file");
        return student();
    }

    QTextStream in(&file);

    if (!in.atEnd()) {
        in.readLine();
    }

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');
        if (!fields.isEmpty()) {
            return student(fields[0].toStdString(), fields[1].toStdString());
        }
    }

    file.close();


}
