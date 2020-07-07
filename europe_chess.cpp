#include<iostream>
#include<windows.h>
#include<conio.h>
#include<vector>
#include<algorithm>
#include<fstream>
#include<ctime>
using namespace std;
HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);
HWND hWnd=GetConsoleWindow();
char KeyPressed;
const string CountryName[20]={"英","荷","比","法","意","丹","联","希","土","芬","瑞","奥","民","南","阿","波","捷","匈","罗","保"};
const int CountryFaction[20]={1,1,1,1,1,1,1,1,1,0,0,0,2,2,2,2,2,2,2,2};
class Chess{
	public:
		int index,faction,status;
		Chess(){
			index=0;
			faction=0;
			status=0;
		}
		Chess operator=(const Chess &C){
			index=C.index;
			faction=C.faction;
			status=C.status;
		}
};
Chess MakeChess(int i,int f,int s){
	Chess TempChess;
	TempChess.index=i;
	TempChess.faction=f;
	TempChess.status=s;
	return TempChess;
}
void Color(int f,int b){
	SetConsoleTextAttribute(hOut,f*16+b);
}
void SetWindow(short r,short c){
	COORD size={c,100};
	SMALL_RECT rc;
	RECT rc2;
	rc.Right=c-1;
	rc.Bottom=r-1;
	rc.Left=rc.Top=0;
	SetConsoleTitle("冷战?! 前传: 欧洲棋 V1.0.0");
	SetConsoleWindowInfo(hOut,true,&rc);
	SetConsoleScreenBufferSize(hOut,size);
	GetWindowRect(hWnd, &rc2);
	SetWindowLongPtr(hWnd,GWL_STYLE,GetWindowLong(hWnd,GWL_STYLE)&~WS_THICKFRAME&~WS_MAXIMIZEBOX&~WS_MINIMIZEBOX);
	SetWindowPos(hWnd,NULL,rc2.left,rc2.top,rc2.right-rc2.left,rc2.bottom-rc2.top,NULL);
}
void HideCursor(){
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut,&cci);
	cci.bVisible=false;
	SetConsoleCursorInfo(hOut,&cci);
}
bool SetCursorPosition(short x,short y){
    return SetConsoleCursorPosition(hOut,(COORD){y,x});
}
COORD GetCursorPosition(){
    CONSOLE_SCREEN_BUFFER_INFO sbi;
    GetConsoleScreenBufferInfo(hOut,&sbi);
    return sbi.dwCursorPosition;
}
bool AttackCheck(int X,int Y,int PlayerRole,Chess GameBoard[4][5]){
	if(GameBoard[Y][X].status==0)return 0;
	if(GameBoard[Y][X].status==2&&GameBoard[Y][X].faction==PlayerRole)return 0;
	if(X!=0&&GameBoard[Y][X-1].status==2&&GameBoard[Y][X-1].faction==PlayerRole)return 1;
	if(X!=4&&GameBoard[Y][X+1].status==2&&GameBoard[Y][X+1].faction==PlayerRole)return 1;
	if(Y!=0&&GameBoard[Y-1][X].status==2&&GameBoard[Y-1][X].faction==PlayerRole)return 1;
	if(Y!=3&&GameBoard[Y+1][X].status==2&&GameBoard[Y+1][X].faction==PlayerRole)return 1;
	return 0;
}
bool PropagandaCheck(int X,int Y,int PlayerRole,Chess GameBoard[4][5]){
	if(GameBoard[Y][X].status==0)return 0;
	if(GameBoard[Y][X].faction!=0)return 0;
	if(X!=0&&GameBoard[Y][X-1].status==2&&GameBoard[Y][X-1].faction==PlayerRole)return 1;
	if(X!=4&&GameBoard[Y][X+1].status==2&&GameBoard[Y][X+1].faction==PlayerRole)return 1;
	if(Y!=0&&GameBoard[Y-1][X].status==2&&GameBoard[Y-1][X].faction==PlayerRole)return 1;
	if(Y!=3&&GameBoard[Y+1][X].status==2&&GameBoard[Y+1][X].faction==PlayerRole)return 1;
 	return 0;
}
void StartGame(){
	srand(time(NULL));
	vector<int>Deck;
	Chess Board[4][5];
	bool Player1Humanity=1,Player2Humanity=1;
	string Player1Name,Player2Name;
	int SovietPlayer=1,TurnPlayer;
	int Flag1,Flag2,Flag3,Flag4;
	int PosX,PosY;
	while(1){
		system("cls");
		cout<<"玩家1:"<<(Player1Humanity?"人类1":"电脑1")<<" 玩家2:"<<(Player2Humanity?"人类2":"电脑2")<<"\n<1>修改玩家1 <2>修改玩家2 <0>确认";
		do KeyPressed=getch();
		while(KeyPressed<'0'||KeyPressed>'2');
		if(KeyPressed=='0')break;
		if(KeyPressed=='1')Player1Humanity=!Player1Humanity;
		else Player2Humanity=!Player2Humanity;
	}
	system("cls");
	cout<<"玩家1("<<(Player1Humanity?"人类1":"电脑1")<<")的名称:";
	getline(cin,Player1Name);
	cout<<"玩家2("<<(Player2Humanity?"人类2":"电脑2")<<")的名称:";
	getline(cin,Player2Name);
	while(1){
		system("cls");
		cout<<"玩家1:"<<(Player1Humanity?"人类1":"电脑1")<<' '<<Player1Name<<" 玩家2:"<<(Player2Humanity?"人类2":"电脑2")<<' '<<Player2Name<<"\n现在的苏联方:玩家"<<SovietPlayer<<"\n<1>玩家1为苏联方 <2>玩家2为苏联方 <0>确认";
		do KeyPressed=getch();
		while(KeyPressed<'0'||KeyPressed>'2');
		if(KeyPressed=='0')break;
		if(KeyPressed=='1')SovietPlayer=1;
		else SovietPlayer=2;
	}
	TurnPlayer=SovietPlayer;
	if(!(Player1Humanity&&Player2Humanity)){
		system("cls");
		cout<<"暂不支持电脑AI，敬请期待!";
		getch();
		return;
	}
	for(int i=0;i<20;i++)Deck.push_back(i);
	while(1){
		random_shuffle(Deck.begin(),Deck.end());
		for(int i=0;i<Deck.size();i++)Board[i/5][i%5]=MakeChess(Deck[i],CountryFaction[Deck[i]],1);
		if(Deck.size()!=20)for(int i=Deck.size();i<20;i++)Board[i/5][i%5]=MakeChess(0,0,0);
		while(1){
			Color(0,15);
			system("cls");
			cout<<"   01 02 03 04 05\n";
			for(int i=0;i<4;i++){
				Color(0,15);
				cout<<'0'<<i+1;
				for(int j=0;j<5;j++){
					cout<<' ';
					if(Board[i][j].status==1){
						Color(0,8);
						cout<<"--";
					}else if(Board[i][j].status==0){
						Color(0,8);
						cout<<"  ";
					}else if(Board[i][j].faction==1){
						Color(0,9);
						cout<<CountryName[Board[i][j].index];
					}else if(Board[i][j].faction==2){
						Color(0,12);
						cout<<CountryName[Board[i][j].index];
					}else{
						Color(0,15);
						cout<<CountryName[Board[i][j].index];
					}
				}
				cout<<'\n';
			}
			Color(0,15);
			cout<<"轮到玩家"<<TurnPlayer<<(TurnPlayer==SovietPlayer?"苏方":"美方")<<"了:";
			PosX=0;
			PosY=0;
			while(1){
				SetCursorPosition(PosY+1,3*PosX+3);
				if(Board[PosY][PosX].status==1){
					Color(15,8);
					cout<<"--";
				}else if(Board[PosY][PosX].status==0){
					Color(15,8);
					cout<<"  ";
				}else if(Board[PosY][PosX].faction==1){
					Color(15,9);
					cout<<CountryName[Board[PosY][PosX].index];
				}else if(Board[PosY][PosX].faction==2){
					Color(15,12);
					cout<<CountryName[Board[PosY][PosX].index];
				}else{
					Color(15,0);
					cout<<CountryName[Board[PosY][PosX].index];
				}
				KeyPressed=getch();
				if(KeyPressed=='w'||KeyPressed=='a'||KeyPressed=='s'||KeyPressed=='d'){
					SetCursorPosition(PosY+1,3*PosX+3);
					if(Board[PosY][PosX].status==1){
						Color(0,8);
						cout<<"--";
					}else if(Board[PosY][PosX].status==0){
						Color(0,8);
						cout<<"  ";
					}else if(Board[PosY][PosX].faction==1){
						Color(0,9);
						cout<<CountryName[Board[PosY][PosX].index];
					}else if(Board[PosY][PosX].faction==2){
						Color(0,12);
						cout<<CountryName[Board[PosY][PosX].index];
					}else{
						Color(0,15);
						cout<<CountryName[Board[PosY][PosX].index];
					}
					switch(KeyPressed){
						case 'w':{
							PosY=(PosY+3)%4;
							break;
						}
						case 'a':{
							PosX=(PosX+4)%5;
							break;
						}
						case 's':{
							PosY=(PosY+1)%4;
							break;
						}
						case 'd':{
							PosX=(PosX+1)%5;
							break;
						}
					}
				}
				if(KeyPressed=='i'||KeyPressed=='o'||KeyPressed=='p'){
					if(KeyPressed=='i'&&Board[PosY][PosX].status==1){
						Board[PosY][PosX].status=2;
						break;
					}
					if(KeyPressed=='o'&&AttackCheck(PosX,PosY,(SovietPlayer+TurnPlayer-1)%2+1,Board)){
						Color(0,15);
						SetCursorPosition(6,0);
						if(rand()%2){
							cout<<"成功!你消灭了"<<CountryName[Board[PosY][PosX].index]<<"国";
							Board[PosY][PosX].status=0;
						}else{
							cout<<"失败!";
							if(Board[PosY][PosX].status==2)cout<<"你没能消灭"<<CountryName[Board[PosY][PosX].index]<<"国";
						}
						getch();
						break;
					}
					if(KeyPressed=='p'&&PropagandaCheck(PosX,PosY,(SovietPlayer+TurnPlayer-1)%2+1,Board)){
						Board[PosY][PosX].faction=(SovietPlayer+TurnPlayer-1)%2+1;
						break;
					}
				}
			}
			TurnPlayer=TurnPlayer%2+1;
			Flag1=0;
			Flag4=1;
			for(int i=0;i<4;i++)for(int j=0;j<5;j++){
				if(Board[i][j].status==1){
					Flag1++;
					Flag2=i;
					Flag3=j;
					Flag4=0;
				}
				if(PropagandaCheck(j,i,(SovietPlayer+TurnPlayer-1)%2+1,Board)||AttackCheck(j,i,(SovietPlayer+TurnPlayer-1)%2+1,Board))Flag4=0;
			}
			if(Flag1==1)Board[Flag2][Flag3].status=2;
			if(Flag4)break;
		}
		Flag1=0;
		Flag2=0;
		for(int i=0;i<4;i++)for(int j=0;j<5;j++)if(Board[i][j].status){
			if(CountryFaction[Board[i][j].index]==1)Flag1++;
			if(CountryFaction[Board[i][j].index]==2)Flag2++;
		}
		if(Flag1==0||Flag2==0)break;
		Deck.clear();
		cout<<Deck.size();
		for(int i=0;i<4;i++)for(int j=0;j<5;j++)if(Board[i][j].status)Deck.push_back(Board[i][j].index);
	}
	Color(0,15);
	system("cls");
	cout<<"游戏结束\n";
	if(Flag1==0&&Flag2==0)cout<<"平局";
	else if(Flag1==0)cout<<"玩家1:"<<(Player1Humanity?"人类1":"电脑1")<<' '<<Player1Name<<"获胜";
	else cout<<"玩家2:"<<(Player2Humanity?"人类2":"电脑2")<<' '<<Player2Name<<"获胜";
	getch();
}
int main(){
	SetWindow(48,100);
	HideCursor();
	while(1){
		Color(0,15);
		system("cls");
		cout<<"冷战?! 前传: 欧洲棋\n<1>开始游戏 <2>退出游戏";
		do KeyPressed=getch();
		while(KeyPressed!='1'&&KeyPressed!='2');
		if(KeyPressed=='2'){
			system("cls");
			cout<<"你真的要离开吗?\n<1>离开 <2>留下";
			do KeyPressed=getch();
			while(KeyPressed!='1'&&KeyPressed!='2');
			if(KeyPressed=='1')return 0;
			continue;
		}
		StartGame();
	}
}
