#include "raylib.h"
#include "header.h"
#include <math.h>
#include <stddef.h>

static Player player;
static int screenWidth = 1000;
static int screenHeight = 600;
static float deltaTime;
static Texture2D myLogo;
static Rectangle framePlayer;
static Rectangle frameGlacier;
static Rectangle frameRock;
static Rectangle frameHatRed;
static Rectangle frameId;
static Rectangle frameBounds;
static Rectangle frameBall;
static Rectangle frameBallA;
static Rectangle frameHeartFull;
static Rectangle frameHeartEmpty;
static Texture2D playerT;
static Texture objT;
static Obj objects[MAX_OBJS];
static BotEnemies enemies[MAX_ENEMIES];
static BotAllies allies[MAX_ALLIES];
static Rectangle frameTile;
static Camera2D camera;
static bool state = false;
static HatColor hatColor = Red;
static int dir;
static bool gameOver = false;
static int counterHearts=0;
static Mode mode={MAX_ENEMIES, MAX_ALLIES};
static Difficulty diff=MEDIUM;
static bool active = false;

int main (void){
	
	InitWindow(screenWidth, screenHeight, "Snow");	
	InitGame();
	SetTargetFPS(60);

	GameScreen currentScreen = LOGO;
	int state = 0;
	int counter = 0;
	
while(!WindowShouldClose()){

	deltaTime = GetFrameTime();

	switch(currentScreen){
		case LOGO : {
			counter ++;
			if (counter>40)
				state = 1;
			if (counter>80)
				currentScreen=MENU;
		}break;
		case MENU : {	    SetConfigFlags(FLAG_FULLSCREEN_MODE);
			//Menu();
			if(IsKeyPressed(KEY_F)){		
					if (IsWindowState(FLAG_FULLSCREEN_MODE))	ClearWindowState(FLAG_FULLSCREEN_MODE);
					else SetWindowState(FLAG_FULLSCREEN_MODE);}
			if(IsKeyPressed(KEY_V)){
				if (IsWindowState(FLAG_VSYNC_HINT))	ClearWindowState(FLAG_VSYNC_HINT);
				else	SetWindowState(FLAG_VSYNC_HINT);
			}
			if(IsKeyPressed(KEY_SPACE))	currentScreen=GAMEPLAY;
			if(IsKeyPressed(KEY_G))		hatColor = Gray;
			if(IsKeyPressed(KEY_R))		hatColor = Red;
            if(!active){
            	if(IsKeyPressed(KEY_ONE)){
		   			diff = EASY;
					mode = (Mode) {MAX_ENEMIES/3, MAX_ALLIES/2};
				}
           	 	if(IsKeyPressed(KEY_TWO)){
		  	 		diff = MEDIUM;
					mode = (Mode) {MAX_ENEMIES/2, MAX_ALLIES/2};
				}
         	   if(IsKeyPressed(KEY_THREE)){
			   		diff = HARD;
					mode = (Mode) {MAX_ENEMIES, MAX_ALLIES/2};
				}
        	    if(IsKeyPressed(KEY_FOUR)){
			   		diff = INSANE;
					mode = (Mode) {MAX_ENEMIES, MAX_ALLIES/4};
				}
			}
			if(IsKeyPressed(KEY_Q)){
				UnloadTexture(myLogo);
				UnloadTexture(playerT);
				UnloadTexture(objT);	
				CloseWindow();
				return 0;
			}
		}break;
		case GAMEPLAY :{
			//Gameplay();
			active = true;
			for(int i=0; i < mode.enemies; i++){
				if (enemies[i].counted){
					counterHearts++;
				}
                if (counterHearts==mode.enemies)	gameOver=true;
			}
            counterHearts=0;

			if(!gameOver){
                
            	UpdateCameraCentre();

				UpdatePlayer();
				for(int i=0; i<MAX_OBJS; i++){
					if(objects[i].blocking)
               			Collision(&(player.rect), &(objects[i].rect), 0.1);
				}


                for (int i= 0; i<mode.enemies; i++){
                    if(!enemies[i].counted){
                        if(enemies[i].hearts==0){
					        enemies[i]=(BotEnemies){0};
						    enemies[i].counted=true;
                        }
				        UpdateEnemies(i);
						for(int o = 0; o<MAX_OBJS; o++){
							if(objects[o].blocking)
               					Collision(&(enemies[i].rect), &(objects[o].rect), 0.4); //objects
						}
						if(i!=mode.enemies){
							for(int n=i+1; n<mode.enemies; n++){
								Collision(&(enemies[i].rect), &(enemies[n].rect), 0.2);
							}
						}
						if(i!=0){
							for(int m=i-1; m>=0; m--){
								Collision(&(enemies[i].rect), &(enemies[m].rect), 0.2);
							}
						}
                        enemies[i].hat.x=enemies[i].rect.x;
	                    enemies[i].hat.y=enemies[i].rect.y;
					}
                }
                for (int i= 0; i<mode.allies; i++){

                    if(!allies[i].counted){
                        if(allies[i].hearts==0){
					        allies[i]=(BotAllies){0};
						    allies[i].counted=true;
                        }
                    	UpdateAllies(i);
						for(int o = 0; o<MAX_OBJS; o++){
							if(objects[o].blocking)
               					Collision(&(allies[i].rect), &(objects[o].rect), 0.4); //objects
						}
						for (int j= 0; j<mode.enemies; j++){
               				Collision(&(allies[i].rect), &(enemies[j].rect), 0.2);	//enemies
						}
						if(i!=mode.allies){
							for(int n=i+1; n<mode.allies; n++){
								Collision(&(allies[i].rect), &(allies[n].rect), 0.2);	//allies after i
							}
						}
						if(i!=0){
							for(int m=i-1; m>=0; m--){
								Collision(&(allies[i].rect), &(allies[m].rect), 0.2);//allies before i
							}
						}
                        allies[i].hat.x=allies[i].rect.x;
	                    allies[i].hat.y=allies[i].rect.y;
					}
			    }

				player.Id.x=player.rect.x;
				player.Id.y=player.rect.y;
				player.hat.x = player.rect.x;
				player.hat.y = player.rect.y;

				if(IsKeyPressed(KEY_M))	currentScreen=MENU;
			}
			else{
				currentScreen=ENDING;
			}
		}break;
		case ENDING : {
			//ENDING();
			if(IsKeyPressed(KEY_SPACE)){
				InitGame();
				active = false;
				currentScreen = MENU;
			}
			if(IsKeyPressed(KEY_Q)){
				UnloadTexture(myLogo);
				UnloadTexture(playerT);
				UnloadTexture(objT);	
				CloseWindow();
				return 0;
			}
		}break;
	default:break;
	}
	BeginDrawing();
	ClearBackground(RAYWHITE);
	switch(currentScreen){
		case LOGO : {
			if (state==0){
				DrawRectangle(screenWidth/2 - 128, screenHeight/2 - 128, 256, 256, BLACK);
            	DrawRectangle(screenWidth/2 - 112, screenHeight/2 - 112, 224, 224, RAYWHITE);
            	DrawText("raylib", screenWidth/2 - 44, screenHeight/2 + 48, 50, BLACK);
			}	
			if (state==1)
				DrawTextureEx (myLogo, (Vector2){screenWidth/2 -myLogo.width*(10/2), screenHeight/2 -myLogo.height*(10/2)}, 0.0f, 10.0f , WHITE);// 
		}break;
		case MENU : {
			//Menu();
			DrawText("Press F to toggle fullscreen mode.", 350, 225, 15, BLACK);
			DrawText("Press V to toggle Vsync mode.", 350, 250, 15, BLACK);
			DrawText("Press G to play with gray hats.", 350, 275, 15, BLACK);
			DrawText("Press R to play with red hats.", 350, 300, 15, BLACK);
			if(!active){
				DrawText("Press SPACE to play.", 350, 175, 17, BLACK);
				DrawText("Choose difficulty from easy to insane.", 350, 325, 16, BLACK);

					DrawText("Press 1 for EASY mode.", 350, 345, 14, BLACK);
					DrawText("Press 2 for MEDIUM mode(default).", 350, 360, 14, BLACK);
					DrawText("Press 3 for HARD mode.", 350, 375, 14, BLACK);
					DrawText("Press 4 for INSANE mode.", 350, 390, 14, BLACK);

				if(diff==EASY)  		DrawText("<--", 600, 345, 15, BLACK);
				else if(diff==MEDIUM) 	DrawText("<--", 600, 360, 15, BLACK);
				else if(diff==HARD)		DrawText("<--", 600, 375, 15, BLACK);
				else if(diff==INSANE){
				 	DrawText("<--", 600, 390, 15, BLACK);
				 	DrawText("(lol, gl)", 360, 410, 12, BLACK);
				}
			}
			else	DrawText("Press SPACE to resume.", 350, 175, 17, BLACK);
			DrawText("Press Q to quit.", 350, 460, 17, BLACK);
		}break;
		case GAMEPLAY :{
			//Gameplay();
            	BeginMode2D(camera);
   					DrawMap();

			   	 	DrawPlayer();
                    for(int i=0; i<mode.enemies; i++){
                        if(!enemies[i].counted)			    	    
                            DrawEnemies(i);
                    }
                    for(int i=0; i<mode.allies; i++){
                        if(!allies[i].counted)			    	    
					        DrawAllies(i);
                    }
					DrawObjects();
            	EndMode2D();
				//if (!IsWindowState(FLAG_VSYNC_HINT)){
					DrawTexturePro(objT, player.frameHeart1, player.heart1, (Vector2){0,0}, 0.0f, WHITE);
					DrawTexturePro(objT, player.frameHeart2, player.heart2, (Vector2){0,0}, 0.0f, WHITE);
					DrawTexturePro(objT, player.frameHeart3, player.heart3, (Vector2){0,0}, 0.0f, WHITE);
				//}
				/*else {
					DrawTexturePro(objT, player.frameHeart1, (Rectangle)
					{player.heart1.x,player.heart1.y-70,player.heart1.width,player.heart1.height},(Vector2){0,0}, 0.0f, WHITE);
					DrawTexturePro(objT, player.frameHeart1, (Rectangle)
					{player.heart2.x,player.heart2.y-70,player.heart2.width,player.heart2.height},(Vector2){0,0}, 0.0f, WHITE);
					DrawTexturePro(objT, player.frameHeart1, (Rectangle)
					{player.heart3.x,player.heart3.y-70,player.heart3.width,player.heart3.height},(Vector2){0,0}, 0.0f, WHITE);
				}*/
				DrawText("Press R to reset zoom.", 875, 590, 10, BLACK);
				DrawText("Press M for Menu.", 10, 590, 10, BLACK);
		}break;
		case ENDING : {
			//ENDING();
			DrawRectangle(0, screenHeight/2.0f - 200, screenWidth, 400, SKYBLUE);
			DrawText("GAME OVER, Press SPACE to play again, Q to exit.", 100, screenHeight/2.0f, 30, WHITE);
		}break;
	default:break;
	}
	EndDrawing();

}
UnloadTexture(myLogo);
UnloadTexture(playerT);
UnloadTexture(objT);
CloseWindow();
return 0;
}

