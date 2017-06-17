#include<iostream>
#include<stdio.h>
#include<climits>
#include<vector>
#include<stdlib.h>
#include<time.h>

#ifdef __linux__
    #define clrscr system("clear")
#else
    #define clrscr system("cls")
#endif

using namespace std;

const int n = 3;
int user=1,bot=-1,draw=0,diff=2;//use 1 n -1 for 2 plyers ... turn switching i easy ... turn*=-1

struct tictac
{
    int board[n][n];
	void printBoard();
	int wincheck(int board[n][n]);
};

void tictac::printBoard()
{
	cout<<"\n\nBOARD: \n\n";
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			if(board[i][j] == user) cout<<" O ";
			else if(board[i][j] == bot) cout<<" X ";
			else cout<<"   ";
			if(j != n-1) cout<<"|";//dont print at last
		}
		if(i!=n-1) cout<<"\n------------\n";//dont print at last
	}
	cout<<"\n";
}

int tictac::wincheck(int board[n][n])
{
    //user win conditions
	if     (board[0][0]==user && board[0][1]==user && board[0][2]==user) return user;
    else if(board[1][0]==user && board[1][1]==user && board[1][2]==user) return user;
    else if(board[2][0]==user && board[2][1]==user && board[2][2]==user) return user;
    else if(board[0][0]==user && board[1][0]==user && board[2][0]==user) return user;
    else if(board[0][1]==user && board[1][1]==user && board[2][1]==user) return user;
    else if(board[0][2]==user && board[1][2]==user && board[2][2]==user) return user;
    else if(board[0][0]==user && board[1][1]==user && board[2][2]==user) return user;
    else if(board[0][2]==user && board[1][1]==user && board[2][0]==user) return user;
    
    //bot win conditions
    else if(board[0][0]==bot && board[0][1]==bot && board[0][2]==bot) return bot;
    else if(board[1][0]==bot && board[1][1]==bot && board[1][2]==bot) return bot;
    else if(board[2][0]==bot && board[2][1]==bot && board[2][2]==bot) return bot;
    else if(board[0][0]==bot && board[1][0]==bot && board[2][0]==bot) return bot;
    else if(board[0][1]==bot && board[1][1]==bot && board[2][1]==bot) return bot;
    else if(board[0][2]==bot && board[1][2]==bot && board[2][2]==bot) return bot;
    else if(board[0][0]==bot && board[1][1]==bot && board[2][2]==bot) return bot;
    else if(board[0][2]==bot && board[1][1]==bot && board[2][0]==bot) return bot;
    
    else return 0;
}

int user_turn(tictac &tt,int countfill);//declaration

void bot_turn_init(tictac &tt,int countfill,int &ipos,int &jpos)//-1
{
    vector<pair<int,int> > best;
    pair<int,int> localmove;
    int marks[3][3];
    /*  2 = win on first turn
     *  1 = win sure but not now
     *  0 = draw
     * -1 = opponent win
     * -2 = already occupied
     */
    countfill++;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(tt.board[i][j]!=0)
                marks[i][j]=-2;//-2 means not feasible
            else if(tt.board[i][j]==0)
            {
                tt.board[i][j]=bot;//try that move in mind....put finger on that place
                if(tt.wincheck(tt.board)==bot)//is bot winning
                    marks[i][j]=2;//2 marks means instant win
                else if(countfill==9)//jita nhi but draw
                    marks[i][j]=0;
                else if(tt.wincheck(tt.board)==0)//na jite na hi draw hu ......
                {
                    int res=user_turn(tt,countfill);
                    if(res==bot)
                        marks[i][j]=1;
                    if(res==user)
                        marks[i][j]=-1;
                    if(res==draw)
                        marks[i][j]=0;
                }
                tt.board[i][j]=0;//pick up finger
            }
        }   
    }//end checking all board
    
    for(int k=2;k>=-1;k--)
    {
        for(int i=0;i<n;i++)  
        {
            for(int j=0;j<n;j++)
            {   
                if(marks[i][j]==k)
                {
                    localmove.first=i;
                    localmove.second=j;
                    best.push_back(localmove);
                }
            }
        }
        if(best.size()>0 && k<=diff)
            break;
    }
    
    //randomness
    srand(100);
    time_t sec;
    time(&sec);
    srand(sec);
    int turn=abs(rand())%best.size();
    
    if(diff==-2 && turn < best.size()-1)//reduce wining chances
        turn++;

    ipos=best[turn].first,jpos=best[turn].second;
}

