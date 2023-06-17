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