void InitGame(void){
gameOver = false;

myLogo = LoadTexture("resources/myLogo.png");
objT = LoadTexture("resources/SpriteSheet.png");
playerT = LoadTexture("resources/Penguine_2.png");

mode.enemies = MAX_ENEMIES;
mode.allies = MAX_ALLIES;
//frames
framePlayer = (Rectangle){0.0f, 0.0f , playerT.width/8, playerT.height};

frameRock = (Rectangle){74, 18, 64, 64};

frameGlacier = (Rectangle) {0, 0, 74, 82};

frameTile = (Rectangle){138, 40, 42, 42};

frameHatRed = (Rectangle){75, 0, 17, 9};

frameId = (Rectangle){133, 0, 23, 7};

frameBounds = (Rectangle){180, 40, 42, 42};

frameBall = (Rectangle){126, 7, 9, 9};
frameBallA = (Rectangle){126, 0, 6, 6};

frameHeartFull = (Rectangle){156, 0, 12, 12};
frameHeartEmpty = (Rectangle){168, 0, 12, 12};

player.frameHeart1=frameHeartFull;
player.frameHeart2=frameHeartFull;
player.frameHeart3=frameHeartFull;

//player
player.rect.width=framePlayer.width*SCALE;
player.rect.height=framePlayer.height*SCALE;
player.rect.x = WORLD/2.0f-player.rect.width/2.0f;
player.rect.y = WORLD/2.0f-player.rect.height/2.0f;
player.dir=6;
player.hat = (Rectangle){player.rect.x, player.rect.y, frameHatRed.width*SCALE, frameHatRed.height*SCALE};
player.Id = (Rectangle){player.rect.x, player.rect.y, frameId.width*SCALE, frameId.height*SCALE};
player.snowBall.position.x = player.rect.x;
player.snowBall.position.y = player.rect.y;
player.snowBall.radius = 20;
player.snowBall.flying = false;
player.snowBall.dest.width = frameBall.width*(SCALE+1);
player.snowBall.dest.height = frameBall.height*(SCALE+1);
player.snowBall.dest.x = player.snowBall.position.x - player.snowBall.dest.width/2.0f;
player.snowBall.dest.y = player.snowBall.position.y - player.snowBall.dest.height/2.0f;
player.snowBall.counterTime = 0;
player.hearts = 3;
player.heart1 = (Rectangle){50, 550, frameHeartFull.width*(SCALE+1), frameHeartEmpty.height*(SCALE+1)};
player.heart2 = (Rectangle){96, 550, frameHeartFull.width*(SCALE+1), frameHeartEmpty.height*(SCALE+1)};
player.heart3 = (Rectangle){142, 550, frameHeartFull.width*(SCALE+1), frameHeartEmpty.height*(SCALE+1)};

//objects
int type;
for (int j = 0; j<MAX_OBJS; j++){
type = GetRandomValue(0,1);
objects[j].blocking=1;
switch(type){
	case 0 : {
		objects[j].type =GLACIER;
		objects[j].rect.width = frameGlacier.width*SCALE;
		objects[j].rect.height = frameGlacier.height*SCALE*2/3;
		objects[j].frame.width = frameGlacier.width*SCALE;
		objects[j].frame.height = frameGlacier.height*SCALE;
		objects[j].rect.x = GetRandomValue(150, WORLD-200);
		objects[j].frame.x= objects[j].rect.x;
		objects[j].rect.y = GetRandomValue(150, WORLD-200);
		objects[j].frame.y= objects[j].rect.y-frameGlacier.height*SCALE*1/3;
		} break;
	case 1 : {
		objects[j].type =ROCK;
		objects[j].rect.width = frameRock.width*SCALE;
		objects[j].rect.height = frameRock.height*SCALE;
		objects[j].frame.width = frameRock.width*SCALE;
		objects[j].frame.height = frameRock.height*SCALE;
		objects[j].rect.x = GetRandomValue(150, WORLD-200);
		objects[j].frame.x= objects[j].rect.x;
		objects[j].rect.y = GetRandomValue(150, WORLD-200);
		objects[j].frame.y= objects[j].rect.y;
		} break;
	default : break;
	}
//counterW+=GetRandomValue(148, 200);
//for(int k = 0; objects[j].rect.y<WORLD; k++){
	//counterH


}
//BotsEnemies
for (int i = 0; i<mode.enemies; i++){
enemies[i].dir=6;
enemies[i].frameBot.width=23;
enemies[i].frameBot.height=41;
enemies[i].rect.width=enemies[i].frameBot.width*SCALE;
enemies[i].rect.height=enemies[i].frameBot.height*SCALE;

enemies[i].rect.x = GetRandomValue(frameBounds.width, WORLD - enemies[i].rect.width - frameBounds.width);
enemies[i].rect.y = GetRandomValue(frameBounds.height, WORLD - enemies[i].rect.height - frameBounds.height);

enemies[i].frameHat.width=17;
enemies[i].frameHat.height=9;


enemies[i].hat.x = enemies[i].rect.x;
enemies[i].hat.y = enemies[i].rect.y;
enemies[i].hat.width = enemies[i].frameHat.width*SCALE;
enemies[i].hat.height = enemies[i].frameHat.height*SCALE;

enemies[i].hearts = 3;
enemies[i].frameHeart1=frameHeartFull;
enemies[i].frameHeart2=frameHeartFull;
enemies[i].frameHeart3=frameHeartFull;

enemies[i].heart1=(Rectangle){enemies[i].rect.x-5, enemies[i].rect.y-26, frameHeartFull.width*(SCALE*3/4), frameHeartEmpty.height*(SCALE*3/4)};
enemies[i].heart2=(Rectangle){enemies[i].rect.x+15, enemies[i].rect.y-26, frameHeartFull.width*(SCALE*3/4), frameHeartEmpty.height*(SCALE*3/4)};
enemies[i].heart3=(Rectangle){enemies[i].rect.x+35, enemies[i].rect.y-26, frameHeartFull.width*(SCALE*3/4), frameHeartEmpty.height*(SCALE*3/4)};

enemies[i].counted = false;

enemies[i].snowBall.flying = false;

enemies[i].snowBall.dest.width = frameBall.width*(SCALE+1);
enemies[i].snowBall.dest.height = frameBall.height*(SCALE+1);
enemies[i].snowBall.dest.x = enemies[i].snowBall.position.x - enemies[i].snowBall.dest.width/2.0f;
enemies[i].snowBall.dest.y = enemies[i].snowBall.position.y - enemies[i].snowBall.dest.height/2.0f;
enemies[i].snowBall.counterTime = 0;

}
//botsAllies
for (int i = 0; i<mode.allies; i++){
allies[i].dir=6;
allies[i].frameBot.width=23;
allies[i].frameBot.height=41;
allies[i].rect.width=allies[i].frameBot.width*SCALE;
allies[i].rect.height=allies[i].frameBot.height*SCALE;

allies[i].rect.x = GetRandomValue(frameBounds.width, WORLD - allies[i].rect.width - frameBounds.width);
allies[i].rect.y = GetRandomValue(frameBounds.height, WORLD - allies[i].rect.height - frameBounds.height);

allies[i].frameHat.width=17;
allies[i].frameHat.height=9;


allies[i].hat.x = allies[i].rect.x; 	
allies[i].hat.y = allies[i].rect.y;
allies[i].hat.width = allies[i].frameHat.width*SCALE;
allies[i].hat.height = allies[i].frameHat.height*SCALE;

allies[i].hearts = 3;
allies[i].frameHeart1=frameHeartFull;
allies[i].frameHeart2=frameHeartFull;
allies[i].frameHeart3=frameHeartFull;

allies[i].heart1=(Rectangle){allies[i].rect.x-5, allies[i].rect.y-26, frameHeartFull.width*(SCALE*3/4), frameHeartEmpty.height*(SCALE*3/4)};
allies[i].heart2=(Rectangle){allies[i].rect.x+15, allies[i].rect.y-26, frameHeartFull.width*(SCALE*3/4), frameHeartEmpty.height*(SCALE*3/4)};
allies[i].heart3=(Rectangle){allies[i].rect.x+35, allies[i].rect.y-26, frameHeartFull.width*(SCALE*3/4), frameHeartEmpty.height*(SCALE*3/4)};

enemies[i].counted = false;

allies[i].snowBall.flying = false;

allies[i].snowBall.dest.width = frameBallA.width*(SCALE+1);
allies[i].snowBall.dest.height = frameBallA.height*(SCALE+1);
allies[i].snowBall.dest.x = allies[i].snowBall.position.x - allies[i].snowBall.dest.width/2.0f;
allies[i].snowBall.dest.y = allies[i].snowBall.position.y - allies[i].snowBall.dest.height/2.0f;
allies[i].snowBall.counterTime = 0;

}

//camera
camera.target = (Vector2){player.rect.x + (player.rect.width/2), player.rect.y + (player.rect.height/2)};
camera.offset = (Vector2){screenWidth/2, screenHeight/2};
camera.rotation = 0.0f;
camera.zoom = 1.0f;

}