int bot_turn(tictac &tt,int countfill)//-1
{
    int marks[3][3];
    countfill++;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(tt.board[i][j]!=0)
                marks[i][j]=-2;//-2 means not feasible
            else if(tt.board[i][j]==0)
            {
                tt.board[i][j]=bot;//try that move
                if(tt.wincheck(tt.board)==bot)//is bot winning
                {
                    tt.board[i][j]=0;
                    return bot;
                }
                else if(countfill==9)//jita nhi but draw
                {
                    tt.board[i][j]=0;
                    return draw;
                }
                else if(tt.wincheck(tt.board)==0)//na jite na hi draw hu ......
                {
                    int res=user_turn(tt,countfill);
                    if(res==bot)
                    {
                        tt.board[i][j]=0;
                        return bot;
                    }
                    if(res==user)
                        marks[i][j]=-1;
                    if(res==draw)
                        marks[i][j]=0;
                    tt.board[i][j]=0;
                }
            }
        }   
    }//end checking all board
    
    for(int i=0;i<n;i++)  
        for(int j=0;j<n;j++)
            if(marks[i][j]==0)
                return draw;
    
    return user;
}

int user_turn(tictac &tt,int countfill)//1
{
    vector<pair<int,int> > best;
    pair<int,int> localmove;
    int marks[3][3];
    countfill++;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(tt.board[i][j]!=0)
                marks[i][j]=-2;//-2 means not feasible
            else if(tt.board[i][j]==0)
            {
                tt.board[i][j]=user;//try that move
                if(tt.wincheck(tt.board)==user)//is user winning
                {
                    tt.board[i][j]=0;
                    return user;
                }
                else if(countfill==9)
                {
                    tt.board[i][j]=0;
                    return draw;
                }
                else if(tt.wincheck(tt.board)==0)//na jite na hi draw hu ......
                {
                    int res=bot_turn(tt,countfill);
                    if(res==user)
                    {
                        tt.board[i][j]=0;
                        return user;
                    }
                    else if(res==bot)
                        marks[i][j]=-1;
                    else if(res==draw)
                        marks[i][j]=0;
                }
                tt.board[i][j]=0;//do board 0
            }
        }   
    }//end check all board but no winning chance
    
    for(int i=0;i<n;i++)  
        for(int j=0;j<n;j++)
            if(marks[i][j]==0)
                return draw;
                
    return bot;
}

//added neetu's comment
// nice game
int main()
{
	tictac tt;
	
	for(int i=0;i<n;i++)//mark every plce 0
		for(int j=0;j<n;j++)
			tt.board[i][j] = 0;
	
	int turn = user , decide=1;
	int ipos, jpos, countfill = 0;//count fill counts num of moves
	
	cout<<"enter difficulty level -2 , -1 , 0 , 1 , 2 : ";
	cin>>diff;
	cout<<"enter -1 for bot turn first : ";
	cin>>decide;
	if(decide==-1) turn=-1;
	
	while(1)
	{
		if(turn == user)
		{
			cout<<"\nEnter the position to mark : ";
			cin >> ipos >> jpos;
		}
		else // bot turn
		{
			clrscr ;
			bot_turn_init(tt,countfill,ipos,jpos);
		}
		if( (ipos>=0 && ipos<n) && (jpos>=0 && jpos<n))//check validty
		{
			if(tt.board[ipos][jpos] == 0)//check position empty or not
			{
				tt.board[ipos][jpos] = turn;
				
				// if win condition comes, then end the game.
				int result = tt.wincheck(tt.board);//returns 0 for not winning conditiion
				
				turn*=-1;//switches turn
				countfill++;
				tt.printBoard();
				if(result!= 0)
				{
					if(result == user) cout<<"You WON\n";
					else cout<<"The bot wins\n";
					break;
				}
				else if(countfill == 9)
				{
					cout<<"GAME DRAWN\n";
					break;
				}	
			}
			else cout<<"position not empty, move again\n";
		}
		else cout<<"Invalid move, move again\n";
	}//end while
	
	return 0;
}
