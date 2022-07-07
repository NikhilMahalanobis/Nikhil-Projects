#include <windows.h>											// Header File For Windows
#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <math.h>												// Header File For Math Operations
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "glut.h"
#include "baseTypes.h"
#include "ShapeDraw.h"
#include "collInfo.h"
#include "object.h"
//#include "inputmapper.h"
#include "box2d/box2d.h"
#include "field.h"
#include "fieldmanager.h"
#include "gamedefs.h"
#include "gameobjects.h"
#include "random.h"
#include "stateManager.h"
#include "game.h"
#include "inputmanager.h"
#include "SpriteDemoManager.h"
#include "ball.h"

void BallC::moveUp()
{
	int x;
	x=5;
}
BallC::BallC(float initPosX,float initPosY,float initVelX,float initVelY, float initAccelX, float initAccelY, float radius, int id, b2World* world)
{

	mPosition.x = initPosX;
	mPosition.y = initPosY;
	mVelocity.x = initVelX;
	mVelocity.y = initVelY;
	mAcceleration.x = initAccelX;
	mAcceleration.y = initAccelY;
	mRadius = radius;

	ballBodyDef.type = b2_dynamicBody;
	ballBodyDef.position.Set(initPosX*scalingFactor, initPosY*scalingFactor);
	ballBodyDef.bullet = true;
	ballBody = world->CreateBody(&ballBodyDef);
	ballBody->SetBullet(true);

	ballCircle.m_p.Set(initPosX*scalingFactor, initPosY*scalingFactor);
	ballCircle.m_radius = radius*scalingFactor;

	ballFixtureDef.shape = &ballCircle;
	ballFixtureDef.density = 1.0f;
	ballFixtureDef.friction = 0.05f;
	ballFixtureDef.restitution = 0.7f;
	ballFixtureDef.filter.groupIndex = -1;

	ballBody->CreateFixture(&ballFixtureDef);
	ballBody->SetLinearVelocity(b2Vec2(1,0));

	sprite = new SpriteDemoManagerC("puck.png", 66, 62, mPosition, 1, 1, 0, 0);
	player1Score = new SpriteDemoManagerC("Numbers.png", 54, 49, { 50, 300 }, 10, 2, 1, 0);
	player2Score = new SpriteDemoManagerC("Numbers.png", 54, 49, { -50, 300 }, 10, 2, 0, 0);

}
BallC::BallC()
{
    mEnabled = true;
    mCollInfo.shape = CollInfoC::SHAPE_CIRCLE;

};
BallC::~BallC()
{
};

void BallC::update(DWORD milliseconds)
{
	b2Vec2 ballPos = ballBody->GetPosition();
	mPosition.x = ballPos.x / scalingFactor;
	mPosition.y = ballPos.y / scalingFactor;

	sprite->updateSprites(milliseconds);

	DoCollisions(milliseconds);

	lastUpdateTime = milliseconds;
	if(renderObject == false){
		currentUpdateTime += milliseconds;
		if (currentUpdateTime - lastUpdateTime > 1000) {
			ballBody->SetAwake(true);
			renderObject = true;
			currentUpdateTime = 0;
			if (playerScored == true) {
				ballBody->SetTransform(b2Vec2(0, 0), 0.0f);
				ballBody->SetLinearVelocity(b2Vec2(1, 0));
				int* col = player1Score->getStartCol();
				player1Score->setStartCol(*col + 1);
				p1Score++;
			}
			if (playerScored == false) {
				ballBody->SetTransform(b2Vec2(0, 0), 0.0f);
				ballBody->SetLinearVelocity(b2Vec2(-1, 0));
				int* col = player2Score->getStartCol();
				player2Score->setStartCol(*col + 1);
				p2Score++;
			}
		}
	}
}
void BallC::DoCollisions(DWORD milliseconds)
{
	CollideField(milliseconds);
}
void BallC::CollideField(DWORD milliseconds)
{
	int count = 0;
	FieldC *field = FieldManagerC::GetInstance()->getFieldPtr(7);

	float_t rightSide = field->getPosition()->x+((float_t)field->getWidth()/2.0f);
	float_t leftSide = field->getPosition()->x-((float_t)field->getWidth()/2.0f);
	float_t topSide = field->getPosition()->y+((float_t)field->getHeight()/2.0f);
	float_t bottomSide = field->getPosition()->y-((float_t)field->getHeight()/2.0f);
	
	b2Vec2 ballPos = ballBody->GetPosition();

	if (((ballPos.x / scalingFactor + ballCircle.m_radius / scalingFactor) >= (leftSide - 100.0f)) &&
		((ballPos.x / scalingFactor - ballCircle.m_radius / scalingFactor) <= (rightSide + 100.0f)) &&
		((ballPos.y / scalingFactor + ballCircle.m_radius / scalingFactor) >= bottomSide) &&
		((ballPos.y / scalingFactor - ballCircle.m_radius / scalingFactor) <= topSide)) {
		 count++;
		 renderObject = false;
		 playerScored = true;
		 ballBody->SetAwake(false);
	}

	field = FieldManagerC::GetInstance()->getFieldPtr(4);

	rightSide = field->getPosition()->x + ((float_t)field->getWidth() / 2.0f);
	leftSide = field->getPosition()->x - ((float_t)field->getWidth() / 2.0f);
	topSide = field->getPosition()->y + ((float_t)field->getHeight() / 2.0f);
	bottomSide = field->getPosition()->y - ((float_t)field->getHeight() / 2.0f);

	ballPos = ballBody->GetPosition();

	if (((ballPos.x / scalingFactor + ballCircle.m_radius / scalingFactor) >= (leftSide - 100.0f)) &&
		((ballPos.x / scalingFactor - ballCircle.m_radius / scalingFactor) <= (rightSide + 100.0f)) &&
		((ballPos.y / scalingFactor + ballCircle.m_radius / scalingFactor) >= bottomSide) &&
		((ballPos.y / scalingFactor - ballCircle.m_radius / scalingFactor) <= topSide)) {
		count++;
		renderObject = false; 
		playerScored = false;
		ballBody->SetAwake(false);
	}

}

void BallC::render()
{
	if (renderObject == true) {
		sprite->renderSprites({ mPosition.x * (1024.0f / 4000.0f), mPosition.y * (768.0f / 4000.f) });
	}
	player1Score->renderSprites({ -50, 300 });
	player2Score->renderSprites({ 50, 300 });
}
void BallC::setRandomColor()
{
	mBallColor = getRangedRandom(0,256);
	mBallColor += getRangedRandom(0,256) << 8;
	mBallColor += getRangedRandom(0,256) << 16;
}



