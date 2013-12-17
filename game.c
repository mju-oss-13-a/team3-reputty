nclude <iostream>

#include <stdio.h>

#include <windows.h>

#include <conio.h>

#include <stdlib.h>

#include <time.h>

#include <string>

#include <fstream>

using namespace std;



/*°ª Á€ÀÇ*/

#define ranking 6 

#define xpos 20		// xÁÂÇ¥ °ª

#define ypos 20		// yÁÂÇ¥ °ª

#define UP 72

#define DOWN 80

#define LEFT 75

#define RIGHT 77

#define apple_views 4  // »ç°ú ÃâÇöÇÏŽÂ ÈœŒö

#define box_views 1    // ÀåŸÖ¹° ÃâÇöÇÏŽÂ ÈœŒö



enum Pattern{¡à,¡á,¡Ú,£À,¢Ë,¢Ã,¡Ü}; // È­žé¿¡ ³ªÅž³ªŽÂ Æ¯Œö¹®ÀÚ Á€ÀÇ



// °ÔÀÓ È­žé ±žŒºÀ» À§ÇÑ Å¬·¡œº

class Background_Map

{

public:

	int arr[ypos][xpos];

	int score;

	int bonus;

	int total;

	int exit;

	int winner;

};



// ·©Å· Å¬·¡œº

class Snake_rank

{

public:

	string name;

	int score;

	string level;

};



// ¹ì žÓž® ÁÂÇ¥

class Snake_Head

{

public:

	int head_x;

	int head_y;

};



// º° ÁÂÇ¥

class Snake_Star

{

public:

	int star_x;

	int star_y;

};



//ºí¶óÀÎµå ÁÂÇ¥

class Snake_wall

{

public:

	int wall_x;

	int wall_y;

};



// ¹ì ²¿ž® ÁÂÇ¥

class Snake_Tail

{

public:

	int tail_x;

	int tail_y;

};



// »ç°ú ÁÂÇ¥

class Snake_Apple

{

public:

	int apple_x;

	int apple_y;

};



// Àü¿ª º¯Œö ¹× ÇÔŒö Œ±ŸðºÎ

Background_Map bgm;

Snake_Head shd;

Snake_Head shd2;

Snake_Star ssr;

Snake_Tail stl[1000];  //²¿ž®ÀÇ °¹Œö

Snake_Apple sae;

Snake_rank srk[ranking];

Snake_wall sw;



char Keyboard;

int Tail;

int h_score = 0;

//int h_score2 = 0;

int boxviews = 0;

int box = 0;

int appleviews = 0;

//int coma=0;

int delay;

int tail_winner;

string leveltext;

string username;

time_t start_time,end_time;



void gotoxy(int x,int y);

void Snake_Cotinue(void);

void Snake_Level(void);

void Snake_Default(void);

void Game_key(void);

void Snake_Control(void);

void Snake_screen2(void);

void Ranking_Score(void);

void Ranking_print(void);

void Search_Star(void);

void Obstacle_Box(void);

int Search_Box(int x,int y);

void Search_Apple(void);

void Generate_Apple(void);

int High_Search_Apple(int x,int y);

void Generate_Star(void);

int High_Search_Star(int x,int y);

void Snake_Move(void);

void Game_Score(void);

void Snake_TMove(void);

void Game_Over(void);

void Game_Winner(void);

//·©Å· œºÄÚŸî Ã³ž® ºÎºÐ

void Ranking_Score(void)

{

	if(srk[ranking].score <= bgm.total) {

		for( int i=0; i<ranking; i++){

			if(bgm.score >= srk[i].score){

				for( int j=ranking-1; j>i ; j--){

					srk[j].name=srk[j-1].name;

					srk[j].score=srk[j-1].score;

					srk[j].level=srk[j-1].level;

				}

				srk[i].score = bgm.total;

				srk[i].name = username;

				srk[i].level = leveltext;

				break;

			}

		}

	}

	ofstream os;

	os.open("ranking.txt");

	if( os.fail() ){

		cerr << "File open warning" << endl;

		exit(1);

	}

	int line_number=1;

	char names[20]={0};

	char levels[20]={0};

	

	for(int p=0; p<ranking-1; p++){

		strcpy_s(names, srk[p].name.c_str());

		strcpy_s(levels, srk[p].level.c_str());

		os<<line_number<< ". "<<names<<"   "<<srk[p].score<<"   "<<srk[p].level<<endl;

		line_number++;

	}

	os.close();

}



