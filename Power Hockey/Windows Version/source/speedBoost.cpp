#define STATE_MANAGER_CPP
#include <windows.h>											// Header File For Windows
#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <math.h>												// Header File For Math Operations
#include <gl\gl.h>											// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "glut.h"
#include <assert.h>
#include "box2d/box2d.h"
#include "baseTypes.h"
#include "openglframework.h"	
#include "gamedefs.h"
#include "collInfo.h"
#include "object.h"
#include "spriteDemoManager.h"
#include "random.h"
#include "soil.h"
#include "ball.h"
#include "BallManager.h"
#include "PowerUpManager.h"
#include "speedBoost.h"


SpeedBoost::SpeedBoost(char spritePage[100], int width, int height, int numCol, int numRow, int initialCol, int initialRow) {
	powerUpPositions.x = getRangedRandom(-300.0f, 300.0f);
	powerUpPositions.y = getRangedRandom(-200.0f, 200.0f);
	sprite = new SpriteDemoManagerC(spritePage, width, height, powerUpPositions, numCol, numRow, initialRow, initialCol);
	powerHeight = height;
	powerWidth = width;
	isSpawned = true;
	currentUpdateTime = 0;
}

SpeedBoost::SpeedBoost() {

}

SpeedBoost::~SpeedBoost() {

}
void SpeedBoost::renderSprite() {
	if (isSpawned == true) {
		sprite->renderSprites(powerUpPositions);
	}
}

void SpeedBoost::updateSprite(DWORD milliseconds, SpeedBoost *speed) {
	if (isSpawned == true) {
		sprite->updateSprites(milliseconds);
		BallC* ball = BallManagerC::GetInstance()->getBallPtr(0);
		b2Vec2 pos = ball->ballBody->GetPosition();
		float rad = ball->ballCircle.m_radius;

		if ((pos.x * 100 + rad * 100>= (powerUpPositions.x - powerWidth / 2) * ( 4000.0f / 1024.0f)) &&
			(pos.x * 100 - rad * 100<= (powerUpPositions.x + powerWidth / 2) * (4000.0f / 1024.0f)) &&
			(pos.y * 100 + rad * 100 >= (powerUpPositions.y - powerHeight / 2) * (4000.0f / 768.0f)) &&
			(pos.y * 100 - rad * 100 <= (powerUpPositions.y + powerHeight / 2) * (4000.0f / 768.0f))) {
			b2Vec2 vel = ball->ballBody->GetLinearVelocity();
			vel.x *= 2;
			vel.y *= 2;
			ball->ballBody->ApplyLinearImpulse(vel, ball->ballBody->GetWorldCenter(), true);
			isSpawned = false;
		}
	}
	else {
		newSpawnLocation(speed);
		respawnTimer(milliseconds);
	}
}

void SpeedBoost::respawnTimer(DWORD milliseconds) {
	
	currentUpdateTime += milliseconds;
	if (currentUpdateTime - lastUpdateTime > RESPAWN_TIME) {
		lastUpdateTime = currentUpdateTime;
		isSpawned = true;
	}
}
void SpeedBoost::newSpawnLocation(SpeedBoost *speed) {
	bool canSpawn = false;
	
	while (canSpawn == false) {
		Coord2D *posOther = speed->getPosition();
		int* widthOther = speed->getWidth();
		int* heightOther = speed->getHeight();
		powerUpPositions.x = getRangedRandom(-300.0f, 300.0f);
		powerUpPositions.y = getRangedRandom(-200.0f, 200.0f);
		if ((powerUpPositions.x + powerWidth / 2 >=  posOther->x - *widthOther/2) &&
			(powerUpPositions.x - powerWidth / 2 <= posOther->x + *widthOther / 2) &&
			(powerUpPositions.y + powerHeight / 2 >= posOther->y - *heightOther / 2) &&
			(powerUpPositions.y - powerHeight / 2 <= posOther->y + *heightOther / 2)) {
		}
		else {
			canSpawn = true;
			break;
		}
	}
}
