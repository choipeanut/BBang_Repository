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
void Gun1Print(int i) {

	setColor(14, 0);
	printf("○");
	setColor(15, 0);

}
void Gun2Print(int i) {

	setColor(10, 0);
	if (BULLET[i].BulletDirect == 0 || BULLET[i].BulletDirect == 2)
		printf("│ ");
	else
		printf("ㅡ");
	setColor(15, 0);

}
void Gun3Print(int i) {

	setColor(4, 0);
	printf("::");
	setColor(15, 0);

}
void Gun4Print(int i) {

	setColor(12, 0);
	printf("▩");
	setColor(15, 0);

}
void mine(int i) {
	SetCurrentCursorPos(BULLET[i].x, BULLET[i].y);

	if (DetectBullet_Collision(BULLET[i].x, BULLET[i].y, BULLET[i].Type) == 0) {
		BULLET[i].UseFlag = 0;
		return;
	}

	//Collision_mapset(i, 3);


}
int DetectBullet_Collision(int posX, int posY, int type) {

	int i;

	PC_mapPosition.X = (posX - GBOARD_ORIGIN_X) / 2;
	PC_mapPosition.Y = posY - GBOARD_ORIGIN_Y;

	if (curMap[PC_mapPosition.Y][PC_mapPosition.X] == 1)
		return 0;

	else if (curMap[PC_mapPosition.Y][PC_mapPosition.X] == 7) {

		if (type == 1)
			Boss_Health -= 15;
		else if (type == 2)
			Boss_Health -= 35;
		else if (type == 3)
			Boss_Health -= 15;
		else
			Boss_Health -= 35;

		return 0;

	}
	else if (curMap[PC_mapPosition.Y][PC_mapPosition.X] == 6)
	{
		for (i = 0; i < NPC_MAX; i++)
		{
			if (NPC[i].x == posX && NPC[i].y == posY && NPC[i].UseFlag == 1)
			{
				if (type == 1)
					NPC[i].NpcHealth--;
				else if (type == 2)
					NPC[i].NpcHealth -= 3;
				else if (type == 3)
					NPC[i].NpcHealth -= 1;
				else
					NPC[i].NpcHealth -= 3;

				if (NPC[i].NpcHealth <= 0)
				{
					NPC[i].UseFlag = 0;
					NPC_Collision_mapset(i, 0);
					SetCurrentCursorPos(posX, posY);
					printf("  ");
					score += 100;
					int prob;
					prob = rand() % 3;
					if (prob == 0)
					{
						NPC_ItemDrop(i, posX, posY);
					}
				}

				return 0;
			}
		}
	}
	return 1;
}