void UpdatePlayer(void){
    player.speed = MAX_SPEED*deltaTime;
    Rectangle *p = &(player.rect);

//health
if(player.hearts == 0){
	gameOver=true;
	player.frameHeart1 = frameHeartEmpty;
}
if(player.hearts == 1)
	player.frameHeart2 = frameHeartEmpty;
if(player.hearts == 2)
	player.frameHeart3 = frameHeartEmpty;
//player bounds collision
if(p->x > WORLD - p->width - frameBounds.width) p->x = WORLD - p->width - frameBounds.width;
if(p->x < frameBounds.width) p->x = frameBounds.width;
if(p->y > WORLD - p->height - frameBounds.height) p->y = WORLD - p->height - frameBounds.height;
if(p->y < frameBounds.height) p->y = frameBounds.height;
//movement
if(IsKeyDown(KEY_D)){
	if(!IsKeyDown(KEY_LEFT_SHIFT))
    	p->x += player.speed;
    player.dir=5;
    }
if(IsKeyDown(KEY_A)){
	if(!IsKeyDown(KEY_LEFT_SHIFT))
    	p->x -= player.speed;
    player.dir=1;
    }
if(IsKeyDown(KEY_W)){
	if(!IsKeyDown(KEY_LEFT_SHIFT))
		p->y -= player.speed;
    player.dir=7;
    }
if(IsKeyDown(KEY_S)){
	if(!IsKeyDown(KEY_LEFT_SHIFT))
    	p->y += player.speed;
    player.dir=3;
    }
if(IsKeyDown(KEY_S)&&IsKeyDown(KEY_D))
    player.dir=4;
if(IsKeyDown(KEY_S)&&IsKeyDown(KEY_A))
    player.dir=2;
if(IsKeyDown(KEY_W)&&IsKeyDown(KEY_A))
    player.dir=0;
if(IsKeyDown(KEY_W)&&IsKeyDown(KEY_D))
    player.dir=6;
//throwing snowballs

SnowBall *ball = &(player.snowBall);
ball->speed = SNOWBALL_SPEED*deltaTime;

if (ball->flying == false){
	ball->position.x = player.rect.x;
	ball->position.y = player.rect.y+55;
	dir = player.dir;
	ball->dest.x = ball->position.x - ball->dest.width/2.0f;
	ball->dest.y = ball->position.y - ball->dest.height/2.0f;
    ball->counterTime += deltaTime;
}

if (IsKeyDown(KEY_SPACE)){
    ball->randomValue=GetRandomValue(1,2);
	if(ball->counterTime > ball->randomValue)
		ball->flying = true;
}

if (ball->flying == true){
    switch(dir){
        case 0 : {	
			ball->position.x -= ball->speed*deltaTime;
			ball->position.y -= ball->speed*deltaTime;
		} break;
        case 1 : {	
			ball->position.x -= ball->speed*deltaTime;
		} break;
        case 2 : {	
			ball->position.x -= ball->speed*deltaTime;
			ball->position.y += ball->speed*deltaTime;
		} break;
        case 3 : {	
			ball->position.y += ball->speed*deltaTime;
		} break;
        case 4 : {	
			ball->position.x += ball->speed*deltaTime;
			ball->position.y += ball->speed*deltaTime;
		} break;
        case 5 : {	
			ball->position.x += ball->speed*deltaTime;
		} break;
        case 6 : {	
			ball->position.x += ball->speed*deltaTime;
			ball->position.y -= ball->speed*deltaTime;
		} break;
        case 7 : {	
			ball->position.y -= ball->speed*deltaTime;
		} break;
    default :break;
    }
	ball->dest.x = ball->position.x - ball->dest.width/2.0f;
	ball->dest.y = ball->position.y - ball->dest.height/2.0f;
}
//ball collision with objects
for(int i= 0; i<MAX_OBJS;i++){
    if(CheckCollisionCircleRec(ball->position, ball->radius, objects[i].rect)&&(ball->flying == true)){
        ball->flying = false;
        ball->counterTime=0;
}}
//ball collision with enemies
for(int j= 0; j<mode.enemies;j++){
    if(CheckCollisionCircleRec(ball->position, ball->radius, enemies[j].rect)&&(ball->flying == true)){
        ball->flying = false;
        enemies[j].hearts --;
        ball->counterTime=0;
    }
}
//ball collision with walls
if(((ball->position.x > WORLD)||(ball->position.x < frameBounds.width)||
    (ball->position.y > WORLD)||(ball->position.y < frameBounds.height))&&(ball->flying == true)){
	ball->flying = false;
    ball->counterTime=0;}
//ball speed over
if((fabs(ball->position.x - player.rect.x)>ZONE*2||fabs(ball->position.y - player.rect.y)>ZONE*2)&&(ball->flying == true)){
    ball->flying = false; 
    ball->counterTime=0;
}

}

