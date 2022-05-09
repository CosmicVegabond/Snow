#pragma once
#include "raylib.h"
#define MAX_SPEED 350.0f
#define CAMERA_SPEED 750.0f
#define MAX_OBJS 10
#define MAX_ENEMIES 15
#define MAX_ALLIES 14
#define SNOWBALL_SPEED 30000.0f
#define ZONE 300.0
#define BOUNDS 18
#define SCALE 2
#define WORLD 2200
//structs
typedef enum GameScreen {LOGO, MENU, GAMEPLAY, ENDING} GameScreen;

typedef enum  {EASY, MEDIUM, HARD, INSANE} Difficulty;

typedef struct{
    int enemies;
    int allies;
}Mode;

typedef struct {
	Vector2 position;
    float radius;
    float speed;
    bool flying;
    Rectangle dest;
    float counterTime;
    float randomValue;
}SnowBall;

typedef struct {
	Rectangle rect;
    Rectangle hat;
    Rectangle Id;
	float speed;
	int dir;
    SnowBall snowBall;
    int hearts;
    Rectangle heart1;
    Rectangle heart2;
    Rectangle heart3;
    Rectangle frameHeart1;
    Rectangle frameHeart2;
    Rectangle frameHeart3;
}Player;

typedef enum {Gray, Red} HatColor;
typedef enum {PLAYER, ALLY, ENEMY} Type;

typedef struct {
    Rectangle rect;
    Rectangle hat;
    float speed;
    int dir;
    int hearts;
    Rectangle frameBot;
    Rectangle frameHat;
    SnowBall snowBall;
    Rectangle heart1;
    Rectangle heart2;
    Rectangle heart3;
    Rectangle frameHeart1;
    Rectangle frameHeart2;
    Rectangle frameHeart3;
    bool counted;
    int dir0;
}BotEnemies;

typedef struct {
    Rectangle rect;
    Rectangle hat;
    float speed;
    int dir;
    int hearts;
    Rectangle frameBot;
    Rectangle frameHat;
    SnowBall snowBall;
    Rectangle heart1;
    Rectangle heart2;
    Rectangle heart3;
    Rectangle frameHeart1;
    Rectangle frameHeart2;
    Rectangle frameHeart3;
    bool counted;
    int dir0;
}BotAllies;

typedef enum {GLACIER, ROCK} ObjType;

typedef struct {
    ObjType type;
    int blocking;
    Rectangle rect;
    Rectangle frame;
}Obj;

//functions
void UpdatePlayer(void);
void DrawPlayer(void);

void UpdateObjects(void);
void DrawObjects(void);

//void UpdateSnowBall(void);

void Collision(Rectangle *rect, Rectangle *ob, float bounds);

void UpdateEnemies(int i);
void DrawEnemies(int i);

void UpdateAllies(int i);
void DrawAllies(int i);

void DrawMap(void);

void UpdateCameraCentre(void);


void Menu(void);
void Ending(void);
void InitGame(void);
//variables