//·©Å·, œºÄÚŸî Ãâ·Â

void Ranking_print(void){

	cout<<endl;

	cout<<"========  Ranking  ========"<<endl;



	ifstream is;

	is.open("ranking.txt");

	if( !is ) { // ! ¿¬»êÀÚ ¿À¹ö·Îµù

		cerr << "Fail open file" << endl;

		exit( 1 );

	}

	char c;

	is.get(c); // ÇÏ³ªÀÇ ¹®ÀÚžŠ ÀÐŽÂŽÙ.

	while(! is.eof() )

	{

		cout << c;

		is.get(c);

	}

	is.close();

}

int player=1;

//œºÅ©ž° žÞŽº Œ³Á€

void Snake_screen2(void)

{

	int num;

	

	cout<<endl;

	cout<<"                         1. 1 player GameStart"<<endl<<endl;

	cout<<"                         2. 2 player GameStart"<<endl<<endl;

	cout<<"                         3. Score"<<endl<<endl;

	cout<<"                         4. Exit"<<endl<<endl;

	cout<<"                       Select number : ";

	cin>>num;

	switch(num)

	{

	case 1:

		break;

	case 2:

		player=2;

		break;

	case 3:

		system("cls");

		Ranking_print();

		cout<<endl;

		cout<<"                  Press any key"<<endl;

		_getch();

		system("cls");

		Snake_Cotinue();

		Snake_screen2();

		break;

	case 4:

		exit(1);

		break;

	}

}



//ÁÂÇ¥ Œ³Á€

void gotoxy(int x,int y)

{

	COORD pos = { x*2+1 , y+1 };

	

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);

}



//³­ÀÌµµ Œ³Á€

void Snake_Level(void)

{

	int level;

	

	system("cls");

	cout << "                     < Select level >" << endl<<endl;

	cout<<"                         1. EASY"<<endl<<endl;

	cout<<"                         2. NORMAL"<<endl<<endl;

	cout<<"                         3. HARD"<<endl<<endl;

	cout<<"                         4. EXPERT"<<endl<<endl;

	cout<<"                       Select number : ";

	cin >> level;

	switch(level) {

		case 1:

			delay = 120;

			leveltext = "EASY";

			tail_winner=20;

			break;

		case 2:

			delay = 80;

			leveltext = "NORMAL";

			tail_winner=30;

			break;

		case 3:

			delay = 50;

			leveltext = "HARD";

			tail_winner=40;

			break;

		case 4:

			delay = 10;

			leveltext = "EXPERT";

			tail_winner=50;

			break;

	}

}





//ÀÎÆ®·Î žÞŒŒÁö ÇÔŒö

void Snake_Cotinue(void)

{

	cout << "======================================================================" << endl;

	cout << "    +++++++++++++                                                     " << endl;

	cout << "  +++++++++++++++++                                                   " << endl;

	cout << " +++++                                                                " << endl;

	cout << "   +++++                                                              " << endl;

	cout << "     +++++                                                            " << endl;

	cout << "       +++++          +++                                             " << endl;

	cout << "         +++++        ++++++++     ++     ++   ++     ++              " << endl;

	cout << "           ++++++     ++++++++   +++ ++   ++ ++    +++   +++          " << endl;

	cout << "             +++++    ++    ++   +++  +   ++++     ++++++++++         " << endl;

	cout << "    ++++++++++++++++  ++    ++   +++  +   ++ ++    ++      +          " << endl;

	cout << "      +++++++++++     ++    ++    +++++++ ++   ++   +++++++           " << endl;

	cout << "======================================================================" << endl;

}



//°ÔÀÓŒ³Á€ ÃÊ±âÈ­

void Snake_Default(void)

