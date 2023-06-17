#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include "map.h"
#include "map2.h"
#include "title.h"
#include "resource.h"
#include<mmsystem.h>
#pragma comment(lib, "winmm.lib")


#define MAP_ORIGIN_X 8
#define MAP_ORIGIN_Y 5
#define GBOARD_ORIGIN_X 8
#define GBOARD_ORIGIN_Y 2
#define GBOARD_HEIGHT1 21
#define GBOARD_WIDTH1 19
#define GBOARD_HEIGHT2 21
#define GBOARD_WIDTH2 21

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ENTER 13
#define B 98
#define SPACE 32

void RemoveCursor(void);
int GetCurrentY(void);
void DrawMap(char map[25][30], int x, int y);
void DeleteMap();
void DrawTitle(char title[5][29]);
int MainScreenKeyInput();
void MainScreen();
int SelectMap();
void Information();
void DeleteScreen(int _y);
int SelectMapKeyInput();
void SetCurrentCursorPos(int x, int y);
void GameStart(int mapnum);

//맵 초기화
char map[25][30] =
{
   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
   { 1,'B',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'B',1 },
   { 1,0,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,0,1 },
   { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
   { 1,0,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,0,1 },
   { 1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1 },
   { 1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1 },
   { 0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0 },
   { 1,1,1,1,0,1,0,1,1,0,1,1,0,1,0,1,1,1,1 },
   { 1,'A',0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,'A',1 },
   { 1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1 },
   { 0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0 },
   { 1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1 },
   { 1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1 },
   { 1,0,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,0,1 },
   { 1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1 },
   { 1,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,1 },
   { 1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1 },
   { 1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,0,1 },
   { 1,'C',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'C',1 },
   { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
};

char easymap[25][30] =
{
   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
   {1,'B',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'B',1},
   {1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1},
   {1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1},
   {1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1},
   {1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1},
   {1,'A',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'A',1},
   {1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1},
   {1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1},
   {1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1},
   {1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1,1,0,1},
   {1,'C',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'C',1},
   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

char title[5][29] =
{
   {1,1,1,1,1,0,1,0,0,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1},
   {1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0},
   {1,1,1,1,1,0,1,1,1,1,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0},
   {0,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0},
   {1,1,1,1,1,0,1,0,0,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,0,0,1,0,0}
};



int main()
{
	int select = 0;
	int key = 0;
	int mapnum = 0;
	//PlaySound(MAKEINTRESOURCE(GAME), NULL, SND_RESOURCE | SND_ASYNC | SND_LOOP);

	system("mode con cols=100 lines=30 | title BBANG");
	RemoveCursor();
	while (1)
	{
		MainScreen();
		SetCurrentCursorPos(36, 10);
		printf("▶");

		while (mapnum == 0)
		{
			select = MainScreenKeyInput();
			switch (select)
			{

			case 10:
				mapnum = SelectMap();
				break;
			case 11:
				Information();
				break;
			}
		}
		DeleteScreen(3);
		SetCurrentCursorPos(20, 5);
		if (mapnum > 0)
		{
			break;
		}
		else
			mapnum = 0;
	}

	GameStart(mapnum);

}

void setColor(unsigned short text, unsigned short back)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text | (back << 4));
}
void SetCurrentCursorPos(int x, int y)
{
	COORD p = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}

void DrawMap(char map[25][30], int _x, int _y)
{
	int x = 0, y = 0;
	int curX = 0, curY = 0;
	for (y = 0; y < _y; y++)
	{
		for (x = 0; x < _x; x++)
		{
			curX = x * 2 + MAP_ORIGIN_X;
			curY = y + MAP_ORIGIN_Y;
			SetCurrentCursorPos(curX, curY);


			setColor(8, 0);
			if (map[y][x] == 1)
				printf("◈");
			if (map[y][x] == 'A' || map[y][x] == 'B' || map[y][x] == 'C')
			{
				printf("%c", map[y][x]);
			}
			setColor(15, 0);



			if (map[y][x] == 50)
			{
				//setColor(6, 0);
				printf("ⓢ");
				//setColor(15, 0);

			}
			if (map[y][x] == 51)
			{
				//setColor(4, 15);
				printf("ⓡ");
				//setColor(15, 0);
			}
			if (map[y][x] == 52)
			{
				//setColor(6, 15);
				printf("ⓜ");
				//setColor(15, 0);
			}
			if (map[y][x] == 53)
			{
				//setColor(6, 15);
				printf("ⓗ");
				//setColor(15, 0);
			}
		}
	}
}

void DrawTitle(char title[5][29])
{
	int x = 0, y = 0;
	int curX = 0, curY = 0;
	for (y = 0; y < 5; y++)
	{
		for (x = 0; x < 29; x++)
		{
			curX = x * 2 + GBOARD_ORIGIN_X + 10;
			curY = y + GBOARD_ORIGIN_Y;
			SetCurrentCursorPos(curX, curY);
			if (title[y][x] == 1)
			{
				printf("■");
			}
			else { printf("  "); }
		}
	}
}

int MainScreenKeyInput()
{
	int i = 0, key = 0;

	while (1)
	{
		if (_kbhit() != 0)
		{
			key = _getch();

			switch (key)
			{
			case UP:
				SetCurrentCursorPos(36, 11);
				printf(" ");
				SetCurrentCursorPos(36, 10);
				printf("▶");
				break;
			case DOWN:
				SetCurrentCursorPos(36, 10);
				printf(" ");
				SetCurrentCursorPos(36, 11);
				printf("▶");
				break;
			case ENTER:
				return GetCurrentY();
			case B:
				MainScreen();
				SetCurrentCursorPos(36, 10);
				printf("▶");
				break;
			}
		}
	}
}

int SelectMapKeyInput()
{
	int i = 1, key = 0;

	while (1)
	{

		SetCurrentCursorPos(40, 2);
		printf("%d", i);
		if (_kbhit() != 0)
		{
			key = _getch();
			switch (key)
			{
			case LEFT:
				SetCurrentCursorPos(40, 2);
				printf("  ");
				DeleteMap();
				if (i > 1)
					i--;
				break;
			case RIGHT:
				SetCurrentCursorPos(40, 2);
				printf("  ");
				DeleteMap();
				if (i < 2)
					i++;
				break;
			case ENTER:
				return i;
			case B:
				return -1;

			}
		}

		switch (i)
		{
		case 1:
			DrawMap(map, 19, 21);
			break;
		case 2:
			DrawMap(easymap, 21, 21);
			break;
		}

	}
}

int GetCurrentY(void)

{

	int curpointy;

	CONSOLE_SCREEN_BUFFER_INFO curInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curpointy = curInfo.dwCursorPosition.Y;


	return curpointy;

}

void MainScreen()
{
	DeleteScreen(25);
	SetCurrentCursorPos(GBOARD_ORIGIN_Y, GBOARD_ORIGIN_X);
	DrawTitle(title);
	SetCurrentCursorPos(40, 10);
	printf("게임 시작");
	SetCurrentCursorPos(40, 11);
	printf("조작키");
}

void Information()
{

	DeleteScreen(11);
	SetCurrentCursorPos(26, 2);
	printf("조작키");
	SetCurrentCursorPos(14, 4);
	printf("↑ - 위 이동        Q - 공격!");
	SetCurrentCursorPos(14, 6);
	printf("↓ - 아래 이동      W - 무기변경!");
	SetCurrentCursorPos(14, 8);
	printf("→ - 오른쪽 이동");
	SetCurrentCursorPos(14, 10);
	printf("← - 왼쪽 이동");
	SetCurrentCursorPos(18, 12);
	printf("<B> 키를 눌러 이전화면으로 이동...");

}
void DeleteScreen(int _y)
{

	int x = 0, y = 0;
	int curX = 0, curY = 0;
	for (y = 0; y < _y; y++)
	{
		for (x = 0; x < 100; x++)
		{
			curX = x * 2 + GBOARD_ORIGIN_X;
			curY = y + GBOARD_ORIGIN_Y;
			SetCurrentCursorPos(curX, curY);
			printf("  ");
		}
	}
}

int SelectMap()
{
	int mapnum = 0;
	DeleteScreen(11);
	SetCurrentCursorPos(26, 2);
	printf("맵 선택");

	SetCurrentCursorPos(14, 3);
	printf("좌우 방향키로 이동 엔터로 선택");
	SetCurrentCursorPos(8, 5);
	SetCurrentCursorPos(40, 2);
	mapnum = SelectMapKeyInput();
	return mapnum;
	//DrawMap(easymap);
}

void RemoveCursor(void)
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}


