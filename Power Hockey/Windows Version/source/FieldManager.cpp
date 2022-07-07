#define FIELD_MANAGER_CPP
#include <windows.h>											// Header File For Windows
#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <math.h>												// Header File For Math Operations
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include "box2d/box2d.h"
#include "glut.h"
#include "baseTypes.h"
#include "openglframework.h"	
#include "gamedefs.h"
#include "collinfo.h"
#include "object.h"
#include "SpriteDemoManager.h"
#include "field.h"
#include "FieldManager.h"

FieldManagerC* FieldManagerC::sInstance=NULL;


FieldManagerC *FieldManagerC::CreateInstance()
{
	if(sInstance!=NULL)return sInstance;
	else

	sInstance = new FieldManagerC();
	return sInstance;
}
void FieldManagerC::init(b2World *world)
{
	mFieldPtr = (FieldC**)malloc(NUM_FIELDS * sizeof(FieldC*));
	//bottom
	mFieldPtr[0] = new FieldC(0.0f, -1650.0f, 100, 4000, 0x00FF0000, world);
	//top 
	mFieldPtr[1] = new FieldC(0.0f, 1650.0f, 100, 4000, 0x00FF0000, world);
	//left lowe
	mFieldPtr[2] = new FieldC(-1700.0f, -1075.0f, 1050, 100, 0x00FF0000, world);
	//left high
	mFieldPtr[3] = new FieldC(-1700.0f, 1075.0f, 1050, 100, 0x00FF0000, world);
	//left goal
	mFieldPtr[4] = new FieldC(-1900.0f, 0.0f, 1100, 50.0f, 0x00FF0000, world);
	//right lower
	mFieldPtr[5] = new FieldC(1700.0f, -1075.0f, 1050, 100, 0x00FF0000, world);
	//right high
	mFieldPtr[6] = new FieldC(1700.0f, 1075.0f, 1050, 100, 0x00FF0000, world);
	//right goal
	mFieldPtr[7] = new FieldC(1900.0f, 0.0f, 1100, 50.0f, 0x00FF0000, world);

	halfBodyDef.type = b2_staticBody;
	halfBodyDef.position.Set(0, 0);
	halfBody = world->CreateBody(&halfBodyDef);

	halfShape.SetAsBox((10 * 0.01) / 2, (4000 * 0.01) / 2);
	
	halfFixtureDef.shape = &halfShape;
	halfFixtureDef.density = 1.0f;
	halfFixtureDef.friction = 0.0f;
	halfFixtureDef.filter.groupIndex = -1;

	halfBody->CreateFixture(&halfFixtureDef);
}
FieldC *FieldManagerC::getFieldPtr(int32_t index)
{
	return mFieldPtr[index];
}
void FieldManagerC::renderField()
{
	for (uint32_t i = 0; i < NUM_FIELDS; ++i) {
		mFieldPtr[i]->render();
	}
}
void FieldManagerC::shutdown()
{
	for (uint32_t i = 0; i < NUM_FIELDS; ++i) {
		delete mFieldPtr[i];
	}
	free(mFieldPtr);
}