{

	int i,j;



	system("cls");

	time(&start_time);

	cout << ",,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,," << endl;

	cout << "|                                        |" << endl;

	cout << "|                                        |" << endl;

	cout << "|                                        |" << endl;

	cout << "|                                        |" << endl;

	cout << "|                                        |" << endl;

	cout << "|                                        |" << endl;

	cout << "|                                        |" << endl;

	cout << "|                                        |" << endl;

	cout << "|                                        |" << endl;

	cout << "|                                        |" << endl;

	cout << "|                                        |" << endl;

	cout << "|                                        |" << endl;

	cout << "|                                        |" << endl;

	cout << "|                                        |" << endl;

	cout << "|                                        |" << endl;

	cout << "|                                        |" << endl;

	cout << "|                                        |" << endl;

	cout << "|                                        |" << endl;

	cout << "|                                        |" << endl;

	cout << "|                                        |" << endl;

	cout << "``````````````````````````````````````````" << endl;

	



	for(i=0;i<ypos;i++)

	{

		for(j=0;j<xpos;j++)

		{

			bgm.arr[i][j] = 0; //¡à

		}

	}

	for(i=0;i<1000;i++)

	{

		stl[i].tail_x=999;

		stl[i].tail_y=999;

	}



	shd.head_x = xpos/2;

	shd.head_y = ypos/2;



	sae.apple_x = 999;

	sae.apple_y = 999;



	bgm.winner = 0;

	bgm.exit  = 0;

	bgm.score = 0;

	Keyboard = UP;

	Tail = 0;

	boxviews = 0;

	appleviews = 0;

	box = 0;

	//coma=0;





}



//Å°Á¶ÀÛ ÇÔŒö

void Game_key(void)

{

	switch(_getch())

	{

	case UP:

		//if(coma!=DOWN) 

		Keyboard = UP;

		break;

	case DOWN:

		//if(coma!=UP) 

		Keyboard = DOWN;

		break;

	case LEFT:

		//if(coma!=RIGHT) 

		Keyboard = LEFT;

		break;

	case RIGHT:

		//if(coma!=LEFT) 

		Keyboard = RIGHT;

		break;

	/*case 119:

		Keyboard = UP1;

		break;

	case 115:

		//if(coma!=UP) 

		Keyboard = DOWN1;

		break;

	case 97:

		//if(coma!=RIGHT) 

		Keyboard = LEFT1;

		break;

	case 100:

		//if(coma!=LEFT) 

		Keyboard = RIGHT1;

		break;*/

	}



}



//œº³×ÀÌÅ© ÄÁÆ®·Ñ ÇÔŒö

void Snake_Control(void){

	Search_Star();

	Obstacle_Box();

	Search_Apple();

	Snake_Move();

	Game_Score();

	Snake_TMove();

	Game_Over();

	Game_Winner();

}



//¢Ã ÀåŸÖ¹° »ýŒº¹× °Ë»ö

void Obstacle_Box(void)

{

	int x,y;



	if(boxviews==box_views) //ÈœŒö

	{

		while(1)

		{

			x=rand()%xpos;

			y=rand()%ypos;

			boxviews = 0;



			if(bgm.arr[y][x] == 0 && Search_Box(x,y)==1) //¡à

			{

				bgm.arr[y][x] = 5; //¢Ã

				gotoxy(x,y);      

				cout << "¢Ã";       //Ãâ·Â

				box++;

				break;

			}

		}



	}



}



int Search_Box(int x,int y)

{

	if(Keyboard==UP)

	{

		if(x == shd.head_x && y == shd.head_y-1)

			return 0;

		else if(x == shd.head_x && y == shd.head_y-2)

			return 0;

		else if(x == shd.head_x && y == shd.head_y-3)

			return 0;

		if(x == shd.head_x && y == shd.head_y-4)

			return 0;

	}

	else if(Keyboard==DOWN)

	{

		if(x == shd.head_x && y == shd.head_y+1)

			return 0;

		else if(x == shd.head_x && y == shd.head_y+2)

			return 0;

		else if(x == shd.head_x && y == shd.head_y+3)

			return 0;

		else if(x == shd.head_x && y == shd.head_y+4)

			return 0;

	}

	else if(Keyboard==LEFT)

	{

		if(x == shd.head_x-1 && y == shd.head_y)

			return 0;

		else if(x == shd.head_x-2 && y == shd.head_y)

			return 0;

		else if(x == shd.head_x-3 && y == shd.head_y)

			return 0;

		else if(x == shd.head_x-4 && y == shd.head_y)

			return 0;

	}

	else if(Keyboard==RIGHT)

	{

		if(x == shd.head_x+1 && y == shd.head_y)

			return 0;

		else if(x == shd.head_x+2 && y == shd.head_y)

			return 0;

		else if(x == shd.head_x+3 && y == shd.head_y)

			return 0;

		else if(x == shd.head_x+4 && y == shd.head_y)

			return 0;

	}



	if(x==ssr.star_x+1 && y==ssr.star_y)

	{

		return 0;

	}

	else if(x==ssr.star_x-1 && y==ssr.star_y)

	{

		return 0;

	}

	else if(x==ssr.star_x && y==ssr.star_y+1)

	{

		return 0;

	}

	else if(x==ssr.star_x && y==ssr.star_y-1)

	{

		return 0;

	}



	return 1;



}