void DeleteMap()
{
	int x = 0, y = 0;
	for (y = 0; y < 25; y++)
	{
		for (x = 0; x < 25; x++)
		{
			SetCurrentCursorPos(MAP_ORIGIN_X + x * 2, MAP_ORIGIN_Y + y);
			printf("  ");
		}
	}
}


void GameStart(int mapnum)
{
	PlaySound(NULL, 0, 0);
	pc_main(mapnum);

}

void GameOver(int score)
{
	int time = 0;
	DeleteScreen(35);
	SetCurrentCursorPos(30, 8);
	printf(" ■■■     ■■     ■      ■   ■■■");
	Sleep(80);
	SetCurrentCursorPos(30, 9);
	printf("■         ■  ■    ■■  ■■  ■");
	Sleep(80);
	SetCurrentCursorPos(30, 10);
	printf("■  ■■  ■■■■   ■ ■■ ■  ■■■");
	Sleep(80);
	SetCurrentCursorPos(30, 11);
	printf("■    ■  ■    ■   ■  ■  ■  ■");
	Sleep(80);
	SetCurrentCursorPos(30, 12);
	printf(" ■■■  ■■  ■■ ■■    ■■  ■■■");
	Sleep(80);
	SetCurrentCursorPos(30, 14);
	printf("  ■■    ■■  ■■   ■■■  ■■■■");
	Sleep(80);
	SetCurrentCursorPos(30, 15);
	printf("■    ■   ■    ■   ■        ■   ■");
	Sleep(80);
	SetCurrentCursorPos(30, 16);
	printf("■    ■    ■  ■    ■■■    ■■■");
	Sleep(80);
	SetCurrentCursorPos(30, 17);
	printf("■    ■     ■■     ■        ■   ■");
	Sleep(80);
	SetCurrentCursorPos(30, 18);
	printf("  ■■        ■       ■■■  ■■ ■■");
	Sleep(80);

	SetCurrentCursorPos(44, 21);
	printf("SCORE : %d", score);

	while (1)
	{
		if (time > 80)
			time = 0;
		time++;
		Sleep(10);
		int key = 0;
		if (time == 40) {
			SetCurrentCursorPos(30, 23);
			printf("***  PRESS SPACE BAR TO PLAY AGAIN  ***");
		}
		if (time == 80) {
			SetCurrentCursorPos(30, 23);
			printf("                                                     ");
		}


		if (_kbhit() != 0)
		{
			key = _getch();
			if (key == SPACE)
				break;

		}
	}

	main();
}