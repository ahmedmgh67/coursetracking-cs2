#ifndef GRADE_H
#define GRADE_H
#include<string>
#include <QFile>
#include <QTextStream>
#include <QStringList>

using namespace std;

class grade
{
private:
    double mid1;
    double mid2;
    double final;
    string student;
    string course;
public:
    grade(double =0, double=0, double=0, string="", string="");
    double getMid1();

    double getMid2();

    double getFinal();

    double getTotal();

    void setMid1(int n);

    void setMid2(int n);

    void setFinal(int n);

    string getStudent();

    string getCourse();

};

#endif // GRADE_H
