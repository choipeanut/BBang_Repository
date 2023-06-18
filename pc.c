#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include "map.h"
#include "map2.h"
// 구조체 추가
// 함수 추가
#define GBOARD_ORIGIN_X 8
#define GBOARD_ORIGIN_Y 5
#define GBOARD_HEIGHT 21
#define GBOARD_WIDTH 19

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define Q 32
#define W 119

#define GUN1 49
#define GUN2 50
#define GUN3 51
#define GUN4 52
#define GUN5 53

#define SHOT_MAX 40 // 총알 최대수
#define NPC_MAX 40
#define BOSS_MAX 100 

COORD CROSSROAD[] = {
   {4,1},{8,1},{10,1},{14,1},//4
   {1,3},{4,3},{6,3},{8,3},{10,3},{12,3},{14,3},{17,3}, //8
   {4,5},{14,5},//2
   {8,7},{10,7},//2
   {4,9},{6,9},{12,9},{14,9},//4
   {6,11},{12,11},//2
   {4,13},{6,13},{12,13},{14,13},//4
   {4,15},{6,15},{8,15},{10,15},{12,15},{14,15},//6
   {2,17},{16,17},//2
   {8,19},{10,19},//2
};
COORD CROSSROAD2[] = {
   {1,1}, {1,4}, {1,7}, {1,10}, {1,13}, {1,16}, {1,19},
   {4,1}, {4,4}, {4,7}, {4,10},{4,13},{4,16},{4,19},
   {7,1}, {7,4}, {7,7}, {7,10},{7,13},{7,16},{7,19},
   {10,1}, {10,4}, {10,7}, {10,10},{10,13},{10,16},{10,19},
	{13,1}, {13,4}, {13,7}, {13,10},{13,13},{13,16},{13,19},
	{16,1}, {16,4}, {16,7}, {16,10},{16,13},{16,16},{16,19},
	 {19,1}, {19,4}, {19,7}, {19,10},{19,13},{19,16},{19,19},

};

typedef struct bullet {
	int x, y;
	int Type;
	int UseFlag;
	int BulletDirect;

}BULLETNODE;
typedef struct npc {
	int x, y;
	int Type;
	int UseFlag;
	int NpcDirect;
	int NpcHealth;

}NPCNODE;
typedef struct Item
{
	int x, y;
	int Type;
	int UseFlag;
}ITEM;


int PC_GunMode = 1; // 현재 들고있는 총
int speed = 0;
int PC_Direction;

int score = 0;
int hp = 3;
int ammo[4] = { 0 };
int count = 0; //
int mooj = 0;
int moojend = 0;

void SetCurrentCursorPos(int x, int y);
void PC_ShiftLeft();
void PC_ShiftRight();
void PC_ShiftUp();
void PC_ShiftDown();
void RemoveCursor();

void DrawMap(char curMap[25][30], int x, int y);
void Draw();
void DrawPlayer(char blockInfo[3][3]);
void PC_DeleteBlock(char blockInfo[3][3]);
int DetectPC_Collision(int posX, int posY);
int DetectBullet_Collision(int posX, int posY, int type);

void CreateBullet(COORD Bullet);
void PC_Shoot(char blockInfo[3][3]);
void DrawShot();

void ShotAction(int count);
void Collision_mapset(int i, int elem); // BULLET[i]의 map위치 에서 elem으로 변경
void Gun1Print(int i);
void Gun2Print(int i);
void Gun3Print(int i);
void Gun4Print(int i);


void ShootUpdate(int i);


int GUN1TRUE = 1;
int GUN2TRUE = 0;
int GUN3TRUE = 0;
int GUN4TRUE = 0;
int GUN5TRUE = 0;
///////////////////////

int Gun1Time = 10;
int Gun2Time = 10;
int Gun3Time = 10;

void mine(int i);
void DrawUI();
void DeleteGunPicture(int tmp);
void DrawGun();

