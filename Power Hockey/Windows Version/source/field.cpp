#include <windows.h>											// Header File For Windows
#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <math.h>												// Header File For Math Operations
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
//#include "box2d/b2_world.h"
#include "box2d/box2d.h"
#include "glut.h"
#include "baseTypes.h"
#include "collInfo.h"
#include "object.h"
#include "ShapeDraw.h"
#include "spriteDemoManager.h"
#include "field.h"


void FieldC::moveUp()
{
	int y;
	y=5;
}
FieldC::FieldC(float_t x, float_t y, float_t height, float_t width, uint32_t initColor, b2World *world)
{
	mEnabled = true;
	mFieldColor = initColor;
	mWidth = width;
	mHeight = height;
	mPosition.x = x;
	mPosition.y = y;
	background = new SpriteDemoManagerC("board.png", 1024, 768, { 0.0f, 0.0f }, 1, 1, 0, 0);

	fieldBodyDef.type = b2_staticBody;
	fieldBodyDef.position.Set(x * scalingFactor, y * scalingFactor);
	fieldBody = world->CreateBody(&fieldBodyDef);
	
	//Define fixture
	fieldBox.SetAsBox((width * scalingFactor) / 2, (height * scalingFactor) / 2);
	
	fieldFixtureDef.shape = &fieldBox;
	fieldFixtureDef.density = 1.0f;
	fieldFixtureDef.friction = 0.0f;
	
	fieldBody->CreateFixture(&fieldFixtureDef);

	
};
FieldC::~FieldC()
{
};
void FieldC::update(DWORD milliseconds)
{
};

void FieldC::render()
{
	background->renderSprites({ 0.0f, 0.0f });
};