void DeleteGunPicture(int tmp) {

	for (int i = 0; i < 10; i++)
	{
		if (score != StageClear)
			SetCurrentCursorPos(tmp, 6 + i);
		else
			SetCurrentCursorPos(tmp, 6 + i);

		printf("                                    ");
	}
}
void DrawGun()
{
	int tmp;

	if (score == StageClear)
		tmp = 70;
	else
		tmp = 50;

	SetCurrentCursorPos(tmp, 5);
	printf("*******************");


	SetCurrentCursorPos(tmp, 16);
	printf("*******************");
	switch (PC_GunMode)
	{
	case 1:
		DeleteGunPicture(tmp);

		SetCurrentCursorPos(tmp, 9);

		setColor(6, 0);
		printf(" ______________           ");
		setColor(8, 0);
		SetCurrentCursorPos(tmp, 10);
		printf(" ■■■■■■");
		setColor(6, 0);
		printf("■                  ");
		setColor(8, 0);
		SetCurrentCursorPos(tmp, 11);
		printf("  ■■■                     ");
		SetCurrentCursorPos(tmp, 12);
		printf(" ■■                       ");
		SetCurrentCursorPos(tmp, 13);
		printf("■■                        ");
		setColor(15, 0);
		break;
	case 2:
		if (GUN2TRUE == 1)
		{
			DeleteGunPicture(tmp);
			SetCurrentCursorPos(tmp, 9);
			setColor(8, 0);
			printf("           ■■■■");
			SetCurrentCursorPos(tmp, 10);
			printf(" ■■■■■■■■■■■");
			setColor(15, 0);
			printf("========");
			setColor(8, 0);
			printf("■");
			setColor(10, 0);
			printf("■");
			setColor(8, 0);
			SetCurrentCursorPos(tmp, 11);
			printf(" ■■■   ■■  ■■");
			SetCurrentCursorPos(tmp, 12);
			printf(" ■■    ■");
			setColor(15, 0);
			break;
		}
		break;
	case 3:
		if (GUN3TRUE == 1)
		{
			DeleteGunPicture(tmp);
			SetCurrentCursorPos(tmp, 9);
			setColor(8, 0);
			printf(" ■■■■■■■■■");
			setColor(15, 0);
			printf("===");
			setColor(8, 0);
			printf("■");
			setColor(4, 0);
			printf("■");
			setColor(8, 0);
			SetCurrentCursorPos(tmp, 10);
			printf(" ■■  ■   ■");
			SetCurrentCursorPos(tmp, 11);
			printf("      ■    ■");
			SetCurrentCursorPos(tmp, 12);
			printf("              ■");
			setColor(15, 0);
			break;
		}
		break;
	case 4:
		if (GUN4TRUE == 1)
		{
			DeleteGunPicture(tmp);
			SetCurrentCursorPos(tmp, 7);
			printf("MINE / %d", ammo[2]);
			setColor(12, 0);
			SetCurrentCursorPos(tmp, 9);
			printf("        ■        ");
			SetCurrentCursorPos(tmp, 10);
			printf("    ■■■■■");
			SetCurrentCursorPos(tmp, 11);
			printf("  ■  ■■■  ■");
			SetCurrentCursorPos(tmp, 12);
			printf("■■■■■■■■■");
			setColor(15, 0);
			break;
		}
		break;
	case 5:
		if (GUN5TRUE == 1)
		{
			DeleteGunPicture(tmp);
			SetCurrentCursorPos(tmp, 7);
			printf("HEALPACK / %d", ammo[3]);
			SetCurrentCursorPos(tmp, 8);
			setColor(15, 0);
			printf("    ■■■");
			SetCurrentCursorPos(tmp, 9);
			printf("  ■  ");
			setColor(4, 0);
			printf("■  ");
			setColor(15, 0);
			printf("■  ");
			SetCurrentCursorPos(tmp, 10);
			printf("  ■  ");
			setColor(4, 0);
			printf("■  ");
			setColor(15, 0);
			printf("■  ");
			setColor(4, 0);
			SetCurrentCursorPos(tmp, 11);
			setColor(15, 0);
			printf("  ■");
			setColor(4, 0);
			printf("■■■");
			setColor(15, 0);
			printf("■    ");
			setColor(15, 0);
			SetCurrentCursorPos(tmp, 12);
			printf("  ■  ");
			setColor(4, 0);
			printf("■  ");
			setColor(15, 0);
			printf("■  ");
			SetCurrentCursorPos(tmp, 13);
			printf("  ■  ");
			setColor(4, 0);
			printf("■  ");
			setColor(15, 0);
			printf("■  ");
			SetCurrentCursorPos(tmp, 14);
			printf("    ■■■");
			setColor(15, 0);
			break;
		}
		break;
	}
}

void DrawUI()
{
	int tmp;
	if (score != StageClear) {
		SetCurrentCursorPos(50, 17);
		setColor(10, 0);
		printf("SCORE : ");
		setColor(15, 0);
		tmp = 50;
		SetCurrentCursorPos(58, 17);
		setColor(10, 0);
		printf("%d", score);
		setColor(15, 0);
	}
	else
		tmp = 70;



	SetCurrentCursorPos(tmp, 19);
	printf("                          ");
	SetCurrentCursorPos(tmp, 19);
	for (int i = 0; i < hp; i++)
	{
		setColor(4, 0);
		printf("♥ ");
		setColor(15, 0);
	}

}


