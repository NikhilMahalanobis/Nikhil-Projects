#define BALL_MANAGER_CPP
#include <assert.h>
#include <windows.h>											// Header File For Windows
#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>	// Header File For Variable Argument Routines
#include <math.h>	// Header File For Math Operations
#include <gl\gl.h>	// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "glut.h"
#include "box2d/box2d.h"
#include "baseTypes.h"
#include "openglframework.h"	
#include "gamedefs.h"
#include "collinfo.h"
#include "object.h"
#include "Ball.h"
#include "BallManager.h"
#include "random.h"


BallManagerC* BallManagerC::sInstance=NULL;


BallManagerC *BallManagerC::CreateInstance()
{
	//assert(sInstance==NULL);
	sInstance = new BallManagerC();
	return sInstance;
}

void BallManagerC::init(b2World* world)
{
	printf("Hey initing the ball manager!\n");
	ballPtrs = (BallC**)malloc(NUM_BALLS*sizeof(BallC*));
	for(uint32_t i=0;i<NUM_BALLS;++i)
    {
		ballPtrs[i] = new BallC(0.0f, 0.0f, 0, 0,0,0, 50, i, world);
    }
}
void BallManagerC::updateBalls(DWORD milliseconds)
{
    for(uint32_t i=0;i<NUM_BALLS;++i)
    {
       ballPtrs[i]->update(milliseconds);
    }


	if (ballPtrs[0]->getP1Score() == winScore)
	{
		p1Wins = true;
	}

	else if (ballPtrs[0]->getP2Score() == winScore)
	{
		p2Wins = true;
	}
}

BallC *BallManagerC::getBallPtr(int32_t index)
{
	return ballPtrs[index];
}
void BallManagerC::renderBalls()
{
    for(uint32_t i=0;i<NUM_BALLS;++i)
    {
       ballPtrs[i]->render();
    }
}
void BallManagerC::shutdown()
{
	for(uint32_t i=0;i<NUM_BALLS;++i)
    {
		delete ballPtrs[i];
	}
	free(ballPtrs);
}