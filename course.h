#ifndef COURSE_H
#define COURSE_H
#include <string>
using namespace std;

class course
{
private:
    string name;
    string id;
    string startTime;
    string day;
public:
    course(string = "", string ="", string="", string="");
    string getName();

    string getID();

    string getStartTime();

    string getDay();
    static course getById(string);
};

#endif // COURSE_H