void CreateNpc(int x, int y) {
	COORD Npc;

	Npc.X = 2 * x + GBOARD_ORIGIN_X;
	Npc.Y = y + GBOARD_ORIGIN_Y;

	int i;
	int n;
	for (i = 0; i < NPC_MAX; i++) {

		if (NPC[i].UseFlag == 0) {

			NPC[i].UseFlag = 1;

			int n = rand() % 2;

			if (n == 0)
				NPC[i].Type = 0;
			else
				NPC[i].Type = 1;

			NPC[i].x = Npc.X;
			NPC[i].y = Npc.Y;
			//NPC[i].NpcDirect = rand() % 4;
			if (x == 1)
			{
				NPC[i].NpcDirect = 1;
			}
			else
			{
				NPC[i].NpcDirect = 3;
			}

			if (NPC[i].Type == 0)
			{
				NPC[i].NpcHealth = 3;
			}
			else if (NPC[i].Type == 1)
			{
				NPC[i].NpcHealth = 2;
			}

			return;
		}
	}
}
void DrawNPC() {

	int i;

	for (i = 0; i < NPC_MAX; i++) {

		int x = NPC[i].x;
		int y = NPC[i].y;
		x /= 2;
		x *= 2;
		if (NPC[i].UseFlag == 1) {

			SetCurrentCursorPos(x, y);

			if (NPC[i].Type == 0)
				setColor(2, 0);
			else
				setColor(12, 0);

			if (NPC[i].NpcHealth == 3)
				setColor(2, 0);
			else if (NPC[i].NpcHealth == 2)
				setColor(12, 0);
			else
				setColor(4, 0);
			printf("☎");

			setColor(15, 0);

			NPC_Collision_mapset(i, 6);

		}
		SetCurrentCursorPos(Pos.X, Pos.Y);
	}
}
void NpcAction(int count) {

	int i;
	for (i = 0; i < NPC_MAX; i++)
	{
		if (NPC[i].UseFlag == 1)
		{
			if (NPC[i].Type == 0 || NPC[i].Type == 1)
			{
				if (score < 2000)
				{
					if (count % 20 == 0)
						NpcUpdate(i);
				}
				else if (score < 5000)
				{
					if (count % 15 == 0)
						NpcUpdate(i);
				}
				else
				{
					if (count % 10 == 0)
						NpcUpdate(i);
				}

			}
		}
	}
}
void NpcUpdate(int i)//종민수정
{

	SetCurrentCursorPos(NPC[i].x, NPC[i].y);
	printf("  ");

	int dirtmp;
	if (DetectNPC_Collision(NPC[i].x, NPC[i].y, -1) == 2)
	{
		dirtmp = rand() % 4;
		while (DetectNPC_Collision(NPC[i].x, NPC[i].y, dirtmp) == 0 || NPC[i].NpcDirect - dirtmp == 2 || NPC[i].NpcDirect - dirtmp == -2)
		{
			dirtmp = rand() % 4;
		}
		NPC[i].NpcDirect = dirtmp;
		if (dirtmp == 0)
		{
			NPC_Collision_mapset(i, 0);
			NPC[i].y--;
			return 0;
		}
		else if (dirtmp == 1)
		{
			NPC_Collision_mapset(i, 0);
			NPC[i].x += 2;
			return 0;
		}
		else if (dirtmp == 2)
		{
			NPC_Collision_mapset(i, 0);
			NPC[i].y++;
			return 0;
		}
		else if (dirtmp == 3)
		{
			NPC_Collision_mapset(i, 0);
			NPC[i].x -= 2;
			return 0;
		}
		return;
	}
	else if (NPC[i].NpcDirect == 0)
	{
		if (DetectNPC_Collision(NPC[i].x, NPC[i].y, 0) == 0)
		{
			dirtmp = rand() % 4;
			while (DetectNPC_Collision(NPC[i].x, NPC[i].y, dirtmp) == 0)
			{
				dirtmp = rand() % 4;
			}
			NPC[i].NpcDirect = dirtmp;
			return;
		}

		NPC_Collision_mapset(i, 0);
		NPC[i].y--;
	}
	else if (NPC[i].NpcDirect == 1)
	{
		if (DetectNPC_Collision(NPC[i].x, NPC[i].y, 1) == 0)
		{
			dirtmp = rand() % 4;
			while (DetectNPC_Collision(NPC[i].x, NPC[i].y, dirtmp) == 0)
			{
				dirtmp = rand() % 4;
			}
			NPC[i].NpcDirect = dirtmp;
			return;
		}

		NPC_Collision_mapset(i, 0);
		NPC[i].x += 2;
	}
	else if (NPC[i].NpcDirect == 2)
	{
		if (DetectNPC_Collision(NPC[i].x, NPC[i].y, 2) == 0)
		{
			dirtmp = rand() % 4;
			while (DetectNPC_Collision(NPC[i].x, NPC[i].y, dirtmp) == 0)
			{
				dirtmp = rand() % 4;
			}
			NPC[i].NpcDirect = dirtmp;
			return;
		}

		NPC_Collision_mapset(i, 0);
		NPC[i].y++;

	}
	else if (NPC[i].NpcDirect == 3)
	{
		if (DetectNPC_Collision(NPC[i].x, NPC[i].y, 3) == 0)
		{
			dirtmp = rand() % 4;
			while (DetectNPC_Collision(NPC[i].x, NPC[i].y, dirtmp) == 0)
			{
				dirtmp = rand() % 4;
			}
			NPC[i].NpcDirect = dirtmp;
			return;
		}

		NPC_Collision_mapset(i, 0);
		NPC[i].x -= 2;
	}
}
int DetectNPC_Collision(int posX, int posY, int dir) { //종민수정
	COORD NPC_mapPosition;

	int x, y;
	x = posX;
	y = posY;
	if (dir == 0)
	{
		posY--;
	}
	else if (dir == 1)
	{
		posX += 2;
	}
	else if (dir == 2)
	{
		posY++;

	}
	else if (dir == 3)
	{
		posX -= 2;
	}

	NPC_mapPosition.X = (posX - GBOARD_ORIGIN_X) / 2;
	NPC_mapPosition.Y = posY - GBOARD_ORIGIN_Y;
	x = (x - GBOARD_ORIGIN_X) / 2;
	y = (y - GBOARD_ORIGIN_Y);

	if (curMap[NPC_mapPosition.Y][NPC_mapPosition.X] == 1)
		return 0;
	else if (curMap[NPC_mapPosition.Y][NPC_mapPosition.X] == 2 && mooj == 0)
	{
		hp--;
		mooj = 1;
		moojend = (count + 60) % 200;
		return 0;
	}
	else if (map_num == 1)//1번맵
	{
		for (int i = 0; i < 64; i++)
		{
			if (x == CROSSROAD[i].X && y == CROSSROAD[i].Y)
			{
				return 2;
			}
		}
	}
	else //2번맵
	{
		for (int i = 0; i < 49; i++)
		{
			if (x == CROSSROAD2[i].X && y == CROSSROAD2[i].Y)
			{
				return 2;
			}
		}
	}

	return 1;

}

