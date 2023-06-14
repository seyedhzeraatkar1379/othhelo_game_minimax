#include "board.h"


board::board()
{
    intializeBoard();
}
int board::getsize()
{
    return sizeboard;
}
void board::intializeBoard()
{
    prsn.setScore(2);
    prsn.setPlayerName("user1");
    prsn.setId(1);

    comp.setScore(2);
    comp.setPlayerName("computer");
    comp.setId(2);

    for(int i = 0 ; i < sizeboard ; i++)
        for(int j = 0 ; j < sizeboard ;j++){
            boardArr[i][j] = 0;
        }
    boardArr[3][3] = prsn.getId();
    boardArr[4][4] = prsn.getId();
    boardArr[3][4] = comp.getId();
    boardArr[4][3] = comp.getId();
}
void board::showBoard(char user='b',char com='w')
{
    QProcess::execute("clear");
    cout<<"\n";
    cout<<"  0 1 2 3 4 5 6 7\n";
    for(int i = 0 ; i < sizeboard ; i++)
    {
        cout<<i<<" ";
        for(int j = 0 ; j < sizeboard ; j++){
            //cout<<"ij: "<<i<<j<<"\t"<<boardArr[i][j]<<"\t";
            //user
            if(boardArr[i][j] == 1)
                cout<<user<<" ";
            else if(boardArr[i][j] == 2)
                cout<<com<<" ";
            else
                cout<<"- ";
        }
        cout<<"\n";
    }
    cellCanUse(boardArr,1);
    //cout<<"comp: "<<comp.getScore()<<"   user: "<<prsn.getScore()<<endl;
}
bool board::insertPiec(int x,int y)
{
    if(changePiec(x,y,1))
    {
        return true;
    }
    return false;
}


//AI START
//pid =1 or pid =2
void board::maximize()
{
    mokhtasat mokhbest(0,0);
    int maxScore = scorecalc(2);
    vector<mokhtasat> decisions = cellCanUse(boardArr,2);
    for(int i= 0 ; i < decisions.size() ; i++)
    {
        int mx = changePiecAndGetScore(boardArr,decisions[i].getX(),decisions[i].getY(),2);
        if(maxScore < mx)
        {
            maxScore = mx;
            int x = decisions[i].getX();
            int y = decisions[i].getY();
            mokhbest.setX(x);
            mokhbest.setY(y);
        }
    }
    changePiec(mokhbest.getX(),mokhbest.getY(),2);
    //cout<<"\nX: "<<mokhbest.getX()<<"  Y: "<<mokhbest.getY()<<"\n";
}

