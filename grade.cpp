#include "grade.h"

grade::grade(double m1, double m2, double f, string s, string c):mid1(m1), mid2(m2), final(f), student(s), course(c) {}


double grade::getMid1(){return mid1;}

double grade::getMid2(){return mid2;}

double grade::getFinal(){return final;}

void grade::setMid1(int n){mid1=n;}

void grade::setMid2(int n){mid2=n;}

void grade::setFinal(int n){final=n;}

double grade::getTotal(){return mid1+mid2+final;}

string grade::getStudent(){return student;}

string grade::getCourse(){return course;}


bool appendRowToCsv(const QString& filePath, const QStringList& rowData) {
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