void NPC_Collision_mapset(int i, int elem) {
	COORD NPC_mapPosition;
	NPC_mapPosition.X = (NPC[i].x - GBOARD_ORIGIN_X) / 2;
	NPC_mapPosition.Y = NPC[i].y - GBOARD_ORIGIN_Y;
	curMap[NPC_mapPosition.Y][NPC_mapPosition.X] = elem;
}
void NPC_ItemDrop(int i, int x, int y)
{
	COORD NPC_mapPosition;
	NPC_mapPosition.X = (NPC[i].x - GBOARD_ORIGIN_X) / 2;
	NPC_mapPosition.Y = NPC[i].y - GBOARD_ORIGIN_Y;

	int itemtype;
	itemtype = rand() % 4 + 50;
	curMap[NPC_mapPosition.Y][NPC_mapPosition.X] = itemtype; ////////////////////
}
void PC_GetItem()
{
	COORD PC_mapPosition;
	PC_mapPosition.X = (Pos.X - GBOARD_ORIGIN_X) / 2;
	PC_mapPosition.Y = Pos.Y - GBOARD_ORIGIN_Y;

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
	}

}


////////////////////////////////////BOSS






void Boss_Shoot() {// 보스맵의 3번부터 21전까지 x의 홀수 칸마다 총알 만들기 ///종민
   //int x, y;//흔히 쓰는 i,j와 같은 맥락

	COORD Boss_Bullet;//구초체 변수 선언

	int dir;

	dir = rand() % 4;
	if (dir == 0)
	{
		for (int i = 3; i <= 30; i += 2)
		{
			Boss_BulletPos.X = 2 * i + (GBOARD_ORIGIN_X / 2);//아래방향
			Boss_BulletPos.Y = GBOARD_ORIGIN_Y + 23; //1
			Boss_Bullet.X = Boss_BulletPos.X;
			Boss_Bullet.Y = Boss_BulletPos.Y; //구조체 변수에 총의 좌표 저장

			Boss_CreateBullet(Boss_Bullet, 0);
		}
	}
	else if (dir == 1)
	{
		for (int i = 1; i < 25; i += 2)
		{
			Boss_BulletPos.X = GBOARD_ORIGIN_X + 2;//아래방향
			Boss_BulletPos.Y = GBOARD_ORIGIN_Y + i; //1
			Boss_Bullet.X = Boss_BulletPos.X;
			Boss_Bullet.Y = Boss_BulletPos.Y; //구조체 변수에 총의 좌표 저장

			Boss_CreateBullet(Boss_Bullet, 1);
		}
	}
	else if (dir == 2)
	{
		for (int i = 4; i <= 30; i += 2)
		{
			Boss_BulletPos.X = 2 * i + (GBOARD_ORIGIN_X / 2);//아래방향
			Boss_BulletPos.Y = GBOARD_ORIGIN_Y + 1; //1
			Boss_Bullet.X = Boss_BulletPos.X;
			Boss_Bullet.Y = Boss_BulletPos.Y; //구조체 변수에 총의 좌표 저장

			Boss_CreateBullet(Boss_Bullet, 2);
		}
	}
	else if (dir == 3)
	{
		for (int i = 2; i < 24; i += 2)
		{
			Boss_BulletPos.X = GBOARD_ORIGIN_X + 56;//아래방향
			Boss_BulletPos.Y = GBOARD_ORIGIN_Y + i; //1
			Boss_Bullet.X = Boss_BulletPos.X;
			Boss_Bullet.Y = Boss_BulletPos.Y; //구조체 변수에 총의 좌표 저장

			Boss_CreateBullet(Boss_Bullet, 3);
		}
	}
}