//¡Ü »ýŒº¹× °Ë»ö 

void Search_Apple(void)

{

	int x,y;

	int apple = 0;



	for(y=0;y<ypos;y++)

	{

		for(x=0;x<xpos;x++)

		{

			if(bgm.arr[y][x]==6) //¡Ü

			{

				appleviews = 0;

				sae.apple_x = x;

				sae.apple_y = y;

				apple++;

				break;

			}



		}

	}



	if(appleviews==apple_views && apple == 0) //ÈœŒö

	{

		appleviews=0;

		Generate_Apple();



	}

}



void Generate_Apple(void)

{

	int x,y;



	x=rand()%xpos;

	y=rand()%ypos;



	if(bgm.arr[y][x] == 0 && High_Search_Apple(x,y) == 0) //¡à

	{

		bgm.arr[y][x] = 6; //¡Ü

		gotoxy(x,y);      

		cout << "¡Ü";       //Ãâ·Â

	}

	else

	{

		Generate_Apple();

	}



	sae.apple_x = x;

	sae.apple_y = y;



}



int High_Search_Apple(int x,int y)

{

	

	if(bgm.arr[y][x+1]==5) //¢Ã

	{

		return 1;

	}

	else if(bgm.arr[y][x-1]==5) //¢Ã

	{

		return 1;

	}

	else if(bgm.arr[y-1][x]==5) //¢Ã

	{

		return 1;

	}

	else if(bgm.arr[y+1][x]==5) //¢Ã

	{

		return 1;

	}



	return 0;



}



//¡Ú »ýŒº °Ë»ö

void Search_Star(void)

{

	int x,y;

	int star = 0;



	for(y=0;y<ypos;y++)

	{

		for(x=0;x<xpos;x++)

		{

			if(bgm.arr[y][x] == 2) //¡Ú

			{

				star=1;

				gotoxy(x,y);

				cout << "¡Ú";            //Ãâ·Â

				ssr.star_x = x;

				ssr.star_y = y;

				break;

			}

		}

	}



	if(star==0)

	{

		Generate_Star();

	}



}





void Generate_Star(void)

{

	int x,y;



	x=rand()%xpos;

	y=rand()%ypos;



	if(bgm.arr[y][x] == 0 && High_Search_Star(x,y) == 0) //¡à

	{

		bgm.arr[y][x] = 2; //¡Ú

		gotoxy(x,y);      

		cout << "¡Ú";       //Ãâ·Â

	}

	else

	{

		Generate_Star();

	}



	ssr.star_x = x;

	ssr.star_y = y;



}





int High_Search_Star(int x,int y)

{

	if(bgm.arr[y][x+1]==5) //¢Ã

	{

		return 1;

	}

	else if(bgm.arr[y][x-1]==5) //¢Ã

	{

		return 1;

	}

	else if(bgm.arr[y-1][x]==5) //¢Ã

	{

		return 1;

	}

	else if(bgm.arr[y+1][x]==5) //¢Ã

	{

		return 1;

	}



	return 0;

}



//œº³×ÀÌÅ© º»ÃŒ ÀÌµ¿ ŸË°íž®Áò

void Snake_Move(void)