int DetectNPC_Collision(int posX, int posY, int dir);
void DrawNPC();
void CreateNpc(int x, int y);
void NPC_Collision_mapset(int i, int elem);
void NpcUpdate(int i);
void NpcAction(int count); BULLETNODE BULLET[SHOT_MAX];
void NPC_ItemDrop(int i, int x, int y);
void PC_GetItem();


//-----------------------

int StageClear = 3001;
int Boss_Health = 1000;

int InBoss = 3000; // 보스맵에 들어가면 1, 아니면 0;


/////////BOSS

typedef struct boss {
	int x, y;
	int UseFlag;
	int BossDirect;
	int type;

}BOSSNODE;

//int Boss_Health = 30;
int Boss_Direction = 0;
BOSSNODE BOSS[BOSS_MAX];
COORD Boss_BulletPos;
COORD Boss_mapPosition;
COORD Boss_Pos;

void Boss_Shoot();
void Boss_CreateBullet(COORD Boss_Bullet, int direction);
void Boss_ShotAction();
int DetectBossBullet_Collision(int posX, int posY);
void Boss_DrawShot();
void DoProgress(char label[], int step, int total);
void DrawBoss(char blockInfo[5][8]);
void Boss_ShiftRight();
void Boss_ShiftLeft();
void Boss_DeleteBlock(char blockInfo[5][8]);



NPCNODE NPC[SHOT_MAX];
char bossmap[25][30] =
{
   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};  //boss

char Boss_Model[5][8]
= {

   {0,0,7,7,7,7,0,0},
   {0,7,0,10,11,0,7,0},
   {7,0,8,0,0,8,0,7},
   {0,7,0,9,9,0,7,0},
   {0,0,7,7,7,7,0,0},

};

char PC_Model[][3][3] =
{
   {
   {0,6,0},
   {0,2,0},
   {0,0,0} },

   {
   {0,0,0},
   {0,2,6},
   {0,0,0} },
   {
   {0,0,0},
   {0,2,0},
   {0,6,0} },
   {
   {0,0,0},
   {6,2,0},
   {0,0,0} },

};
char curMap[25][30];
int map_num;

COORD Pos;

COORD PC_mapPosition; // map에서의 pc좌표