void Boss_CreateBullet(COORD Boss_Bullet, int direction) {

	int i;
	for (i = 0; i < BOSS_MAX; i++) {

		if (BOSS[i].UseFlag == 0) {//useflag가 0이고 맵의 위치의 값이 1이 아니면 = 벽과 충돌이 아니라면

			BOSS[i].UseFlag = 1;//useflag를 1
			BOSS[i].x = Boss_Bullet.X;//
			BOSS[i].y = Boss_Bullet.Y;//좌표 저장
			BOSS[i].BossDirect = direction;//총알 방향은 아래
			return;
		}
	}
}

void Boss_ShotAction() {

	int i;
	for (i = 0; i < BOSS_MAX; i++) {

		if (BOSS[i].UseFlag == 1) {//총알의 useflag가 1이면
			SetCurrentCursorPos(BOSS[i].x, BOSS[i].y);//총알 좌표 등록
			printf("  ");//지우기
			//방향마다 

			if (BOSS[i].BossDirect == 0) {// 아래yyyy

				if (DetectBossBullet_Collision(BOSS[i].x, BOSS[i].y - 1) == 0) {
					BOSS[i].UseFlag = 0;//충돌검사
					//return;
				}
				else if (DetectBossBullet_Collision(BOSS[i].x, BOSS[i].y) == 2) {
					hp--;
					BOSS[i].UseFlag = 0;
				}

				else
					BOSS[i].y -= 1;//위로 한칸이동

			}

			else if (BOSS[i].BossDirect == 1) {// 왼 -> 오른

				if (DetectBossBullet_Collision(BOSS[i].x + 2, BOSS[i].y) == 0) {
					BOSS[i].UseFlag = 0;

					//return;
				}
				else if (DetectBossBullet_Collision(BOSS[i].x, BOSS[i].y) == 2) {

					hp--;
					BOSS[i].UseFlag = 0;
				}

				else
					BOSS[i].x += 2;//오른쪽으로 한칸이동

			}

			else if (BOSS[i].BossDirect == 2) {// 위

				if (DetectBossBullet_Collision(BOSS[i].x, BOSS[i].y + 1) == 0) {
					BOSS[i].UseFlag = 0;

					//return;
				}
				else if (DetectBossBullet_Collision(BOSS[i].x, BOSS[i].y) == 2) {
					hp--;
					BOSS[i].UseFlag = 0;
				}

				else
					BOSS[i].y++;//아래로 한칸이동

			}

			else if (BOSS[i].BossDirect == 3) {// 오른->왼

				if (DetectBossBullet_Collision(BOSS[i].x - 2, BOSS[i].y) == 0) {
					BOSS[i].UseFlag = 0;

					//return;
				}
				else if (DetectBossBullet_Collision(BOSS[i].x, BOSS[i].y) == 2) {
					hp--;
					BOSS[i].UseFlag = 0;
				}

				else
					BOSS[i].x -= 2;//왼쪽으로 이동

			}
		}
	}
}