{



	stl[0].tail_y = shd.head_y;

	stl[0].tail_x = shd.head_x;



	if(Keyboard==UP)

	{

		gotoxy(shd.head_x,shd.head_y);

		bgm.arr[shd.head_y][shd.head_x] = 0; //¡à

		cout << "  ";

		gotoxy(shd.head_x,--shd.head_y);

		bgm.arr[shd.head_y][shd.head_x] = 1; //¡á

		cout << "¡á";

	}

	else if(Keyboard==DOWN)

	{

		gotoxy(shd.head_x,shd.head_y);

		bgm.arr[shd.head_y][shd.head_x] = 0; //¡à

		cout << "  ";

		gotoxy(shd.head_x,++shd.head_y);

		bgm.arr[shd.head_y][shd.head_x] = 1; //¡á

		cout << "¡á";

	}

	else if(Keyboard==RIGHT)

	{

		gotoxy(shd.head_x,shd.head_y);

		bgm.arr[shd.head_y][shd.head_x] = 0; //¡à

		cout << "  ";

		gotoxy(++shd.head_x,shd.head_y);

		bgm.arr[shd.head_y][shd.head_x] = 1; //¡á

		cout << "¡á";

	}

	else if(Keyboard==LEFT)

	{

		gotoxy(shd.head_x,shd.head_y);

		bgm.arr[shd.head_y][shd.head_x] = 0; //¡à

		cout << "  ";

		gotoxy(--shd.head_x,shd.head_y);

		bgm.arr[shd.head_y][shd.head_x] = 1; //¡á

		cout << "¡á";

	}



}

void Snake_Move2(void)

{



	stl[0].tail_y = shd.head_y;

	stl[0].tail_x = shd.head_x;



	if(Keyboard==UP)

	{

		gotoxy(shd.head_x,shd.head_y);

		bgm.arr[shd.head_y][shd.head_x] = 0; //¡à

		cout << "  ";

		gotoxy(shd.head_x,--shd.head_y);

		bgm.arr[shd.head_y][shd.head_x] = 1; //¡á

		cout << "¡á";

	}

	else if(Keyboard==DOWN)

	{

		gotoxy(shd.head_x,shd.head_y);

		bgm.arr[shd.head_y][shd.head_x] = 0; //¡à

		cout << "  ";

		gotoxy(shd.head_x,++shd.head_y);

		bgm.arr[shd.head_y][shd.head_x] = 1; //¡á

		cout << "¡á";

	}

	else if(Keyboard==RIGHT)

	{

		gotoxy(shd.head_x,shd.head_y);

		bgm.arr[shd.head_y][shd.head_x] = 0; //¡à

		cout << "  ";

		gotoxy(++shd.head_x,shd.head_y);

		bgm.arr[shd.head_y][shd.head_x] = 1; //¡á

		cout << "¡á";

	}

	else if(Keyboard==LEFT)

	{

		gotoxy(shd.head_x,shd.head_y);

		bgm.arr[shd.head_y][shd.head_x] = 0; //¡à

		cout << "  ";

		gotoxy(--shd.head_x,shd.head_y);

		bgm.arr[shd.head_y][shd.head_x] = 1; //¡á

		cout << "¡á";

	}

	stl[0].tail_y = shd2.head_y;

	stl[0].tail_x = shd2.head_x;



	/*if(Keyboard==UP1)

	{

		gotoxy(shd.head_x,shd.head_y);

		bgm.arr[shd.head_y][shd.head_x] = 0; //¡à

		cout << "  ";

		gotoxy(shd.head_x,--shd.head_y);

		bgm.arr[shd.head_y][shd.head_x] = 1; //¡á

		cout << "¡á";

	}

	else if(Keyboard==DOWN1)

	{

		gotoxy(shd.head_x,shd.head_y);

		bgm.arr[shd.head_y][shd.head_x] = 0; //¡à

		cout << "  ";

		gotoxy(shd.head_x,++shd.head_y);

		bgm.arr[shd.head_y][shd.head_x] = 1; //¡á

		cout << "¡á";

	}

	else if(Keyboard==RIGHT1)

	{

		gotoxy(shd.head_x,shd.head_y);

		bgm.arr[shd.head_y][shd.head_x] = 0; //¡à

		cout << "  ";

		gotoxy(++shd.head_x,shd.head_y);

		bgm.arr[shd.head_y][shd.head_x] = 1; //¡á

		cout << "¡á";

	}

	else if(Keyboard==LEFT1)

	{

		gotoxy(shd.head_x,shd.head_y);

		bgm.arr[shd.head_y][shd.head_x] = 0; //¡à

		cout << "  ";

		gotoxy(--shd.head_x,shd.head_y);

		bgm.arr[shd.head_y][shd.head_x] = 1; //¡á

		cout << "¡á";

	}*/

}



