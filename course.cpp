#include "course.h"

course::course(string n, string i, string t, string d):name(n), id(i), startTime(t), day(d) {}

string course::getName(){return name;}

string course::getID(){return id;}

string course::getStartTime(){return startTime;}

string course::getDay(){return day;}