//minimax Solution
int board::minimax(int arr[sizeboard][sizeboard],bool max,int depth)
{
    if(depth == 0)
        return scorecalc(arr,2);
    if (max) {
        int bestValue = -99999;
        vector<mokhtasat> list1 = cellCanUse(arr,2);
        for (mokhtasat move : list1) {
            int newBoard[sizeboard][sizeboard];
            memcpy(&newBoard,&arr,sizeboard*sizeboard*sizeof (int));
            changePiec(newBoard,move.getX(),move.getY(),2);
            int v = minimax(newBoard, depth - 1, !max);
            if (v > bestValue) {
                bestValue = v;
                bestcord.setX(move.getX());
                bestcord.setY(move.getY());
            }
        }
        return bestValue;
    }
    else {
        int bestValue = 99999;
        vector<mokhtasat> list1 = cellCanUse(arr,1);
        for (mokhtasat move : list1) {
            int newBoard[sizeboard][sizeboard];
            memcpy(&newBoard,&arr,sizeboard*sizeboard*sizeof (int));
            changePiec(newBoard,move.getX(),move.getY(),2);
            int v = minimax(newBoard, depth - 1, !max);
            if (v < bestValue) {
                bestcord.setX(move.getX());
                bestcord.setY(move.getY());
            }
        }
        return bestValue;
    }
}
void board::AIPlay()
{
    minimax(boardArr,2,20);
    changePiec(bestcord.getX(),bestcord.getY(),2);
}
//AI END
//calc SCORE
int board::scorecalc(int pid)
{
    int userScore = 0 ,compScore = 0;
    for(int i = 0 ; i < sizeboard ; i++)
        for(int j = 0 ; j < sizeboard ; j++)
        {
            if(boardArr[i][j] == 1)
                userScore++;
            else if(boardArr[i][j] == 2)
                compScore++;
        }
    if(pid==1)
        return (userScore-compScore);
    else if(pid==2)
        return (compScore-userScore);
    else return 0;
}
int board::scorecalc(int arr[sizeboard][sizeboard],int pid)
{
    int userScore = 0 ,compScore = 0;
    for(int i = 0 ; i < sizeboard ; i++)
        for(int j = 0 ; j < sizeboard ; j++)
        {
            if(arr[i][j] == 1)
                userScore++;
            else if(arr[i][j] == 2)
                compScore++;
        }
    if(pid==1)
        return (userScore-compScore);
    else if(pid==2)
        return (compScore-userScore);
    else return 0;
}
//calc SCORE END
// List can insert START
vector<mokhtasat> board::cellCanUse(int brd[sizeboard][sizeboard],int pid)
{
    vector<mokhtasat> listDecision;
    for(int i = 0 ; i < sizeboard ; i++)
        for(int j = 0 ; j < sizeboard ; j++)
        {
            if(isValidMove(boardArr,i,j,pid))
            {
                mokhtasat m(i,j);
                //cout<<"mokhi: "<<i<<"mokhj: "<<j<<endl;
                listDecision.push_back(m);
            }
        }
    return listDecision;
}
vector<mokhtasat> board::cellCanUse(int pid)
{
    vector<mokhtasat> listDecision;
    for(int i = 0 ; i < sizeboard ; i++)
        for(int j = 0 ; j < sizeboard ; j++)
        {
            if(isValidMove(boardArr,i,j,pid))
            {
                mokhtasat m(i,j);
                cout<<"mokhi: "<<i<<"mokhj: "<<j<<endl;
                listDecision.push_back(m);
            }
        }
    return listDecision;
}
bool board::isValidMove(int brd[sizeboard][sizeboard],int x,int y,int pid)
{
    int personvsid = 2;
    if(pid == 2)
        personvsid = 1;

    if(x > sizeboard || y > sizeboard || x < 0 || y < 0 || brd[x][y]!=0)
        return false;
    /*
     * * * * *
     * * * * *
     * * 2 * *
     * * * \ *
     * * * * \
    */
    if(x+1 < sizeboard && y+1 < sizeboard){
        bool if1 = false,if2 = false;
        for(int i = x+1,j = y+1 ; i < sizeboard && j < sizeboard;i++,j++){
            if(if1 == false && (brd[i][j] == personvsid)){
                if1 = true;
            }
            else if((brd[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && brd[i][j] == pid)
                {
                    return true;
                }
            }
        }
    }
    /*
     * * * * *
     * * * * *
     * * 2 * *
     * * | * *
     * * | * *
    */
    if(x+1 < sizeboard){
        bool if1 = false,if2 = false;
        for(int i = x+1,j = y ; i < sizeboard ;i++){
            if(if1 == false && (brd[i][j] == personvsid)){
                if1 = true;
            }
            else if((brd[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && brd[i][j] == pid)
                {
                    return true;
                }
            }
        }

    }
    /*
     * * * * *
     * * * * *
     * * 2 - -
     * * * * *
     * * * * *
    */
    if(y+1 < sizeboard){
        bool if1 = false,if2 = false;
        for(int i = x,j = y+1 ; j < sizeboard ;j++){
            if(if1 == false && (brd[i][j] == personvsid)){
                if1 = true;
                continue;
            }
            else if((brd[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && brd[i][j] == pid)
                {
                    return true;
                }
            }
        }
    }
    /*
     \ * * * *
     * \ * * *
     * * 2 * *
     * * * * *
     * * * * *
    */
    if(x-1 > 0 && y-1>0){
        bool if1 = false,if2 = false;
        for(int i = x-1,j = y-1 ; i >= 0 && j >= 0 ;i--,j--){
            if(if1 == false && (brd[i][j] == personvsid)){
                if1 = true;
                continue;
            }
            else if((brd[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && brd[i][j] == pid)
                {
                    return true;
                }
            }
        }
    }
    /*
     * * | * *
     * * | * *
     * * 2 * *
     * * * * *
     * * * * *
    */
    if(x-1 > 0){
        bool if1 = false,if2 = false;
        for(int i = x-1,j = y ; i >= 0 ;i--){
            if(if1 == false && (brd[i][j] == personvsid)){
                if1 = true;
                continue;
            }
            else if((brd[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && brd[i][j] == pid)
                {
                    return true;
                }
            }
        }
    }
    /*
     * * * * *
     * * * * *
     - - 2 * *
     * * * * *
     * * * * *
    */
    if(y-1 > 0){
        bool if1 = false,if2 = false;
        for(int i = x,j = y-1 ; j >= 0 ;j--){
            if(if1 == false && (brd[i][j] == personvsid)){
                if1 = true;
                continue;
            }
            else if((brd[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && brd[i][j] == pid)
                {
                    return true;
                }
            }
        }
    }
    /*
     * * * * *
     * * * * *
     * * 2 * *
     * / * * *
     / * * * *
    */
    if(x+1 < sizeboard && y-1 > 0)
    {
        bool if1 = false,if2 = false;
        for(int i = x+1,j = y-1 ; i < sizeboard && j >= 0 ;i++,j--){
            if(if1 == false && (brd[i][j] == personvsid)){
                if1 = true;
                continue;
            }
            else if((brd[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && brd[i][j] == pid)
                {
                    return true;
                }
            }
        }
    }
    /*
     * * * * /
     * * * / *
     * * 2 * *
     * * * * *
     * * * * *
    */
    if(x-1 > 0 && y+1 < sizeboard)
    {
        bool if1 = false,if2 = false;
        for(int i = x-1,j = y+1 ; j < sizeboard && i >= 0 ;j++,i--){
            if(if1 == false && (brd[i][j] == personvsid)){
                if1 = true;
                continue;
            }
            else if((brd[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && brd[i][j] == pid)
                {
                    return true;
                }
            }
        }
    }

    return false;
}
// List can insert END
int board::changePiecAndGetScore(int arr1[sizeboard][sizeboard],int x , int y,int pid)
{

    int arr[sizeboard][sizeboard];
    for(int i = 0 ; i < sizeboard ; i++)
        for(int j = 0 ; j < sizeboard ;j++){
            arr[i][j] = arr1[i][j];
        }
    int personvsid = 1;
    if(pid == 1)
        personvsid = 2;
    bool isSafe = false;
    if( !(x >= 0 && x < sizeboard) || !(y < sizeboard && y >= 0) || arr[x][y]!=0)
        return false;
    /*
     * * * * *
     * * * * *
     * * 2 * *
     * * * \ *
     * * * * \
    */
    if(x+1 < sizeboard && y+1 < sizeboard){
        bool if1 = false,if2 = false;
        for(int i = x+1,j = y+1 ; i < sizeboard && j < sizeboard;i++,j++){
            if(if1 == false && (arr[i][j] == personvsid)){
                if1 = true;
            }
            else if((arr[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && arr[i][j] == pid)
                {
                    if2=true;
                    break;
                }
            }
        }
        if(if1 && if2){
            for(int i = x,j = y ; i < sizeboard && j < sizeboard;i++,j++){
                if(arr[i][j] != pid){
                    arr[i][j] = pid;
                }
                else
                {
                    isSafe = true;
                    break;
                }
            }
        }
    }
    /*
     * * * * *
     * * * * *
     * * 2 * *
     * * | * *
     * * | * *
    */
    if(x+1 < sizeboard){
        bool if1 = false,if2 = false;
        for(int i = x+1,j = y ; i < sizeboard ;i++){
            if(if1 == false && (arr[i][j] == personvsid)){
                if1 = true;
            }
            else if((arr[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && arr[i][j] == pid)
                {
                    if2=true;
                    break;
                }
            }
        }
        if(if1 && if2){
            for(int i = x,j = y ; i < sizeboard ;i++){
                if(arr[i][j] != pid){

                    arr[i][j] = pid;


                }
                else
                {
                    isSafe = true;
                    break;
                }
            }
        }
    }
    /*
     * * * * *
     * * * * *
     * * 2 - -
     * * * * *
     * * * * *
    */
    if(y+1 < sizeboard){
        bool if1 = false,if2 = false;
        for(int i = x,j = y+1 ; j < sizeboard ;j++){
            if(if1 == false && (arr[i][j] == personvsid)){
                if1 = true;
                continue;
            }
            else if((arr[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && arr[i][j] == pid)
                {
                    if2=true;
                    break;
                }
            }
        }
        if(if1 && if2){
            for(int i = x,j = y ; j < sizeboard ;j++){
                if(arr[i][j] != pid){

                    arr[i][j] = pid;
                }
                else
                {
                    isSafe = true;
                    break;
                }
            }
        }
    }
    /*
     \ * * * *
     * \ * * *
     * * 2 * *
     * * * * *
     * * * * *
    */
    if(x-1 > 0 && y-1>0){
        bool if1 = false,if2 = false;
        for(int i = x-1,j = y-1 ; i >= 0 && j >= 0 ;i--,j--){
            if(if1 == false && (arr[i][j] == personvsid)){
                if1 = true;
                continue;
            }
            else if((arr[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && arr[i][j] == pid)
                {
                    if2=true;
                    break;
                }
            }
        }
        if(if1 && if2){
            for(int i = x,j = y ; i > 0 && j > 0 ;i--,j--){
                if(arr[i][j] != pid){
                    arr[i][j] = pid;
                }
                else
                {
                    isSafe = true;
                    break;
                }
            }
        }
    }
    /*
     * * | * *
     * * | * *
     * * 2 * *
     * * * * *
     * * * * *
    */
    if(x-1 > 0){
        bool if1 = false,if2 = false;
        for(int i = x-1,j = y ; i >= 0 ;i--){
            if(if1 == false && (arr[i][j] == personvsid)){
                if1 = true;
                continue;
            }
            else if((arr[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && arr[i][j] == pid)
                {
                    if2=true;
                    break;
                }
            }
        }
        if(if1 && if2){
            for(int i = x,j = y ; i > 0 ;i--){
                if(arr[i][j] != pid){
                    arr[i][j] = pid;
                }
                else
                {
                    isSafe = true;
                    break;
                }
            }
        }
    }
    /*
     * * * * *
     * * * * *
     - - 2 * *
     * * * * *
     * * * * *
    */
    if(y-1 > 0){
        bool if1 = false,if2 = false;
        for(int i = x,j = y-1 ; j >= 0 ;j--){
            if(if1 == false && (arr[i][j] == personvsid)){
                if1 = true;
                continue;
            }
            else if((arr[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && arr[i][j] == pid)
                {
                    if2=true;
                    break;
                }
            }
        }
        if(if1 && if2){
            for(int i = x,j = y ; j > 0 ;j--){
                if(arr[i][j] != pid){
                    arr[i][j] = pid;
                }
                else
                {
                    isSafe = true;
                    break;
                }
            }
        }
    }
    /*
     * * * * *
     * * * * *
     * * 2 * *
     * / * * *
     / * * * *
    */
    if(x+1 < sizeboard && y-1 > 0)
    {
        bool if1 = false,if2 = false;
        for(int i = x+1,j = y-1 ; i < sizeboard && j >= 0 ;i++,j--){
            if(if1 == false && (arr[i][j] == personvsid)){
                if1 = true;
                continue;
            }
            else if((arr[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && arr[i][j] == pid)
                {
                    if2=true;
                    break;
                }
            }
        }
        if(if1 && if2){
            for(int i = x,j = y ; j > 0 && i < sizeboard ;i++,j--){
                if(arr[i][j] != pid){
                    arr[i][j] = pid;
                }
                else
                {
                    isSafe = true;
                    break;
                }
            }
        }
    }
    /*
     * * * * /
     * * * / *
     * * 2 * *
     * * * * *
     * * * * *
    */
    if(x-1 > 0 && y+1 < sizeboard)
    {
        bool if1 = false,if2 = false;
        for(int i = x-1,j = y+1 ; j < sizeboard && i >= 0 ;j++,i--){
            if(if1 == false && (arr[i][j] == personvsid)){
                if1 = true;
                continue;
            }
            else if((arr[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && arr[i][j] == pid)
                {
                    if2=true;
                    break;
                }
            }
        }
        if(if1 && if2){
            for(int i = x,j = y ; j < sizeboard && i > 0 ;i--,j++){
                if(arr[i][j] != pid){
                    arr[i][j] = pid;
                }
                else
                {
                    isSafe = true;
                    break;
                }
            }
        }
    }
    if(isSafe)
        return scorecalc(arr,2);
    else return 0;
}
bool board::changePiec(int x , int y,int pid=1)
{
    int personvsid = 2;
    if(pid == 2)
        personvsid = 1;
    bool isSafe = false;
    if(x > sizeboard || y > sizeboard || x < 0 || y < 0 || boardArr[x][y]!=0)
        return false;
    /*
     * * * * *
     * * * * *
     * * 2 * *
     * * * \ *
     * * * * \
    */
    if(x+1 < sizeboard && y+1 < sizeboard){
        bool if1 = false,if2 = false;
        for(int i = x+1,j = y+1 ; i < sizeboard && j < sizeboard;i++,j++){
            if(if1 == false && (boardArr[i][j] == personvsid)){
                if1 = true;
            }
            else if((boardArr[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && boardArr[i][j] == pid)
                {
                    if2=true;
                    break;
                }
            }
        }
        if(if1 && if2){
            for(int i = x+1,j = y+1 ; i < sizeboard && j < sizeboard;i++,j++){
                if(boardArr[i][j] != pid){
                    //if(boardArr[i][j] == comp.getId())
                    //    comp.scoreMines1();

                    boardArr[i][j] = pid;
                    //prsn.scoresum1();
                }
                else
                {
                    isSafe = true;
                    break;
                }
            }
        }
    }
    /*
     * * * * *
     * * * * *
     * * 2 * *
     * * | * *
     * * | * *
    */
    if(x+1 < sizeboard){
        bool if1 = false,if2 = false;
        for(int i = x+1,j = y ; i < sizeboard ;i++){
            if(if1 == false && (boardArr[i][j] == personvsid)){
                if1 = true;
            }
            else if((boardArr[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && boardArr[i][j] == pid)
                {
                    if2=true;
                    break;
                }
            }
        }
        if(if1 && if2){
            for(int i = x+1,j = y ; i < sizeboard ;i++){
                if(boardArr[i][j] != pid){
                    //if(boardArr[i][j] == comp.getId())
                    //    comp.scoreMines1();
                    boardArr[i][j] = pid;
                    //prsn.scoresum1();

                }
                else
                {
                    isSafe = true;
                    break;
                }
            }
        }
    }
    /*
     * * * * *
     * * * * *
     * * 2 - -
     * * * * *
     * * * * *
    */
    if(y+1 < sizeboard){
        bool if1 = false,if2 = false;
        for(int i = x,j = y+1 ; j < sizeboard ;j++){
            if(if1 == false && (boardArr[i][j] == personvsid)){
                if1 = true;
                continue;
            }
            else if((boardArr[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && boardArr[i][j] == pid)
                {
                    if2=true;
                    break;
                }
            }
        }
        if(if1 && if2){
            for(int i = x,j = y+1 ; j < sizeboard ;j++){
                if(boardArr[i][j] != pid){
                    //if(boardArr[i][j] == comp.getId())
                    //    comp.scoreMines1();
                    boardArr[i][j] = pid;
                    //prsn.scoresum1();
                }
                else
                {
                    isSafe = true;
                    break;
                }
            }
        }
    }
    /*
     \ * * * *
     * \ * * *
     * * 2 * *
     * * * * *
     * * * * *
    */
    if(x-1 > 0 && y-1>0){
        bool if1 = false,if2 = false;
        for(int i = x-1,j = y-1 ; i >= 0 && j >= 0 ;i--,j--){
            if(if1 == false && (boardArr[i][j] == personvsid)){
                if1 = true;
                continue;
            }
            else if((boardArr[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && boardArr[i][j] == pid)
                {
                    if2=true;
                    break;
                }
            }
        }
        if(if1 && if2){
            for(int i = x-1,j = y-1 ; i > 0 && j > 0 ;i--,j--){
                if(boardArr[i][j] != pid){
                    //if(boardArr[i][j] == comp.getId())
                    //    comp.scoreMines1();
                    boardArr[i][j] = pid;
                    //prsn.scoresum1();
                }
                else
                {
                    isSafe = true;
                    break;
                }
            }
        }
    }
    /*
     * * | * *
     * * | * *
     * * 2 * *
     * * * * *
     * * * * *
    */
    if(x-1 > 0){
        bool if1 = false,if2 = false;
        for(int i = x-1,j = y ; i >= 0 ;i--){
            if(if1 == false && (boardArr[i][j] == personvsid)){
                if1 = true;
                continue;
            }
            else if((boardArr[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && boardArr[i][j] == pid)
                {
                    if2=true;
                    break;
                }
            }
        }
        if(if1 && if2){
            for(int i = x-1,j = y ; i > 0 ;i--){
                if(boardArr[i][j] != pid){
                    //if(boardArr[i][j] == comp.getId())
                        //comp.scoreMines1();
                    boardArr[i][j] = pid;
                    //prsn.scoresum1();
                }
                else
                {
                    isSafe = true;
                    break;
                }
            }
        }
    }
    /*
     * * * * *
     * * * * *
     - - 2 * *
     * * * * *
     * * * * *
    */
    if(y-1 > 0){
        bool if1 = false,if2 = false;
        for(int i = x,j = y-1 ; j >= 0 ;j--){
            if(if1 == false && (boardArr[i][j] == personvsid)){
                if1 = true;
                continue;
            }
            else if((boardArr[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && boardArr[i][j] == pid)
                {
                    if2=true;
                    break;
                }
            }
        }
        if(if1 && if2){
            for(int i = x,j = y-1 ; j > 0 ;j--){
                if(boardArr[i][j] != pid){
                    //if(boardArr[i][j] == comp.getId())
                    //    comp.scoreMines1();
                    boardArr[i][j] = pid;
                    //prsn.scoresum1();
                }
                else
                {
                    isSafe = true;
                    break;
                }
            }
        }
    }
    /*
     * * * * *
     * * * * *
     * * 2 * *
     * / * * *
     / * * * *
    */
    if(x+1 < sizeboard && y-1 > 0)
    {
        bool if1 = false,if2 = false;
        for(int i = x+1,j = y-1 ; i < sizeboard && j >= 0 ;i++,j--){
            if(if1 == false && (boardArr[i][j] == personvsid)){
                if1 = true;
                continue;
            }
            else if((boardArr[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && boardArr[i][j] == pid)
                {
                    if2=true;
                    break;
                }
            }
        }
        if(if1 && if2){
            for(int i = x+1,j = y-1 ; j > 0 && i < sizeboard ;i++,j--){
                if(boardArr[i][j] != pid){
                    //if(boardArr[i][j] == comp.getId())
                    //   comp.scoreMines1();
                    boardArr[i][j] = pid;
                    //prsn.scoresum1();
                }
                else
                {
                    isSafe = true;
                    break;
                }
            }
        }
    }
    /*
     * * * * /
     * * * / *
     * * 2 * *
     * * * * *
     * * * * *
    */
    if(x-1 > 0 && y+1 < sizeboard)
    {
        bool if1 = false,if2 = false;
        for(int i = x-1,j = y+1 ; j < sizeboard && i >= 0 ;j++,i--){
            if(if1 == false && (boardArr[i][j] == personvsid)){
                if1 = true;
                continue;
            }
            else if((boardArr[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && boardArr[i][j] == pid)
                {
                    if2=true;
                    break;
                }
            }
        }
        if(if1 && if2){
            for(int i = x-1,j = y+1 ; j < sizeboard && i > 0 ;i--,j++){
                if(boardArr[i][j] != pid){
                    //if(boardArr[i][j] == comp.getId())
                    //    comp.scoreMines1();
                    boardArr[i][j] = pid;
                    //prsn.scoresum1();
                }
                else
                {
                    isSafe = true;
                    break;
                }
            }
        }
    }
    if(isSafe==true)
        boardArr[x][y] = pid;
    return isSafe;
}
bool board::changePiec(int arr[sizeboard][sizeboard],int x , int y,int pid=2)
{
    int personvsid = 2;
    if(pid == 2)
        personvsid = 1;
    bool isSafe = false;
    if(x > sizeboard || y > sizeboard || x < 0 || y < 0 || arr[x][y]!=0)
        return false;
    /*
     * * * * *
     * * * * *
     * * 2 * *
     * * * \ *
     * * * * \
    */
    if(x+1 < sizeboard && y+1 < sizeboard){
        bool if1 = false,if2 = false;
        for(int i = x+1,j = y+1 ; i < sizeboard && j < sizeboard;i++,j++){
            if(if1 == false && (arr[i][j] == personvsid)){
                if1 = true;
            }
            else if((arr[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && arr[i][j] == pid)
                {
                    if2=true;
                    break;
                }
            }
        }
        if(if1 && if2){
            for(int i = x+1,j = y+1 ; i < sizeboard && j < sizeboard;i++,j++){
                if(arr[i][j] != pid){
                    //if(arr[i][j] == comp.getId())
                    //    comp.scoreMines1();

                    arr[i][j] = pid;
                    //prsn.scoresum1();
                }
                else
                {
                    isSafe = true;
                    break;
                }
            }
        }
    }
    /*
     * * * * *
     * * * * *
     * * 2 * *
     * * | * *
     * * | * *
    */
    if(x+1 < sizeboard){
        bool if1 = false,if2 = false;
        for(int i = x+1,j = y ; i < sizeboard ;i++){
            if(if1 == false && (arr[i][j] == personvsid)){
                if1 = true;
            }
            else if((arr[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && arr[i][j] == pid)
                {
                    if2=true;
                    break;
                }
            }
        }
        if(if1 && if2){
            for(int i = x+1,j = y ; i < sizeboard ;i++){
                if(arr[i][j] != pid){
                    //if(arr[i][j] == comp.getId())
                    //    comp.scoreMines1();
                    arr[i][j] = pid;
                    //prsn.scoresum1();

                }
                else
                {
                    isSafe = true;
                    break;
                }
            }
        }
    }
    /*
     * * * * *
     * * * * *
     * * 2 - -
     * * * * *
     * * * * *
    */
    if(y+1 < sizeboard){
        bool if1 = false,if2 = false;
        for(int i = x,j = y+1 ; j < sizeboard ;j++){
            if(if1 == false && (arr[i][j] == personvsid)){
                if1 = true;
                continue;
            }
            else if((arr[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && arr[i][j] == pid)
                {
                    if2=true;
                    break;
                }
            }
        }
        if(if1 && if2){
            for(int i = x,j = y+1 ; j < sizeboard ;j++){
                if(arr[i][j] != pid){
                    //if(arr[i][j] == comp.getId())
                    //    comp.scoreMines1();
                    arr[i][j] = pid;
                    //prsn.scoresum1();
                }
                else
                {
                    isSafe = true;
                    break;
                }
            }
        }
    }
    /*
     \ * * * *
     * \ * * *
     * * 2 * *
     * * * * *
     * * * * *
    */
    if(x-1 > 0 && y-1>0){
        bool if1 = false,if2 = false;
        for(int i = x-1,j = y-1 ; i >= 0 && j >= 0 ;i--,j--){
            if(if1 == false && (arr[i][j] == personvsid)){
                if1 = true;
                continue;
            }
            else if((arr[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && arr[i][j] == pid)
                {
                    if2=true;
                    break;
                }
            }
        }
        if(if1 && if2){
            for(int i = x-1,j = y-1 ; i > 0 && j > 0 ;i--,j--){
                if(arr[i][j] != pid){
                    //if(arr[i][j] == comp.getId())
                    //    comp.scoreMines1();
                    arr[i][j] = pid;
                    //prsn.scoresum1();
                }
                else
                {
                    isSafe = true;
                    break;
                }
            }
        }
    }
    /*
     * * | * *
     * * | * *
     * * 2 * *
     * * * * *
     * * * * *
    */
    if(x-1 > 0){
        bool if1 = false,if2 = false;
        for(int i = x-1,j = y ; i >= 0 ;i--){
            if(if1 == false && (arr[i][j] == personvsid)){
                if1 = true;
                continue;
            }
            else if((arr[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && arr[i][j] == pid)
                {
                    if2=true;
                    break;
                }
            }
        }
        if(if1 && if2){
            for(int i = x-1,j = y ; i > 0 ;i--){
                if(arr[i][j] != pid){
                    //if(arr[i][j] == comp.getId())
                        //comp.scoreMines1();
                    arr[i][j] = pid;
                    //prsn.scoresum1();
                }
                else
                {
                    isSafe = true;
                    break;
                }
            }
        }
    }
    /*
     * * * * *
     * * * * *
     - - 2 * *
     * * * * *
     * * * * *
    */
    if(y-1 > 0){
        bool if1 = false,if2 = false;
        for(int i = x,j = y-1 ; j >= 0 ;j--){
            if(if1 == false && (arr[i][j] == personvsid)){
                if1 = true;
                continue;
            }
            else if((arr[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && arr[i][j] == pid)
                {
                    if2=true;
                    break;
                }
            }
        }
        if(if1 && if2){
            for(int i = x,j = y-1 ; j > 0 ;j--){
                if(arr[i][j] != pid){
                    //if(arr[i][j] == comp.getId())
                    //    comp.scoreMines1();
                    arr[i][j] = pid;
                    //prsn.scoresum1();
                }
                else
                {
                    isSafe = true;
                    break;
                }
            }
        }
    }
    /*
     * * * * *
     * * * * *
     * * 2 * *
     * / * * *
     / * * * *
    */
    if(x+1 < sizeboard && y-1 > 0)
    {
        bool if1 = false,if2 = false;
        for(int i = x+1,j = y-1 ; i < sizeboard && j >= 0 ;i++,j--){
            if(if1 == false && (arr[i][j] == personvsid)){
                if1 = true;
                continue;
            }
            else if((arr[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && arr[i][j] == pid)
                {
                    if2=true;
                    break;
                }
            }
        }
        if(if1 && if2){
            for(int i = x+1,j = y-1 ; j > 0 && i < sizeboard ;i++,j--){
                if(arr[i][j] != pid){
                    //if(arr[i][j] == comp.getId())
                    //   comp.scoreMines1();
                    arr[i][j] = pid;
                    //prsn.scoresum1();
                }
                else
                {
                    isSafe = true;
                    break;
                }
            }
        }
    }
    /*
     * * * * /
     * * * / *
     * * 2 * *
     * * * * *
     * * * * *
    */
    if(x-1 > 0 && y+1 < sizeboard)
    {
        bool if1 = false,if2 = false;
        for(int i = x-1,j = y+1 ; j < sizeboard && i >= 0 ;j++,i--){
            if(if1 == false && (arr[i][j] == personvsid)){
                if1 = true;
                continue;
            }
            else if((arr[i][j] == 0 && if1 == true)||if1==false)
                break;
            else
            {
                if(if1 == true && if2 == false && arr[i][j] == pid)
                {
                    if2=true;
                    break;
                }
            }
        }
        if(if1 && if2){
            for(int i = x-1,j = y+1 ; j < sizeboard && i > 0 ;i--,j++){
                if(arr[i][j] != pid){
                    //if(arr[i][j] == comp.getId())
                    //    comp.scoreMines1();
                    arr[i][j] = pid;
                    //prsn.scoresum1();
                }
                else
                {
                    isSafe = true;
                    break;
                }
            }
        }
    }
    if(isSafe==true)
        arr[x][y] = pid;
    return isSafe;
}