//°ÔÀÓ Á¡ŒöžŠ ÃÑ°ý G Ãâ·Â

void Game_Score(void)

{

	int x = 0,y = 0;

	int apple = 0;



	if(bgm.arr[ssr.star_y][ssr.star_x] == 1)  //¡á

	{

		bgm.score+=100;

		bgm.bonus = end_time-start_time;

		bgm.total = bgm.score + bgm.bonus;

		if(h_score<bgm.score) h_score=bgm.score+bgm.bonus; 

		Tail+=1;

		boxviews++;

		appleviews++;

	}

	else if(sae.apple_y!=999)

	{

		if(bgm.arr[sae.apple_y][sae.apple_x] == 1) //¡á

		{

			sae.apple_y=999;

			sae.apple_x=999;

			for(y=0;y<ypos;y++)

			{

				for(x=0;x<xpos;x++)

				{

					if(bgm.arr[y][x]==5) //¢Ã

					{

						apple++;

						if(box!=0)

						{

							//box--;

							bgm.arr[y][x]=0; //¡à

							gotoxy(x,y);

							cout << "  ";

						}

						else

						{

							cout << "¿À·ù";

							Sleep(5000);

						}

						break;

					}



				}

				if(apple!=0)

				{

					box--;

					break;

				}

			}

		}

		

	}







	gotoxy(21,0);

	cout << "¡ÚSnake Game¡Ú";

	gotoxy(21,2);

	cout << "ÇöÀç ³­ÀÌµµ : " << leveltext;

	gotoxy(21,4);

	cout << "ÀåŸÖ¹° : " << box << "°³";

	gotoxy(21,6);

	cout << "ÇöÀç À§Ä¡ : " << shd.head_y << " " << shd.head_x;

	gotoxy(21,7);

	cout << "º°ÀÇ À§Ä¡ : " << ssr.star_y << " " << ssr.star_x;

	gotoxy(21,10);

	cout << "²¿ž®ÀÇ °³Œö : " << Tail;

	gotoxy(21,11);

	cout << "žñÇ¥²¿ž® °³Œö : " << tail_winner;

	gotoxy(21,14);

	cout << "Score :  " << bgm.score;

	gotoxy(21,15);

	cout << "Bonus :  " << bgm.bonus;

	gotoxy(21,16);

	cout << "High_Score : " << h_score;

}



//²¿ž® ºÎºÐ ŸË°íž®Áò

void Snake_TMove(void)

{

	static int i = 0;

	int com = 0;

	int turbo = 0;



	if(Tail!=0)

	{

		for(com=0;com<Tail;com++)

		{

			if(stl[turbo+1].tail_y!=999)

			{

				if(i==0)

				{

					stl[2].tail_x = stl[1].tail_x;

					stl[2].tail_y = stl[1].tail_y;

					i++;

				}

				else

				{

					stl[turbo+2].tail_x = stl[turbo+1].tail_x;

					stl[turbo+2].tail_y = stl[turbo+1].tail_y;

				}

				bgm.arr[stl[turbo+1].tail_y][stl[turbo+1].tail_x]=0; //¡à

				gotoxy(stl[turbo+1].tail_x,stl[turbo+1].tail_y);

				cout << "  ";

			}

	

			stl[turbo+1].tail_y = stl[turbo].tail_y;

			stl[turbo+1].tail_x = stl[turbo].tail_x;



			bgm.arr[stl[turbo].tail_y][stl[turbo].tail_x]=4; //¢Ë

			gotoxy(stl[turbo].tail_x,stl[turbo].tail_y);

			cout << "¢Ë";



			turbo+=2;

			

		}

	}



	i = 0;

}



//ÆÐ¹è ¿©ºÎ °Ë»ç

void Game_Over(void)

{

	int i,j;

	int turbo = 0;



	if(shd.head_x<0)	

	{

		bgm.exit=1;

	}

	else if(shd.head_x>xpos-1)

	{

		bgm.exit=1;

	}

	else if(shd.head_y<0)

	{

		bgm.exit=1;

	}

	else if(shd.head_y>ypos-1)

	{

		bgm.exit=1;

	}





	for(i=0;i<ypos;i++)

	{

		for(j=0;j<xpos;j++)

		{

			if(bgm.arr[i][j] == 4)

			{

				turbo++;

			}

		}

	}



	if(turbo!=Tail)

	{	

		bgm.exit=1;

	}



	turbo = 0;

	for(i=0;i<ypos;i++)

	{

		for(j=0;j<xpos;j++)

		{

			if(bgm.arr[i][j] == 5)  //¢Ã

			{

				turbo++;

			}

		}

	}



	if(turbo!=box)

	{

		bgm.exit=1;

	}

	

	time(&end_time);

}

