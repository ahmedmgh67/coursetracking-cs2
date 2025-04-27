#ifndef STUDENT_H
#define STUDENT_H
#include <string>
using namespace std;
class student
{
private:
    string name;
    string id;

public:
    student(string="", string="");
    string getName();

    string getID();
    student getById(string name);
};

#endif // STUDENT_H
