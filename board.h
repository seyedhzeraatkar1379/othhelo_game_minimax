#ifndef BOARD_H
#define BOARD_H

#include "mokhtasat.h"
#include "node.h"
#include "person.h"
#include <iostream>
//#include <cstring>
#include <QProcess>
#include <vector>
using namespace std;
#define sizeboard 8

class board
{
private:
    person prsn;
    person comp;
    int boardArr[sizeboard][sizeboard];
    mokhtasat bestcord;
    void intializeBoard();
public:
    //void board::minimax();
    void setuser(person p)
    {
        prsn = p;
    }
    void showBoard(char,char);
    bool insertPiec(int,int);
    bool changePiec(int, int,int);
    int changePiecAndGetScore(int arr[sizeboard][sizeboard],int, int,int);
    bool changePiec(int arr[sizeboard][sizeboard],int x , int y,int);
    bool isValidMove(int [sizeboard][sizeboard],int,int,int);
    void maximize();
    vector<mokhtasat> cellCanUse(int [sizeboard][sizeboard],int);
    vector<mokhtasat> cellCanUse(int);
    int scorecalc(int [sizeboard][sizeboard],int);
    int scorecalc(int);
    int getsize();
    int minimax(int [sizeboard][sizeboard],bool,int);
    void AIPlay();
    board();
};
#endif // BOARD_H
