#define GAME_CPP
#include <windows.h>											// Header File For Windows
#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <math.h>												// Header File For Math Operations
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "glut.h"
#include "box2d/box2d.h"
#include "baseTypes.h"
#include "openglframework.h"	
#include "gamedefs.h"
#include "collInfo.h"
#include "object.h"
#include "ball.h"
#include "field.h"
#include "random.h"
#include "gameObjects.h"
#include "openGLStuff.h"
#include "game.h"
#include "newDebugDraw.h"
#include "StateManager.h"
#include "BallManager.h"
#include "FieldManager.h"
#include "InputManager.h"
#include "SpriteDemoManager.h"
#include "PowerUpManager.h"
#include "speedBoost.h"
#include "Player.h"

// Declarations
const char8_t CGame::mGameTitle[]="Framework1";
CGame* CGame::sInstance=NULL;

b2Vec2 gravity(0.0f, 0.0f);
b2World world(gravity);

Player* player1;
Player* player2;

SpeedBoost* speedBoost1;
SpeedBoost* speedBoost2;


float_t timeStep = 1.0f / 60.0f;
int32_t velocityIterations = 6;
int32_t positionIterations = 2;


SpriteDemoManagerC* titleCard;

BOOL Initialize (GL_Window* window, Keys* keys)					// Any OpenGL Initialization Goes Here
{
	initOpenGLDrawing(window,keys,0.0f, 0.0f, 0.0f);
	CGame::CreateInstance();
	
	uint32_t flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_centerOfMassBit;
	
	CGame::GetInstance()->init();
	return TRUE;						
}

void CGame::init()
{
	BallManagerC::CreateInstance();
	StateManagerC::CreateInstance();
	FieldManagerC::CreateInstance();
	InputManagerC::CreateInstance();

	InputManagerC::GetInstance()->init();

	BallManagerC::GetInstance()->init(&world);
	StateManagerC::GetInstance()->setState(StateManagerC::HALF_BALLS_FILLED);
	FieldManagerC::GetInstance()->init(&world);

	//						sprite		       xPos,yPos, r,   g,    b,  id, width, height, struct Keys ASCII, b2word
	player1 = new Player("player2_paddle.png", -500, 10, 0.0f, 0.5f, 1.0f, 1, 243, 600, {65, 68, 87, 83, 32}, &world);
	player2 = new Player("player1_paddle.png", 500 , 100 , 1.0f, 0.5f, 0.0f, 2, 243, 600, { 100, 102, 104, 101, 96 }, &world);

	//                            sprite          width, height, max col, max row, init col, init row
	speedBoost1 = new SpeedBoost("power_ups.png", 60, 60, 5, 2, 0, 0);
	speedBoost2 = new SpeedBoost("power_ups.png", 60, 60, 5, 2, 0, 0);

	Coord2D spawnPos = { 0,0 };
	//                                     sprite       width, height, pos, max col, max row, init col, init row
	titleCard = new SpriteDemoManagerC("TitleCard.png", 1020, 768, spawnPos, 3, 2, 0, 1);
	
}

void CGame::UpdateFrame(DWORD milliseconds)			
{
	keyProcess();
	if (onStartScreen)
	{
		if (g_keys->keyDown[32] > 0)
		{
			onStartScreen = false;
		}
	}

	else if (!onStartScreen && !BallManagerC::GetInstance()->p1Wins && !BallManagerC::GetInstance()->p2Wins)
	{
		player1->update(milliseconds);
		player2->update(milliseconds);
		speedBoost1->updateSprite(milliseconds, speedBoost2);
		speedBoost2->updateSprite(milliseconds, speedBoost1);
		BallManagerC::GetInstance()->updateBalls(milliseconds);
	}

	else if (BallManagerC::GetInstance()->p1Wins)
	{
		shutdown();
		init();
		onStartScreen = true;
		p1Win = false;
	}

	else if (BallManagerC::GetInstance()->p2Wins)
	{
		shutdown();
		init();
		onStartScreen = true;
		p2Win = false;
	}
	
	world.Step(timeStep, velocityIterations, positionIterations);
	world.ClearForces();
}

void CGame::DrawScene(void)											
{
	startOpenGLDrawing();
	if (onStartScreen)
	{
		drawStartScreen();
	}
	else if (!onStartScreen && !p1Win && !p2Win)
	{
		FieldManagerC::GetInstance()->renderField();
		player1->render();
		player2->render();
		speedBoost1->renderSprite();
		speedBoost2->renderSprite();
		BallManagerC::GetInstance()->renderBalls();
	}

	else if (BallManagerC::GetInstance()->p1Wins)
	{
	}

	else if (BallManagerC::GetInstance()->p2Wins)
	{
	}
	
}


CGame *CGame::CreateInstance()
{
	sInstance = new CGame();
	return sInstance;
}
void CGame::shutdown()
{
	delete player1;
	delete player2;
	delete speedBoost1;
	delete speedBoost2;
	BallManagerC::GetInstance()->shutdown();
	StateManagerC::GetInstance()->shutdown();
	FieldManagerC::GetInstance()->shutdown();
	
}
void CGame::DestroyGame(void)
{
	delete BallManagerC::GetInstance();	
	delete StateManagerC::GetInstance();	
	delete FieldManagerC::GetInstance();	
	
}

void CGame::drawStartScreen()
{
	Coord2D pos = { 0,0 };
	titleCard->renderSprites(pos);
}

