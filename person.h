#ifndef PERSON_H
#define PERSON_H

#include <QString>



class person
{
private:
    QString playerName;
    int score = 0;
    int id;
public:
    void setPlayerName(QString);
    void setScore(int);
    int getScore();
    QString getPlayerName();
    int getId();
    void setId(int);
    void scoreMines1();
    void scoresum1();
    void operator=(person p);
    person();
};

#endif // PERSON_H