void DrawPlayer(void){
	switch(player.dir){
			case 0 : {
                framePlayer.x = (float)player.dir*(float)playerT.width/8;
				if(hatColor==Gray){
					frameHatRed.x = 92;
                	frameHatRed.y = 9;
                }
				else {
                	frameHatRed.x = 109;
                	frameHatRed.y = 0;}
				} break;
			case 1 : {
                framePlayer.x = (float)player.dir*(float)playerT.width/8;
				if(hatColor==Gray){
                	frameHatRed.x = 92;
					frameHatRed.y = 9;
                }
				else {
                	frameHatRed.x = 75;
                	frameHatRed.y = 0;}
				} break;
			case 2 : {
                framePlayer.x = (float)player.dir*(float)playerT.width/8;
				if(hatColor==Gray){
					frameHatRed.x = 92;
                	frameHatRed.y = 0; 
                }
				else {
                	frameHatRed.x = 75;
                	frameHatRed.y = 0; }
				} break;
			case 3 : {
                framePlayer.x = (float)player.dir*(float)playerT.width/8;
				if(hatColor==Gray){
					frameHatRed.x = 109;
                	frameHatRed.y = 9; 
                }
				else {
                	frameHatRed.x = 109;
                	frameHatRed.y = 0;}
				} break;
			case 4 : {
                framePlayer.x = (float)player.dir*(float)playerT.width/8;
				if(hatColor==Gray){
                	frameHatRed.x = 92;
                	frameHatRed.y = 9;
                }
				else {
                	frameHatRed.x = 75;
                	frameHatRed.y = 9;}
				} break;
			case 5 : {
                framePlayer.x = (float)player.dir*(float)playerT.width/8;
				if(hatColor==Gray){
                	frameHatRed.x = 92;
                	frameHatRed.y = 9;
                } 
				else {
                	frameHatRed.x = 75;
                	frameHatRed.y = 9;}
				} break;
			case 6 : {
                framePlayer.x = (float)player.dir*(float)playerT.width/8;
				if(hatColor==Gray){
                	frameHatRed.x = 109;
                	frameHatRed.y = 9;
                }
				else {
                	frameHatRed.x = 109;
                	frameHatRed.y = 0;}
				} break;
			case 7 : {
                framePlayer.x = (float)player.dir*(float)playerT.width/8;
				if(hatColor==Gray){
                	frameHatRed.x = 109;
                	frameHatRed.y = 9;
                }
				else {
                	frameHatRed.x = 109;
                	frameHatRed.y = 0;}
				} break;
			default : break;
			}
	DrawTexturePro(objT, frameId, player.Id, (Vector2){0.0f,-75.0f}, 0.0f, WHITE);
	DrawTexturePro(playerT, framePlayer, player.rect, (Vector2){0.0f,0.0f}, 0.0f, WHITE);
	DrawTexturePro(objT, frameHatRed, player.hat, (Vector2){-6.0f,10.0f}, 0.0f, WHITE);
	if(player.snowBall.flying==true)
		DrawTexturePro(objT, frameBall, player.snowBall.dest,(Vector2){0,0}, 0.0f, WHITE);
	else if(player.snowBall.flying == false){
		if(player.snowBall.counterTime>=player.snowBall.randomValue)
			DrawTexturePro(objT, frameBall, player.snowBall.dest,(Vector2){0,0}, 0.0f, WHITE);
	}
}

void DrawObjects(void){ 
for (int i = 0; i<MAX_OBJS; i++){
	switch(objects[i].type){
		case ROCK : {
			DrawTexturePro(objT, frameRock, objects[i].frame, (Vector2){0.0f,0.0f}, 0.0f, WHITE);
		} break;
		case GLACIER : {
			DrawTexturePro(objT, frameGlacier, objects[i].frame, (Vector2){0.0f,0.0f}, 0.0f, WHITE);
		} break;
	default : break;
	}
}
}