BULLETNODE BULLET[NPC_MAX];
int pc_main(int mapnum) {

	Boss_Pos.X = 28;
	Boss_Pos.Y = 8;


	if (mapnum == 1)
	{
		for (int i = 0; i < 21; i++)
		{
			for (int j = 0; j < 21; j++)
			{
				curMap[i][j] = map[i][j];
			}
		}
		map_num = mapnum;
		// pc 커서좌표
		Pos.X = 24;
		Pos.Y = 13;
		SetCurrentCursorPos(Pos.X, Pos.Y);

		//pc방향초기화
		PC_Direction = 0;
		//pc 출력
		DrawPlayer(PC_Model[PC_Direction]);
	}
	else
	{
		for (int i = 0; i < 21; i++)
		{
			for (int j = 0; j < 21; j++)
			{
				curMap[i][j] = easymap[i][j];
			}
		}

		map_num = mapnum;
		// pc 커서좌표
		Pos.X = 26;
		Pos.Y = 14;
		SetCurrentCursorPos(Pos.X, Pos.Y);

		//pc방향초기화
		PC_Direction = 0;
		//pc 출력
		DrawPlayer(PC_Model[PC_Direction]);

	}

	SetCurrentCursorPos(50, 5);
	printf("*******************");


	SetCurrentCursorPos(50, 16);
	printf("*******************");
	SetCurrentCursorPos(50, 17);
	setColor(10, 0);
	printf("SCORE : ");
	setColor(15, 0);




	for (int i = 0; i < SHOT_MAX; i++) //
		BULLET[i].UseFlag = 0; //

	srand(time(NULL));

	RemoveCursor();

	DrawGun();

	while (1) {

		if (hp <= 0)
		{

			int tmpscore = score;
			score = 0;
			for (int i = 0; i < BOSS_MAX; i++)
			{
				if (i < 40)
				{
					BULLET[i].UseFlag = 0;
					NPC[i].UseFlag = 0;
				}
				BOSS[i].UseFlag = 0;
			}

			for (int i = 0; i < 4; i++)
				ammo[i] = 0;

			PC_GunMode = 1;
			GUN1TRUE = 1;
			GUN2TRUE = 0;
			GUN3TRUE = 0;
			GUN4TRUE = 0;
			GUN5TRUE = 0;

			Gun1Time = 10;
			Gun2Time = 10;
			Gun3Time = 10;
			InBoss = 7000;

			hp = 3;
			Boss_Health = 1000;
			SetCurrentCursorPos(0, 3);
			printf("                                                                        ");
			GameOver(tmpscore);
		}
		if (score == InBoss) {

			InBoss--;

			DeleteScreen(30);
			score++;

			//hp = 3;
			for (int i = 0; i < 25; i++)
			{
				for (int j = 0; j < 30; j++)
				{
					curMap[i][j] = bossmap[i][j];
				}
			}

			for (int i = 0; i < SHOT_MAX; i++) {

				BULLET[i].UseFlag = 0;
				NPC[i].UseFlag = 0;
			}

			system("mode con:cols=110 lines=35");
			for (int i = 0; i < BOSS_MAX; i++) {
				BOSS[i].UseFlag = 0; // BOSS
			}



			DoProgress("Boss HP ", Boss_Health, 1000);
			PC_GunMode = 1;
			DrawGun();
			DrawMap(bossmap, 30, 25);

		}

		int key;
		if (_kbhit() != 0) {

			key = _getch();
			switch (key)
			{
			case LEFT:
				PC_ShiftLeft();
				break;
			case RIGHT:
				PC_ShiftRight();
				break;
			case UP:
				PC_ShiftUp();
				break;
			case DOWN:
				PC_ShiftDown();
				break;
			case Q:
				PC_Shoot(PC_Model[PC_Direction]);
				//Boss_Health -= 3;
				break;
			case GUN1:
				if (GUN1TRUE == 1)
				{
					PC_GunMode = 1;//
					DrawGun();
					break;
				}
				break;
			case GUN2:
				if (GUN2TRUE == 1)
				{
					PC_GunMode = 2;//
					DrawGun();
					break;
				}
				break;
			case GUN3:
				if (GUN3TRUE == 1)
				{
					PC_GunMode = 3;//
					DrawGun();
					break;
				}
				break;
			case GUN4:
				if (GUN4TRUE == 1)
				{
					PC_GunMode = 4;//
					DrawGun();
					break;
				}
				break;
			case GUN5:
				if (GUN5TRUE == 1)
				{
					PC_GunMode = 5;//
					DrawGun();

					break;
				}
				break;

			}

		}
		if (count == moojend)
		{
			mooj = 0;
			moojend = 0;
		}
		Sleep(10);  ///
		ShotAction(count);
		if (score != StageClear) {

			PC_GetItem();
			NpcAction(count);

			if (speed < 150)
				speed = score / 50;

			if (count % (200 - speed) == 0)
			{
				if (mapnum == 1)
				{
					if (score >= 1000)
					{
						CreateNpc(1, 1);
						CreateNpc(17, 1);
					}
					if (score >= 2000)
					{
						CreateNpc(1, 19);
						CreateNpc(17, 19);
					}
					CreateNpc(1, 9);
					CreateNpc(17, 9);
				}
				else if (mapnum == 2)
				{
					if (score >= 1000)
					{
						CreateNpc(1, 1);
						CreateNpc(19, 1);
					}
					if (score >= 2000)
					{
						CreateNpc(1, 19);
						CreateNpc(19, 19);
					}
					CreateNpc(1, 10);
					CreateNpc(19, 10);
				}
			}
		}


		else {

			if (Boss_Health <= 0) {
				score--;
				Boss_Health = 1000;
				DeleteScreen(35);

				SetCurrentCursorPos(0, 2);
				printf("                                             ");
				SetCurrentCursorPos(0, 3);
				printf("                                             ");

				DrawGun();
				//DrawMap(bossmap, 30, 25);
				if (mapnum == 1)
				{
					for (int i = 0; i < 25; i++)
					{
						for (int j = 0; j < 30; j++)
						{
							curMap[i][j] = map[i][j];
						}
					}
					map_num = mapnum;
					// pc 커서좌표
					Pos.X = 24;
					Pos.Y = 13;
					SetCurrentCursorPos(Pos.X, Pos.Y);

					//pc방향초기화
					PC_Direction = 0;
					//pc 출력
					DrawPlayer(PC_Model[PC_Direction]);
				}
				else
				{
					for (int i = 0; i < 25; i++)
					{
						for (int j = 0; j < 30; j++)
						{
							curMap[i][j] = easymap[i][j];
						}
					}

					map_num = mapnum;
					// pc 커서좌표
					Pos.X = 26;
					Pos.Y = 14;
					SetCurrentCursorPos(Pos.X, Pos.Y);

					//pc방향초기화
					PC_Direction = 0;
					//pc 출력
					DrawPlayer(PC_Model[PC_Direction]);

				}

				SetCurrentCursorPos(50, 5);
				printf("*******************");


				SetCurrentCursorPos(50, 16);
				printf("*******************");
				SetCurrentCursorPos(50, 17);
				setColor(10, 0);
				printf("SCORE : ");
				setColor(15, 0);

			}
			Sleep(10);
			if (count % 40 == 0)
			{
				Boss_Shoot();
			}
			if (count % 20 == 0) {
				if (Boss_Direction == 0)
					Boss_ShiftRight();
				else
					Boss_ShiftLeft();
			}
			if (count % 3 == 0)
				Boss_ShotAction();
		}


		Draw();


		if (count > 201)
			count = 0; //
		count++;//
		Gun1Time++;//
		Gun2Time++;//
		Gun3Time++;//

		int tmp;
		if (score != StageClear)
			tmp = 50;
		else
			tmp = 75;

		SetCurrentCursorPos(tmp, 7);
		printf("                       ");
		switch (PC_GunMode)
		{
		case 1:
			SetCurrentCursorPos(tmp, 7);
			printf("PISTOL / ∞");
			break;
		case 2:
			SetCurrentCursorPos(tmp, 7);
			printf("SNIPER / %d", ammo[0]);
			break;
		case 3:
			SetCurrentCursorPos(tmp, 7);
			printf("ASSAULT RIFLE / %d", ammo[1]);
			break;
		case 4:
			SetCurrentCursorPos(tmp, 7);
			printf("MINES / %d", ammo[2]);
			break;
		case 5:
			SetCurrentCursorPos(tmp, 7);
			printf("HEALPACK / %d", ammo[3]);
			break;
		}
	}
}
void PC_ShiftRight() {


	if (DetectPC_Collision(Pos.X + 2, Pos.Y) == 0)
		return;


	PC_DeleteBlock(PC_Model[PC_Direction]);

	PC_mapPosition.X = (Pos.X - GBOARD_ORIGIN_X) / 2;
	PC_mapPosition.Y = Pos.Y - GBOARD_ORIGIN_Y;
	curMap[PC_mapPosition.Y + 1][PC_mapPosition.X + 1] = 0;

	Pos.X += 2;
	PC_mapPosition.X = (Pos.X - GBOARD_ORIGIN_X) / 2;
	curMap[PC_mapPosition.Y + 1][PC_mapPosition.X + 1] = 2;

	PC_Direction = 1;

}
void PC_ShiftLeft() {//나머지 방향도 작동방식 동일
	if (DetectPC_Collision(Pos.X - 2, Pos.Y) == 0)
		return;


	PC_DeleteBlock(PC_Model[PC_Direction]);
	//이전위치 삭제
	PC_mapPosition.X = (Pos.X - GBOARD_ORIGIN_X) / 2;
	PC_mapPosition.Y = Pos.Y - GBOARD_ORIGIN_Y;
	curMap[PC_mapPosition.Y + 1][PC_mapPosition.X + 1] = 0;
	//위치 업데이트
	Pos.X += -2;
	PC_mapPosition.X = (Pos.X - GBOARD_ORIGIN_X) / 2;
	curMap[PC_mapPosition.Y + 1][PC_mapPosition.X + 1] = 2;
	//pc방향 업데이트
	PC_Direction = 3;


}
void PC_ShiftUp() {
	if (DetectPC_Collision(Pos.X, Pos.Y - 1) == 0)
		return;


	PC_DeleteBlock(PC_Model[PC_Direction]);

	PC_mapPosition.X = (Pos.X - GBOARD_ORIGIN_X) / 2;
	PC_mapPosition.Y = Pos.Y - GBOARD_ORIGIN_Y;
	curMap[PC_mapPosition.Y + 1][PC_mapPosition.X + 1] = 0;

	Pos.Y += -1;
	PC_mapPosition.Y = Pos.Y - GBOARD_ORIGIN_Y;
	curMap[PC_mapPosition.Y + 1][PC_mapPosition.X + 1] = 2;

	PC_Direction = 0;

}
void PC_ShiftDown() {
	if (DetectPC_Collision(Pos.X, Pos.Y + 1) == 0)
		return;


	PC_DeleteBlock(PC_Model[PC_Direction]);

	PC_mapPosition.X = (Pos.X - GBOARD_ORIGIN_X) / 2;
	PC_mapPosition.Y = Pos.Y - GBOARD_ORIGIN_Y;
	curMap[PC_mapPosition.Y + 1][PC_mapPosition.X + 1] = 0;

	Pos.Y += 1;
	PC_mapPosition.Y = Pos.Y - GBOARD_ORIGIN_Y;
	curMap[PC_mapPosition.Y + 1][PC_mapPosition.X + 1] = 2;

	PC_Direction = 2;

}
void PC_DeleteBlock(char blockInfo[3][3]) {

	int x, y;
	COORD curPos = Pos;

	for (y = 0; y < 3; y++) {
		for (x = 0; x < 3; x++) {

			SetCurrentCursorPos(curPos.X + x * 2, curPos.Y + y);

			if (blockInfo[y][x] == 2 || blockInfo[y][x] == 6) {
				printf("  ");
			}
		}
	}
	SetCurrentCursorPos(curPos.X, curPos.Y);


}
void DrawPlayer(char blockInfo[3][3]) {

	int x, y;

	COORD curPos = Pos;


	for (y = 0; y < 3; y++) {

		for (x = 0; x < 3; x++) {
			SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y);
			PC_mapPosition.X = (Pos.X - GBOARD_ORIGIN_X) / 2;
			PC_mapPosition.Y = Pos.Y - GBOARD_ORIGIN_Y;

			if (blockInfo[y][x] == 2) {
				curMap[PC_mapPosition.Y + y][PC_mapPosition.X + x] = 2;

				printf("■");
			}
			if (blockInfo[y][x] == 6 && curMap[PC_mapPosition.Y + y][PC_mapPosition.X + x] != 1) {

				//
				if (PC_GunMode == 1) {
					setColor(6, 0);
					if (PC_Direction == 0)
						printf("┗ ");
					else if (PC_Direction == 1)
						printf("┏ ");
					else if (PC_Direction == 2)
						printf("┏ ");
					else
						printf("┒ ");
					setColor(15, 0);
				}

				else if (PC_GunMode == 2) {

					setColor(10, 0);
					if (PC_Direction == 0)
						printf("∧");
					else if (PC_Direction == 1)
						printf("＞");
					else if (PC_Direction == 2)
						printf("∨");
					else
						printf("＜");
					setColor(15, 0);
				}
				else if (PC_GunMode == 3) {
					setColor(13, 0);
					if (PC_Direction == 0)
						printf("ㅐ");
					else if (PC_Direction == 1)
						printf("〓");
					else if (PC_Direction == 2)
						printf("ㅐ");
					else
						printf("〓");
					setColor(15, 0);
				}
				else if (PC_GunMode == 4) {
					setColor(4, 0);
					printf("♨");
					setColor(15, 0);
				}
				else {
					setColor(8, 0);
					printf("▣");
					setColor(15, 0);

				}
			}
		}
	}

	SetCurrentCursorPos(curPos.X, curPos.Y);

}
void Draw() {
	//모든 출력함수 호출
	DrawPlayer(PC_Model[PC_Direction]);
	if (score != StageClear)
		DrawNPC();

	DrawUI();

	DrawShot();


	if (score == StageClear) {

		DrawPlayer(PC_Model[PC_Direction]);
		Boss_DrawShot();
		DrawBoss(Boss_Model);
		DoProgress("Boss HP ", Boss_Health, 1000);

		return;
	}

	if (map_num == 1)
		DrawMap(curMap, 19, 21);
	else  if (map_num == 2) {
		DrawMap(curMap, 21, 21);
	}

}
int DetectPC_Collision(int posX, int posY) {
	int i, y;
	//진행방향 한칸 앞을 받아옴
	PC_mapPosition.X = (posX - GBOARD_ORIGIN_X) / 2;
	PC_mapPosition.Y = posY - GBOARD_ORIGIN_Y;

	if (curMap[PC_mapPosition.Y + 1][PC_mapPosition.X + 1] == 1)
		return 0;
	//_________________________________________________________________________--
	else if (curMap[PC_mapPosition.Y + 1][PC_mapPosition.X + 1] == 6) {
		if (mooj == 0)
		{
			hp--;
			mooj = 1;
			moojend = (count + 60) % 200;
		}
		return 0;
	}


	if (curMap[PC_mapPosition.Y + 1][PC_mapPosition.X + 1] >= 50 && curMap[PC_mapPosition.Y + 1][PC_mapPosition.X + 1] <= 53)
	{
		if (curMap[PC_mapPosition.Y + 1][PC_mapPosition.X + 1] == 50)
		{
			GUN2TRUE = 1;
			ammo[0] += 10;
		}
		else if (curMap[PC_mapPosition.Y + 1][PC_mapPosition.X + 1] == 51)
		{
			GUN3TRUE = 1;
			ammo[1] += 40;
		}
		else if (curMap[PC_mapPosition.Y + 1][PC_mapPosition.X + 1] == 52)
		{
			GUN4TRUE = 1;
			ammo[2] += 5;
		}
		else if (curMap[PC_mapPosition.Y + 1][PC_mapPosition.X + 1] == 53)
		{
			GUN5TRUE = 1;
			ammo[3] += 1;
		}
		curMap[PC_mapPosition.Y + 1][PC_mapPosition.X + 1] = 0;
		//printf("tlqkfqtlqtkmdslkfnsdlfbwe");
	}


	return 1;

}