void Boss_DrawShot() {

	int i;

	for (i = 0; i < BOSS_MAX; i++) {

		int x = BOSS[i].x;
		int y = BOSS[i].y;
		if (BOSS[i].UseFlag == 1) {

			SetCurrentCursorPos(x, y);
			printf("☆");//별모양 출력

		}
		SetCurrentCursorPos(Boss_Pos.X, Boss_Pos.Y);
	}


}

//////////BOSS HP ///////////

void DoProgress(char label[], int step, int total)
{
	//progress width
	const int pwidth = 72;

	//minus label len
	int width = pwidth - strlen(label);
	int pos = (step * width) / total;


	int percent = (step * 100) / total;

	//set green text color, only on Windows
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
	SetCurrentCursorPos(2, 3);
	printf("%s[", label);

	//fill progress bar with =
	for (int i = 0; i < pos; i++)
		printf("%c", '=');

	//fill progress bar with spaces
	printf("% *c", width - pos + 1, ']');
	printf(" %3d%%\r", percent);

	setColor(15, 0);
	//reset text color, only on Windows
	//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x08);
}

void DrawBoss(char blockInfo[5][8]) {

	int x, y;
	COORD Boss_mapPosition;
	COORD curPos = Boss_Pos;

	for (y = 0; y < 5; y++) {

		for (x = 0; x < 8; x++) {
			SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y);
			Boss_mapPosition.X = (curPos.X - GBOARD_ORIGIN_X) / 2;
			Boss_mapPosition.Y = curPos.Y - GBOARD_ORIGIN_Y;

			if (blockInfo[y][x] == 7) {
				curMap[Boss_mapPosition.Y + y][Boss_mapPosition.X + x] = 7;
				printf("■");
			}
			else if (blockInfo[y][x] == 8) {
				curMap[Boss_mapPosition.Y + y][Boss_mapPosition.X + x] = 8;
				printf("@@");
			}
			else if (blockInfo[y][x] == 0) {
				curMap[Boss_mapPosition.Y + y][Boss_mapPosition.X + x] = 0;
				printf("  ");
			}
			else if (blockInfo[y][x] == 9) {
				curMap[Boss_mapPosition.Y + y][Boss_mapPosition.X + x] = 9;
				printf("--");
			}
			else if (blockInfo[y][x] == 10) {
				curMap[Boss_mapPosition.Y + y][Boss_mapPosition.X + x] = 10;
				printf("＼");
			}
			else {
				curMap[Boss_mapPosition.Y + y][Boss_mapPosition.X + x] = 11;
				printf("／");
			}

		}
	}


	SetCurrentCursorPos(curPos.X, curPos.Y);
}