//œÂž® žÞŒŒÁö

void Game_Winner(void)

{

	if(Tail==tail_winner)

	{

		bgm.winner = 1;



		system("cls");

		cout << "--------------------------------------------------------------------" << endl;

		cout << "-                                                                  -" << endl;

		cout << "-                                                                  -" << endl;

		cout << "-                                                                  -" << endl;

		cout << "-                                                                  -" << endl;

		cout << "-                         == You Win ==                            -" << endl;

		cout << "-                                                                  -" << endl;

		cout << "-                         -great work!-                            -" << endl;

		cout << "-                                                                  -" << endl;

		cout << "-                                                                  -" << endl;

		cout << "-                                                                  -" << endl;

		cout << "--------------------------------------------------------------------" << endl;

		cout << endl;

		cout<<"Press any key"<<endl;

		_getch();

	}

}



//žÞÀÎ

int main(void)

{

	int i,j;

	string temp;

	srand((int)time(NULL));

	ifstream is;

	is.open("ranking.txt");

	if( !is ) { // ! ¿¬»êÀÚ ¿À¹ö·Îµù

		cerr << "Fail open file" << endl;

		exit( 1 );

	}

	

	for(int k=0;k<ranking;k++)

		is>>temp>>srk[k].name>>srk[k].score>>srk[k].level;



	is.close();

	h_score=srk[0].score;

loop_1:

	Snake_Cotinue();

	Snake_screen2();

loop:

	Snake_Level();

	Snake_Default();  //µðÆúÆ® °ªÀž·Î ÃÊ±âÈ­

	

	while(1)

	{



		for(i = 0; i < 1000; i++)

		{

            if(_kbhit())	Game_key();

		}

		//coma=Keyboard;

		Snake_Control();

		Sleep(delay);

		fflush(stdin);

		if(bgm.exit!=0)

		{

			system("cls");

			cout << "----------------------------------------------------------------" << endl;

			cout << "-                                                              -" << endl;	

			cout << "-                                                              -" << endl;	

			cout << "-                                                              -" << endl;	

			cout << "-                                                              -" << endl;	

			cout << "-                     == You Defeat ==                         -" << endl;	

			cout << "-                                                              -" << endl;	

			cout << "-                      - Give up... -                          -" << endl;	

			cout << "-                                                              -" << endl;	

			cout << "-                                                              -" << endl;	

			cout << "-                                                              -" << endl;	

			cout << "----------------------------------------------------------------" << endl;

			cout << endl;

			cout<<"Press any key"<<endl;

			_getch();

			system("cls");

			cout<<"Name"<<endl;

			cin>>username;

			system("cls");

			Ranking_Score();

			Ranking_print();

			cout<<endl;

			cout<<"                         1. Re Start                        "<<endl;

			cout<<endl;

			cout<<"                         2. Menu                            "<<endl;

			cout<<endl;

			cout<<"                         3. Exit                            "<<endl;

			cout<<endl;

			cout<<"                       Select number : ";

			cin >> j;

			if(j==1) {

				system("cls");

				goto loop;

			} else if(j==2) {

				system("cls");

				goto loop_1;

			} else if(j==3) {

				exit(1);

			}

		}

		if(bgm.winner!=0)

		{

			system("cls");

			cout<<"name"<<endl;

			cin>>username;

			Ranking_Score();

			Ranking_print();

		    cout<<endl;

			cout<<"                         1. Re Start                          "<<endl;

			cout<<endl;

			cout<<"                         2. Menu                              "<<endl;

			cout<<endl;

			cout<<"                         3. Exit                              "<<endl;

			cout<<endl;

			cout<<"                       Select number : ";

			cin >> j;

			if(j==1) {

				system("cls");

				goto loop;

			} else if(j==2) {

				system("cls");

				goto loop_1;

			} else if(j==3) {

				exit(1);

			}

			break;

		}

	}



	return 0;

}
A
A
}
