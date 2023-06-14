#include <QCoreApplication>
#include "board.h"
#include <QDebug>
#include <unistd.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    char color,colorvs='b';
    qDebug()<<"hello!";
    do
    {
        qDebug()<<"select color: w  or  b:";
        cin >> color;
    }while(!(color == 'w' || color == 'b'));
    if(color == 'b')
        colorvs = 'w';
    board *b= new board();
    int x,y;
    bool inserted=false;
    while(true)
    {
        int if1 = (b->cellCanUse(1).size());
        if(if1 > 0)
        {
            b->showBoard(color,colorvs);
            qDebug()<<"enter x:";
            cin>>x;
            qDebug()<<"enter y:";
            cin>>y;
            if(x<0 || y<0 || x>b->getsize() || y>b->getsize())
                continue;
            inserted = b->insertPiec(x,y);
            if(inserted == false)
                continue;
            b->showBoard(color,colorvs);
            usleep(2000000);
        }
        int if2 = (b->cellCanUse(2).size());
        if(if2 > 0)
            b->AIPlay();
        if(if1 <=0 && if2 <= 0)
            break;
    }
    int user = b->scorecalc(1);
    int AI = b->scorecalc(2);
    qDebug()<<"your score";
    qDebug()<<user;
    qDebug()<<"AI Score";
    qDebug()<<AI;
    if(user > AI)
        qDebug()<<"You Win!";
    else
        qDebug()<<"You Lose!";
    return a.exec();
}
