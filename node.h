#ifndef NODE_H
#define NODE_H

#endif // NODE_H

#include <list>

using namespace std;
#define sizeboard 8

struct node
{
    int board[sizeboard][sizeboard];
    int utility = -99999;
    list<node> child;
};