void UpdateEnemies(int i){




//health
if(enemies[i].hearts == 0)
	enemies[i].frameHeart1 = frameHeartEmpty;
if(enemies[i].hearts == 1)
	enemies[i].frameHeart2 = frameHeartEmpty;
if(enemies[i].hearts == 2)
	enemies[i].frameHeart3 = frameHeartEmpty;

enemies[i].heart1.x = enemies[i].rect.x-5;
enemies[i].heart1.y = enemies[i].rect.y-26;

enemies[i].heart2.x = enemies[i].rect.x+15;
enemies[i].heart2.y = enemies[i].rect.y-26;

enemies[i].heart3.x = enemies[i].rect.x+35;
enemies[i].heart3.y = enemies[i].rect.y-26;


//begin

float distance;
float min;
float distanceP;
int counter=0;
int target;
//Rectangle *Target=NULL;
	Rectangle *enemy = &(enemies[i].rect);
	enemies[i].hat.x=enemy->x;
	enemies[i].hat.y=enemy->y;
	enemies[i].speed = (MAX_SPEED-50)*deltaTime;

		//enemies bounds collision
		if(enemy->x > WORLD - enemy->width - frameBounds.width) enemy->x = WORLD - enemy->width - frameBounds.width;
		if(enemy->x < frameBounds.width) enemy->x = frameBounds.width;
		if(enemy->y > WORLD - enemy->height - frameBounds.height) enemy->y = WORLD - enemy->height - frameBounds.height;
		if(enemy->y < frameBounds.height) enemy->y = frameBounds.height;
		//enemys AI
		for(int j= 0; j<mode.allies; j++){
			if(allies[j].counted)
				counter++;
			else if(!allies[j].counted){
				target = j;
				j=mode.allies;
			}
		}
		Rectangle *Target=&(allies[target].rect);
		if(counter<mode.allies){
			min = sqrt((fabs((enemy->x+enemy->width/2)-(Target->x+Target->width/2))*fabs((enemy->x+enemy->width/2)-
			(Target->x+Target->width/2))) +
			(fabs((enemy->y+enemy->height/2)-(Target->y+Target->height/2))*fabs((enemy->y+enemy->height/2)-
			(Target->y+Target->height/2))));

			for (int n=0; n<mode.allies; n++) {
        		Rectangle *ally = &(allies[n].rect);

            distance = sqrt( ( fabs( (ally->x+ally->width/2)-(enemy->x+enemy->width/2) ) * fabs( (ally->x+ally->width/2)-(enemy->x+enemy->width/2) ) )+( fabs( (ally->y+ally->height/2)-(enemy->y+enemy->height/2) ) *  fabs( (ally->y+ally->height/2)-(enemy->y+enemy->height/2) ) ) );

				if(distance<min){
					min = distance;
					Target = &(allies[n].rect);
        		}
        	}
    		distanceP = sqrt((fabs((player.rect.x+player.rect.width/2)-(enemy->x+enemy->width/2)))*(fabs((player.rect.x+player.rect.width/2)-(enemy->x+enemy->width/2))) +(fabs((enemy->y+enemy->height/2)-(player.rect.y+player.rect.height/2)))*(fabs((enemy->y+enemy->height/2)-
(player.rect.y+player.rect.height/2))));

    	//if((min < distanceP)&&allies[target].hearts!=0)		 	Target = &(allies[target].rect);
		if(distanceP < min)	Target = &(player.rect);
		}
		else if(counter>=mode.allies)	Target = &(player.rect);

			if((enemy->x > Target->x + Target->width)&&
				((enemy->y < Target->y + Target->height)&&
				(enemy->y > Target->y - enemy->height))	)
				{
                    if(enemy->x > Target->x + Target->width + ZONE)
                        enemy->x -= enemies[i].speed;
				 	enemies[i].dir = 1;}
			else if((enemy->x < Target->x)&&
				((enemy->y < Target->y + Target->height)&&
				(enemy->y > Target->y -  enemy->height))	)
				{
                    if(enemy->x < Target->x - ZONE)
                        enemy->x += enemies[i].speed;
					enemies[i].dir = 5;}	

			else if((enemy->y + enemy->height < Target->y)&&
				((enemy->x < Target->x + Target->width)&&
				(enemy->x > Target->x - enemy->width))	)
				{	
                    if(enemy->y + enemy->height < Target->y - ZONE)
					    enemy->y += enemies[i].speed;
					enemies[i].dir = 3;}
			else if((enemy->y > Target->y + Target->height)&&
				((enemy->x < Target->x + Target->width)&&
				(enemy->x > Target->x - enemy->width))	)
				{
                    if(enemy->y > Target->y + Target->height + ZONE)
					    enemy->y -= enemies[i].speed;
					enemies[i].dir = 7;}
			
			else if((enemy->y > Target->y + Target->height)&&
					(enemy->x > Target->x + Target->width))
				{
                    if((enemy->y > Target->y + Target->height + ZONE)&&
					(enemy->x > Target->x + Target->width + ZONE)){
					    enemy->y -= enemies[i].speed;
					    enemy->x -= enemies[i].speed;}
					enemies[i].dir = 0;}
			else if((enemy->y > Target->y + Target->height)&&
					(enemy->x + enemy->width < Target->x))
				{
                    if((enemy->y > Target->y + Target->height + ZONE)&&
					(enemy->x + enemy->width < Target->x - ZONE)){
					    enemy->y -= enemies[i].speed;
					    enemy->x += enemies[i].speed;}
					enemies[i].dir = 6;}

			else if((enemy->y + enemy->height < Target->y)&&
					(enemy->x + enemy->width < Target->x))
				{
                    if((enemy->y + enemy->height < Target->y - ZONE)&&
					(enemy->x + enemy->width < Target->x - ZONE)){
					    enemy->y += enemies[i].speed;
					    enemy->x += enemies[i].speed;}
					enemies[i].dir = 4;}
			else if((enemy->y + enemy->height < Target->y)&&
					(enemy->x > Target->x + Target->width))
				{
                    if((enemy->y + enemy->height < Target->y - ZONE)&&
					(enemy->x > Target->x + Target->width + ZONE)){
					    enemy->y += enemies[i].speed;
					    enemy->x -= enemies[i].speed;}
					enemies[i].dir = 2;}



	//throwing snowballs

		SnowBall *ball = &(enemies[i].snowBall);
		ball->speed = SNOWBALL_SPEED*deltaTime;
		if(ball->flying==false){
			ball->position.x = enemy->x;
			ball->position.y = enemy->y+55;
			enemies[i].dir0 = enemies[i].dir;
			ball->dest.x = ball->position.x - ball->dest.width/2.0f;
			ball->dest.y = ball->position.y - ball->dest.height/2.0f;
			ball->counterTime +=deltaTime;
		}
if ((fabs(Target->x-enemy->x)<ZONE)||(fabs(Target->y-enemy->y)<ZONE)){
	ball->randomValue=GetRandomValue(1,2);
	if(ball->counterTime>ball->randomValue)
		ball->flying = true;
}

if (ball->flying == true){
    switch(enemies[i].dir0){
        case 0 : {	
			ball->position.x -= ball->speed*deltaTime;
			ball->position.y -= ball->speed*deltaTime;
		} break;
        case 1 : {	
			ball->position.x -= ball->speed*deltaTime;
		} break;
        case 2 : {	
			ball->position.x -= ball->speed*deltaTime;
			ball->position.y += ball->speed*deltaTime;
		} break;
        case 3 : {	
			ball->position.y += ball->speed*deltaTime;
		} break;
        case 4 : {	
			ball->position.x += ball->speed*deltaTime;
			ball->position.y += ball->speed*deltaTime;
		} break;
        case 5 : {	
			ball->position.x += ball->speed*deltaTime;
		} break;
        case 6 : {	
			ball->position.x += ball->speed*deltaTime;
			ball->position.y -= ball->speed*deltaTime;
		} break;
        case 7 : {	
			ball->position.y -= ball->speed*deltaTime;
		} break;
    default :break;
    }
	ball->dest.x = ball->position.x - ball->dest.width/2.0f;
	ball->dest.y = ball->position.y - ball->dest.height/2.0f;
}
//ball collision with objects
for(int i= 0; i<MAX_OBJS;i++){
    if(CheckCollisionCircleRec(ball->position, ball->radius, objects[i].rect)){
        ball->flying = false;
        ball->counterTime=0;
}}
//ball collision with allies
for(int j= 0; j<mode.allies;j++){
    if(CheckCollisionCircleRec(ball->position, ball->radius, allies[j].rect)&&(ball->flying == true)){
        ball->flying = false;
        allies[j].hearts --;
        ball->counterTime=0;
    }
}
//ball collision with player
    if(CheckCollisionCircleRec(ball->position, ball->radius, player.rect)&&(ball->flying == true)){
        ball->flying = false;
        player.hearts --;
        ball->counterTime=0;
    }
//ball speed over
if(fabs(ball->position.x - enemy->x)>ZONE*2||fabs(ball->position.y - enemy->y)>ZONE*2){
    ball->flying = false; 
    ball->counterTime=0;
}
if(((ball->position.x > WORLD)||(ball->position.x < frameBounds.width)||
    (ball->position.y > WORLD)||(ball->position.y < frameBounds.height))&&(ball->flying == true)){
	ball->flying = false;
    ball->counterTime=0;}
   

}
void DrawEnemies(int i){

		switch(enemies[i].dir){
			case 0 : {
                enemies[i].frameBot.x = (float)enemies[i].dir*(float)playerT.width/8;
				if(hatColor==Red){
					enemies[i].frameHat.x = 92;
                	enemies[i].frameHat.y = 9;
                }
				else {
                	enemies[i].frameHat.x = 109;
                	enemies[i].frameHat.y = 0;}
				} break;
			case 1 : {
                enemies[i].frameBot.x = (float)enemies[i].dir*(float)playerT.width/8;
				if(hatColor==Red){
                	enemies[i].frameHat.x = 92;
					enemies[i].frameHat.y = 9;
                }
				else {
                	enemies[i].frameHat.x = 75;
                	enemies[i].frameHat.y = 0;}
				} break;
			case 2 : {
                enemies[i].frameBot.x = (float)enemies[i].dir*(float)playerT.width/8;
				if(hatColor==Red){
					enemies[i].frameHat.x = 92;
                	enemies[i].frameHat.y = 0; 
                }
				else {
                	enemies[i].frameHat.x = 75;
                	enemies[i].frameHat.y = 0; }
				} break;
			case 3 : {
                enemies[i].frameBot.x = (float)enemies[i].dir*(float)playerT.width/8;
				if(hatColor==Red){
					enemies[i].frameHat.x = 109;
                	enemies[i].frameHat.y = 9; 
                }
				else {
                	enemies[i].frameHat.x = 109;
                	enemies[i].frameHat.y = 0;}
				} break;
			case 4 : {
                enemies[i].frameBot.x = (float)enemies[i].dir*(float)playerT.width/8;
				if(hatColor==Red){
                	enemies[i].frameHat.x = 92;
                	enemies[i].frameHat.y = 9;
                }
				else {
                	enemies[i].frameHat.x = 75;
                	enemies[i].frameHat.y = 9;}
				} break;
			case 5 : {
                enemies[i].frameBot.x = (float)enemies[i].dir*(float)playerT.width/8;
				if(hatColor==Red){
                	enemies[i].frameHat.x = 92;
                	enemies[i].frameHat.y = 9;
                } 
				else {
                	enemies[i].frameHat.x = 75;
                	enemies[i].frameHat.y = 9;}
				} break;
			case 6 : {
                enemies[i].frameBot.x = (float)enemies[i].dir*(float)playerT.width/8;
				if(hatColor==Red){
                	enemies[i].frameHat.x = 109;
                	enemies[i].frameHat.y = 9;
                }
				else {
                	enemies[i].frameHat.x = 109;
                	enemies[i].frameHat.y = 0;}
				} break;
			case 7 : {
                enemies[i].frameBot.x = (float)enemies[i].dir*(float)playerT.width/8;
				if(hatColor==Red){
                	enemies[i].frameHat.x = 109;
                	enemies[i].frameHat.y = 9;
                }
				else {
                	enemies[i].frameHat.x = 109;
                	enemies[i].frameHat.y = 0;}
				} break;
			default : break;
			}
		
		DrawTexturePro(playerT, enemies[i].frameBot, enemies[i].rect, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
        DrawTexturePro(objT, enemies[i].frameHat, enemies[i].hat, (Vector2){-6.0f, 10.5f}, 0.0f, WHITE);

		DrawTexturePro(objT, enemies[i].frameHeart1, enemies[i].heart1, (Vector2){0,0}, 0.0f, WHITE);
		DrawTexturePro(objT, enemies[i].frameHeart2, enemies[i].heart2, (Vector2){0,0}, 0.0f, WHITE);
		DrawTexturePro(objT, enemies[i].frameHeart3, enemies[i].heart3, (Vector2){0,0}, 0.0f, WHITE);
		if(enemies[i].snowBall.flying==true)
			DrawTexturePro(objT, frameBall, enemies[i].snowBall.dest,(Vector2){0,0}, 0.0f, WHITE);
			else if(enemies[i].snowBall.flying == false){
				if(enemies[i].snowBall.counterTime>=enemies[i].snowBall.randomValue)
					DrawTexturePro(objT, frameBall, enemies[i].snowBall.dest,(Vector2){0,0}, 0.0f, WHITE);
			}
}

void UpdateAllies(int i){
//health
if(allies[i].hearts == 0)
	allies[i].frameHeart1 = frameHeartEmpty;
if(allies[i].hearts == 1)
	allies[i].frameHeart2 = frameHeartEmpty;
if(allies[i].hearts == 2)
	allies[i].frameHeart3 = frameHeartEmpty;

allies[i].heart1.x = allies[i].rect.x-5;
allies[i].heart1.y = allies[i].rect.y-26;

allies[i].heart2.x = allies[i].rect.x+15;
allies[i].heart2.y = allies[i].rect.y-26;

allies[i].heart3.x = allies[i].rect.x+35;
allies[i].heart3.y = allies[i].rect.y-26;


//begin
int target;
float distance;
float min;
//Rectangle *Target=NULL;
int counter =0;
	Rectangle *ally = &(allies[i].rect);
	allies[i].hat.x=ally->x;
	allies[i].hat.y=ally->y;
	allies[i].speed = (MAX_SPEED-50)*deltaTime;
	//bounds collision
	if(ally->x > WORLD - ally->width - frameBounds.width) ally->x = WORLD - ally->width - frameBounds.width;
	if(ally->x < frameBounds.width) ally->x = frameBounds.width;
	if(ally->y > WORLD - ally->height - frameBounds.height) ally->y = WORLD - ally->height - frameBounds.height;
	if(ally->y < frameBounds.height) ally->y = frameBounds.height;
	
		//allys AI
		for(int j = 0; j<mode.enemies; j++){
			if(enemies[j].counted)
				counter++;			
			else if(!enemies[j].counted){
                target = j;
                j = mode.enemies;
            }
		}
	if(counter<mode.enemies){
		Rectangle *Target=&(enemies[target].rect);
		min = sqrt( ( fabs( (ally->x+ally->width/2)-(Target->x+Target->width/2) )*fabs( (ally->x+ally->width/2)-(Target->x+Target->width/2) ) ) +( fabs( (ally->y+ally->height/2)-(Target->y+Target->height/2) )*fabs( (ally->y+ally->height/2)-(Target->y+Target->height/2) ) ) );

			for (int n=0; n<mode.enemies; n++) {
        		Rectangle *enemy = &(enemies[n].rect);

            	distance = sqrt( ( fabs( (ally->x+ally->width/2)-(enemy->x+enemy->width/2) ) * fabs( (ally->x+ally->width/2)-(enemy->x+enemy->width/2) ) )+( fabs( (ally->y+ally->height/2)-(enemy->y+enemy->height/2) ) * fabs( (ally->y+ally->height/2)-(enemy->y+enemy->height/2) ) ) );
		        if(distance<min){
				    min=distance;
				    Target = &(enemies[n].rect);
			    }
            }
		
			if((ally->x > Target->x + Target->width)&&
				((ally->y < Target->y + Target->height)&&
				(ally->y > Target->y - ally->height))	)
				{
                    if(ally->x > Target->x + Target->width + ZONE)
                        ally->x -= allies[i].speed;
				 	allies[i].dir = 1;}
			else if((ally->x < Target->x)&&
				((ally->y < Target->y + Target->height)&&
				(ally->y > Target->y -  ally->height))	)
				{
                    if(ally->x < Target->x - ZONE)
                        ally->x += allies[i].speed;
					allies[i].dir = 5;}	

			else if((ally->y + ally->height < Target->y)&&
				((ally->x < Target->x + Target->width)&&
				(ally->x > Target->x - ally->width))	)
				{	
                    if(ally->y + ally->height < Target->y - ZONE)
					    ally->y += allies[i].speed;
					allies[i].dir = 3;}
			else if((ally->y > Target->y + Target->height)&&
				((ally->x < Target->x + Target->width)&&
				(ally->x > Target->x - ally->width))	)
				{
                    if(ally->y > Target->y + Target->height + ZONE)
					    ally->y -= allies[i].speed;
					allies[i].dir = 7;}
			
			else if((ally->y > Target->y + Target->height)&&
					(ally->x > Target->x + Target->width))
				{
                    if((ally->y > Target->y + Target->height + ZONE)&&
					(ally->x > Target->x + Target->width + ZONE)){
					    ally->y -= allies[i].speed;
					    ally->x -= allies[i].speed;}
					allies[i].dir = 0;}
			else if((ally->y > Target->y + Target->height)&&
					(ally->x + ally->width < Target->x))
				{
                    if((ally->y > Target->y + Target->height + ZONE)&&
					(ally->x + ally->width < Target->x - ZONE)){
					    ally->y -= allies[i].speed;
					    ally->x += allies[i].speed;}
					allies[i].dir = 6;}

			else if((ally->y + ally->height < Target->y)&&
					(ally->x + ally->width < Target->x))
				{
                    if((ally->y + ally->height < Target->y - ZONE)&&
					(ally->x + ally->width < Target->x - ZONE)){
					    ally->y += allies[i].speed;
					    ally->x += allies[i].speed;}
					allies[i].dir = 4;}
			else if((ally->y + ally->height < Target->y)&&
					(ally->x > Target->x + Target->width))
				{
                    if((ally->y + ally->height < Target->y - ZONE)&&
					(ally->x > Target->x + Target->width + ZONE)){
					    ally->y += allies[i].speed;
					    ally->x -= allies[i].speed;}
					allies[i].dir = 2;}

		

//throwing snowballs

		SnowBall *ball = &(allies[i].snowBall);
		ball->speed = SNOWBALL_SPEED*deltaTime;
		if(ball->flying==false){
			ball->position.x = ally->x;
			ball->position.y = ally->y+55;
			allies[i].dir0 = allies[i].dir;
			ball->dest.x = ball->position.x - ball->dest.width/2.0f;
			ball->dest.y = ball->position.y - ball->dest.height/2.0f;
			ball->counterTime +=deltaTime;
		}
if ((fabs(Target->x-ally->x)<ZONE)||(fabs(Target->y-ally->y)<ZONE)){
	ball->randomValue=GetRandomValue(1,2);
	if(ball->counterTime>ball->randomValue)
		ball->flying = true;
}
		
if (ball->flying == true){
    switch(allies[i].dir0){
        case 0 : {	
			ball->position.x -= ball->speed*deltaTime;
			ball->position.y -= ball->speed*deltaTime;
		} break;
        case 1 : {	
			ball->position.x -= ball->speed*deltaTime;
		} break;
        case 2 : {	
			ball->position.x -= ball->speed*deltaTime;
			ball->position.y += ball->speed*deltaTime;
		} break;
        case 3 : {	
			ball->position.y += ball->speed*deltaTime;
		} break;
        case 4 : {	
			ball->position.x += ball->speed*deltaTime;
			ball->position.y += ball->speed*deltaTime;
		} break;
        case 5 : {	
			ball->position.x += ball->speed*deltaTime;
		} break;
        case 6 : {	
			ball->position.x += ball->speed*deltaTime;
			ball->position.y -= ball->speed*deltaTime;
		} break;
        case 7 : {	
			ball->position.y -= ball->speed*deltaTime;
		} break;
    default :break;
    }
	ball->dest.x = ball->position.x - ball->dest.width/2.0f;
	ball->dest.y = ball->position.y - ball->dest.height/2.0f;
}
//ball collision with objects
for(int i= 0; i<MAX_OBJS;i++){
    if(CheckCollisionCircleRec(ball->position, ball->radius, objects[i].rect)){
        ball->flying = false;
        ball->counterTime = 0;
	}
}
//ball collision with enemies
for(int j= 0; j<mode.enemies;j++){
    if(CheckCollisionCircleRec(ball->position, ball->radius, enemies[j].rect)&&(ball->flying == true)){
        ball->flying = false;
        enemies[j].hearts --;
        ball->counterTime = 0;
    }
}

//ball speed over
if(fabs(ball->position.x - ally->x)>ZONE*2||fabs(ball->position.y - ally->y)>ZONE*2){
    ball->flying = false; 
    ball->counterTime=0;
}

if(((ball->position.x > WORLD)||(ball->position.x < frameBounds.width)||
    (ball->position.y > WORLD )||(ball->position.y < frameBounds.height))&&(ball->flying == true)){
		ball->flying = false;
    	ball->counterTime=0;
}
            
    }
}

void DrawAllies(int i){

		switch(allies[i].dir){
			case 0 : {
                allies[i].frameBot.x = (float)allies[i].dir*(float)playerT.width/8;
				if(hatColor==Gray){
					allies[i].frameHat.x = 92;
                	allies[i].frameHat.y = 9;
                }
				else {
                	allies[i].frameHat.x = 109;
                	allies[i].frameHat.y = 0;}
				} break;
			case 1 : {
                allies[i].frameBot.x = (float)allies[i].dir*(float)playerT.width/8;
				if(hatColor==Gray){
                	allies[i].frameHat.x = 92;
					allies[i].frameHat.y = 9;
                }
				else {
                	allies[i].frameHat.x = 75;
                	allies[i].frameHat.y = 0;}
				} break;
			case 2 : {
                allies[i].frameBot.x = (float)allies[i].dir*(float)playerT.width/8;
				if(hatColor==Gray){
					allies[i].frameHat.x = 92;
                	allies[i].frameHat.y = 0; 
                }
				else {
                	allies[i].frameHat.x = 75;
                	allies[i].frameHat.y = 0; }
				} break;
			case 3 : {
                allies[i].frameBot.x = (float)allies[i].dir*(float)playerT.width/8;
				if(hatColor==Gray){
					allies[i].frameHat.x = 109;
                	allies[i].frameHat.y = 9; 
                }
				else {
                	allies[i].frameHat.x = 109;
                	allies[i].frameHat.y = 0;}
				} break;
			case 4 : {
                allies[i].frameBot.x = (float)allies[i].dir*(float)playerT.width/8;
				if(hatColor==Gray){
                	allies[i].frameHat.x = 92;
                	allies[i].frameHat.y = 9;
                }
				else {
                	allies[i].frameHat.x = 75;
                	allies[i].frameHat.y = 9;}
				} break;
			case 5 : {
                allies[i].frameBot.x = (float)allies[i].dir*(float)playerT.width/8;
				if(hatColor==Gray){
                	allies[i].frameHat.x = 92;
                	allies[i].frameHat.y = 9;
                } 
				else {
                	allies[i].frameHat.x = 75;
                	allies[i].frameHat.y = 9;}
				} break;
			case 6 : {
                allies[i].frameBot.x = (float)allies[i].dir*(float)playerT.width/8;
				if(hatColor==Gray){
                	allies[i].frameHat.x = 109;
                	allies[i].frameHat.y = 9;
                }
				else {
                	allies[i].frameHat.x = 109;
                	allies[i].frameHat.y = 0;}
				} break;
			case 7 : {
                allies[i].frameBot.x = (float)allies[i].dir*(float)playerT.width/8;
				if(hatColor==Gray){
                	allies[i].frameHat.x = 109;
                	allies[i].frameHat.y = 9;
                }
				else {
                	allies[i].frameHat.x = 109;
                	allies[i].frameHat.y = 0;}
				} break;
			default : break;
			}
		
		DrawTexturePro(playerT, allies[i].frameBot, allies[i].rect, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
        DrawTexturePro(objT, allies[i].frameHat, allies[i].hat, (Vector2){-6.0f, 10.5f}, 0.0f, WHITE);

		DrawTexturePro(objT, allies[i].frameHeart1, allies[i].heart1, (Vector2){0,0}, 0.0f, WHITE);
		DrawTexturePro(objT, allies[i].frameHeart2, allies[i].heart2, (Vector2){0,0}, 0.0f, WHITE);
		DrawTexturePro(objT, allies[i].frameHeart3, allies[i].heart3, (Vector2){0,0}, 0.0f, WHITE);
		if(allies[i].snowBall.flying==true)
			DrawTexturePro(objT, frameBallA, allies[i].snowBall.dest,(Vector2){0,0}, 0.0f, WHITE);
			else if(allies[i].snowBall.flying == false){
				if(allies[i].snowBall.counterTime>=allies[i].snowBall.randomValue)
					DrawTexturePro(objT, frameBallA, allies[i].snowBall.dest,(Vector2){0,0}, 0.0f, WHITE);
			}
}

void DrawMap(void){
int counterW = 0;
int counterH = 0;

	for(int i= 0; counterW<=WORLD; i++){
		if(i==counterW){ 
			counterH=0;
			for(int j= 0; counterH<=WORLD; j++){
				if(j==counterH){
					if((counterW==0)||(counterH==0)||(counterW>=WORLD-frameBounds.width)||(counterH>=WORLD-frameBounds.height))
						DrawTextureRec(objT, frameBounds, (Vector2){(float)counterW,(float)counterH}, WHITE);
					else
						DrawTextureRec(objT, frameTile, (Vector2){(float)counterW,(float)counterH}, WHITE);
				    counterH+=frameTile.height;
                }
			}
			counterW+=frameTile.width;
		}
	}
}

    

void UpdateCameraCentre(void){
    Vector2 posC = GetMousePosition();  

	float zoom = GetMouseWheelMove()*0.5;
	if(IsKeyPressed(KEY_C)){
		if (state)	state=false;
		else state = true;		
	}
	if(!state){
	if ((posC.x>(screenWidth*(BOUNDS-1)/BOUNDS))&&(posC.y>(screenHeight/BOUNDS))&&(posC.y<(screenHeight*(BOUNDS-1)/BOUNDS)))
			camera.target.x += CAMERA_SPEED*deltaTime;
	else if ((posC.x<(screenWidth/BOUNDS))&&(posC.y>(screenHeight/BOUNDS))&&(posC.y<(screenHeight*(BOUNDS-1)/BOUNDS)))
			camera.target.x -= CAMERA_SPEED*deltaTime;
	if ((posC.y>(screenHeight*(BOUNDS-1)/BOUNDS))&&(posC.x>(screenWidth/BOUNDS))&&(posC.x<(screenWidth*(BOUNDS-1)/BOUNDS)))
			camera.target.y += CAMERA_SPEED*deltaTime;
	else if ((posC.y<(screenHeight/BOUNDS))&&(posC.x>(screenWidth/BOUNDS))&&(posC.x<(screenWidth*(BOUNDS-1)/BOUNDS)))
			camera.target.y -= CAMERA_SPEED*deltaTime;
	if((posC.x>(screenWidth*(BOUNDS-1)/BOUNDS))&&(posC.y>screenHeight*(BOUNDS-1)/BOUNDS)){
			camera.target.x += CAMERA_SPEED*deltaTime;
			camera.target.y += CAMERA_SPEED*deltaTime;}
	else if((posC.x<(screenWidth/BOUNDS))&&(posC.y<screenHeight/BOUNDS)){
			camera.target.x -= CAMERA_SPEED*deltaTime;
			camera.target.y -= CAMERA_SPEED*deltaTime;}
	if((posC.x>(screenWidth*(BOUNDS-1)/BOUNDS))&&(posC.y<screenHeight/BOUNDS)){
			camera.target.x += CAMERA_SPEED*deltaTime;
			camera.target.y -= CAMERA_SPEED*deltaTime;}
	else if((posC.x<(screenWidth/BOUNDS))&&(posC.y>screenHeight*(BOUNDS-1)/BOUNDS)){
			camera.target.x -= CAMERA_SPEED*deltaTime;
			camera.target.y += CAMERA_SPEED*deltaTime;}
	if(camera.target.x > WORLD)	camera.target.x = WORLD;
	if(camera.target.y > WORLD)	camera.target.y = WORLD;
	if(camera.target.x < 0)	camera.target.x = 0;
	if(camera.target.y < 0)	camera.target.y = 0;

	if(IsKeyDown(KEY_LEFT_CONTROL)) camera.target = (Vector2){player.rect.x + (player.rect.width/2), player.rect.y + (player.rect.height/2)};
	}
	else if(state)	{
		camera.offset = (Vector2){screenWidth/2, screenHeight/2};	
		camera.target = (Vector2){player.rect.x + (player.rect.width/2), player.rect.y + (player.rect.height/2)};
	}
	camera.zoom += zoom;
	if(camera.zoom > 2)
		camera.zoom = 2;
	else if (camera.zoom < 0.8)
		camera.zoom = 0.8;
	if(IsKeyDown(KEY_R)) camera.zoom = 1.0f;
}


void Collision(Rectangle *rect, Rectangle *ob, float bounds){
            
            if( (rect->y + rect->height > ob->y)&&
		        (rect->y < ob->y + ob->height)&&
		        (rect->x + rect->width > ob->x)&&
		        (rect->x + rect->width < ob->x + (ob->width*bounds))	){
		            rect->x = ob->x - rect->width;	
                }
	        if( (rect->y + rect->height > ob->y)&&
		        (rect->y < ob->y + ob->height)&&
		        (rect->x < ob->x + ob->width)&&
		        (rect->x > ob->x + (ob->width*(1-bounds)))	){
			        rect->x = ob->x + ob->width;
	            }
	        if( (rect->x + rect->width > ob->x )&&
		        (rect->x < ob->x + ob->width)&&
		        (rect->y + rect->height > ob->y)&&
		        (rect->y + rect->height < ob->y + (ob->height*bounds))	){
			        rect->y = ob->y - rect->height;
	            }
	        if( (rect->x + rect->width > ob->x )&&
		        (rect->x < ob->x + ob->width)&&
		        (rect->y < ob->y + ob->height)&&
		        (rect->y > ob->y + (ob->height*(1-bounds)))	){
			        rect->y = ob->y + ob->height;
	            }   
}
