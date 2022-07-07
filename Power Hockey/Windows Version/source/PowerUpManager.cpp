#define STATE_MANAGER_CPP
#include <windows.h>											// Header File For Windows
#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <math.h>												// Header File For Math Operations
#include <gl\gl.h>											// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "glut.h"
#include <assert.h>
#include "baseTypes.h"
#include "openglframework.h"	
#include "gamedefs.h"
#include "spriteDemoManager.h"
#include "random.h"
#include "soil.h"
#include "PowerUpManager.h"


PowerUpManager::PowerUpManager(char spritePage[100],int width, int height, int numCol, int numRow, int initialCol, int initialRow) {
	currentPos.x = getRangedRandom(-500.0f, 500.0f);
	currentPos.y = getRangedRandom(-500.0f, 500.0f);
	sprite = new SpriteDemoManagerC(spritePage, width, height, currentPos, numCol, numRow, initialRow, initialCol);
	
}


PowerUpManager::PowerUpManager() {
	
}
PowerUpManager::~PowerUpManager() {
	delete sprite;
}

void PowerUpManager::renderSprite() {
	sprite->renderSprites(currentPos);
}

void PowerUpManager::updateSprite(DWORD milliseconds) {
	sprite->updateSprites(milliseconds);
}