void Collision_mapset(int i, int elem) {
	PC_mapPosition.X = (BULLET[i].x - GBOARD_ORIGIN_X) / 2;
	PC_mapPosition.Y = BULLET[i].y - GBOARD_ORIGIN_Y;
	curMap[PC_mapPosition.Y][PC_mapPosition.X] = elem;
	//map[PC_mapPosition.Y][PC_mapPosition.X] = elem;
}

void ShotAction(int count) {

	int i;
	for (i = 0; i < SHOT_MAX; i++) {

		if (BULLET[i].UseFlag == 1) {

			if (BULLET[i].Type == 1) {
				if (count % 3 == 0)
					ShootUpdate(i);
			}
			else if (BULLET[i].Type == 2) {

				ShootUpdate(i);
			}
			else if (BULLET[i].Type == 3) {
				if (count % 2 == 0)
					ShootUpdate(i);
			}
			else if (BULLET[i].Type == 4) {
				//
				mine(i);
			}




		}
	}
}

void ShootUpdate(int i) {

	SetCurrentCursorPos(BULLET[i].x, BULLET[i].y);
	printf("  ");

	if (BULLET[i].BulletDirect == 0) {

		if (DetectBullet_Collision(BULLET[i].x, BULLET[i].y - 1, BULLET[i].Type) == 0) {
			BULLET[i].UseFlag = 0;
			return;
		}


		BULLET[i].y--;

	}
	else if (BULLET[i].BulletDirect == 1) {


		if (DetectBullet_Collision(BULLET[i].x + 2, BULLET[i].y, BULLET[i].Type) == 0) {
			BULLET[i].UseFlag = 0;
			return;
		}

		BULLET[i].x += 2;

	}
	else if (BULLET[i].BulletDirect == 2) {

		if (DetectBullet_Collision(BULLET[i].x, BULLET[i].y + 1, BULLET[i].Type) == 0) {
			BULLET[i].UseFlag = 0;
			return;
		}

		BULLET[i].y++;

	}
	else {

		if (DetectBullet_Collision(BULLET[i].x - 2, BULLET[i].y, BULLET[i].Type) == 0) {
			BULLET[i].UseFlag = 0;
			return;
		}

		BULLET[i].x -= 2;

	}
}
void DrawShot() {

	int i;

	for (i = 0; i < SHOT_MAX; i++) {

		int x = BULLET[i].x;
		int y = BULLET[i].y;
		if (BULLET[i].UseFlag == 1) {

			SetCurrentCursorPos(x, y);

			if (BULLET[i].Type == 1)
				Gun1Print(i);
			else if (BULLET[i].Type == 2)
				Gun2Print(i);
			else if (BULLET[i].Type == 3)
				Gun3Print(i);
			else if (BULLET[i].Type == 4)
				Gun4Print(i);
		}
		SetCurrentCursorPos(Pos.X, Pos.Y);
	}
}
void PC_Shoot(char blockInfo[3][3]) {
	int x, y;

	COORD Bullet;
	COORD Bullet2;

	for (y = 0; y < 3; y++) {
		for (x = 0; x < 3; x++) {
			if (blockInfo[y][x] == 2) {
				Bullet.X = Pos.X + (x * 2);
				Bullet.Y = Pos.Y + y;
			}
			if (blockInfo[y][x] == 6) {
				Bullet2.X = Pos.X + (x * 2);
				Bullet2.Y = Pos.Y + y;
			}
		}
	}

	if (PC_GunMode == 1) {
		if (Gun1Time >= 10) {
			CreateBullet(Bullet);
			Gun1Time = 0;
		}
	}
	else if (PC_GunMode == 2) {
		if (Gun2Time >= 10) {
			CreateBullet(Bullet);
			Gun2Time = 0;
		}
	}

	else if (PC_GunMode == 3) {
		if (Gun3Time >= 3) {
			CreateBullet(Bullet);
			Gun3Time = 0;
		}
	}
	else if (PC_GunMode == 4)
		CreateBullet(Bullet2);

	else {
		if (ammo[3] > 0) {
			if (hp < 5)
				hp++;

			ammo[3]--;
		}
	}


	SetCurrentCursorPos(Pos.X, Pos.Y);

}
void CreateBullet(COORD Bullet) {

	int i;
	for (i = 0; i < SHOT_MAX; i++) {

		if (BULLET[i].UseFlag == 0 && curMap[Bullet.Y - GBOARD_ORIGIN_Y][(Bullet.X - GBOARD_ORIGIN_X) / 2] != 1) {

			BULLET[i].UseFlag = 1;
			BULLET[i].Type = PC_GunMode;
			if (PC_GunMode != 1)
				ammo[PC_GunMode - 2]--;
			BULLET[i].x = Bullet.X;
			BULLET[i].y = Bullet.Y;
			BULLET[i].BulletDirect = PC_Direction;

			if (ammo[PC_GunMode - 2] == 0)
			{
				if (PC_GunMode == 2)
				{
					GUN2TRUE = 0;
					PC_GunMode = 1;
					DrawGun();

				}
				else if (PC_GunMode == 3)
				{
					GUN3TRUE = 0;
					PC_GunMode = 1;
					DrawGun();
				}
				else if (PC_GunMode == 4)
				{
					GUN4TRUE = 0;
					PC_GunMode = 1;
					DrawGun();
				}
				else if (PC_GunMode == 5)
				{
					GUN5TRUE = 0;
					PC_GunMode = 1;
					DrawGun();
				}
			}
			return;

		}
	}
}