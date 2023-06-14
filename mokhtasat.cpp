#include "mokhtasat.h"

mokhtasat::mokhtasat()
{

}
mokhtasat::mokhtasat(int x,int y)
{
    setX(x);
    setY(y);
}
void mokhtasat::setX(int x)
{
    this->x = x;
}
void mokhtasat::setY(int y)
{
    this->y = y;
}
int mokhtasat::getX()
{
    return x;
}
int mokhtasat::getY()
{
    return y;
}
