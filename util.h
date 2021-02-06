#ifndef UTIL_H
#define UTIL_H


#include <QString>
#include <QVector>
#include <QStringList>


struct Participant
{
    QString firstName;
    QString lastName;
    QString grade;
    QString email;


    Participant(QString lastName, QString firstName, QString grade): firstName(firstName), lastName(lastName), grade(grade){}
    Participant(QString lastName, QString firstName): firstName(firstName), lastName(lastName){}
    Participant(QString lastName, QString firstName, QString grade, QString email): firstName(firstName), lastName(lastName), grade(grade), email(email){}

    bool operator<=(Participant b)
    {
       return qMakePair(qMakePair(lastName, firstName), qMakePair(email, grade)) <= qMakePair(qMakePair(b.lastName, b.firstName), qMakePair(b.email, b.grade));
    }


};


QString remove_yo(QString text);


#endif // UTIL_H