int DetectBossBullet_Collision(int posX, int posY) {


	Boss_mapPosition.X = (posX - GBOARD_ORIGIN_X) / 2;
	Boss_mapPosition.Y = posY - GBOARD_ORIGIN_Y;

	if (curMap[Boss_mapPosition.Y][Boss_mapPosition.X] == 1)
		return 0;
	else if (curMap[Boss_mapPosition.Y][Boss_mapPosition.X] == 2)
		return 2;
	return 1;
}
void Boss_DeleteBlock(char blockInfo[5][8]) {

	int x, y;
	COORD curPos = Boss_Pos;


	Boss_mapPosition.X = (curPos.X - GBOARD_ORIGIN_X) / 2;
	Boss_mapPosition.Y = curPos.Y - GBOARD_ORIGIN_Y;

	for (y = 0; y < 5; y++) {
		for (x = 0; x < 8; x++) {

			Boss_mapPosition.X = (curPos.X - GBOARD_ORIGIN_X) / 2;
			Boss_mapPosition.Y = curPos.Y - GBOARD_ORIGIN_Y;

			SetCurrentCursorPos(curPos.X + x * 2, curPos.Y + y);

			if (blockInfo[y][x] == 7 || blockInfo[y][x] == 8 || blockInfo[y][x] == 9 || blockInfo[y][x] == 10 || blockInfo[y][x] == 11 || blockInfo[y][x] == 0) {
				printf("  ");
				char Boss_Model[5][8]
					= {

					   {0,0,7,7,7,7,0,0},
					   {0,7,0,10,11,0,7,0},
					   {7,0,8,0,0,8,0,7},
					   {0,7,0,9,9,0,7,0},
					   {0,0,7,7,7,7,0,0},

				};
				if (blockInfo[y][x] == 7)
					curMap[Boss_mapPosition.Y][Boss_mapPosition.X] == 7;
				else if (blockInfo[y][x] == 8)
					curMap[Boss_mapPosition.Y][Boss_mapPosition.X] == 8;
				else if (blockInfo[y][x] == 9)
					curMap[Boss_mapPosition.Y][Boss_mapPosition.X] == 9;
				else if (blockInfo[y][x] == 10)
					curMap[Boss_mapPosition.Y][Boss_mapPosition.X] == 10;
				else if (blockInfo[y][x] == 11)
					curMap[Boss_mapPosition.Y][Boss_mapPosition.X] == 11;
				else
					curMap[Boss_mapPosition.Y][Boss_mapPosition.X] == 0;
			}
		}
	}
	SetCurrentCursorPos(curPos.X, curPos.Y);


}
void Boss_ShiftRight() {


	if (DetectBossBullet_Collision(Boss_Pos.X + 16, Boss_Pos.Y) == 0) {
		Boss_Direction = 1;
		return;
	}

	Boss_DeleteBlock(Boss_Model);


	Boss_Pos.X += 2;
	DrawBoss(Boss_Model);

}
void Boss_ShiftLeft() {


	if (DetectBossBullet_Collision(Boss_Pos.X - 2, Boss_Pos.Y) == 0) {
		Boss_Direction = 0;
		return;
	}

	Boss_DeleteBlock(Boss_Model);


	Boss_Pos.X -= 2;
	DrawBoss(Boss_Model);

}