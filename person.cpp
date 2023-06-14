#include "person.h"

person::person()
{

}

void person::setPlayerName(QString name)
{
    playerName = name;
}
void person::setScore(int scr)
{
    score = scr;
}
int person::getScore()
{
    return score;
}
QString person::getPlayerName()
{
    return playerName;
}
int person::getId()
{
    return id;
}
void person::setId(int id)
{
    this->id = id;
}
void person::operator= (person p)
{
    playerName = p.getPlayerName();
    score = p.getScore();
}
void person::scoreMines1()
{
    --score;
}
void person::scoresum1()
{
    ++score;
}
