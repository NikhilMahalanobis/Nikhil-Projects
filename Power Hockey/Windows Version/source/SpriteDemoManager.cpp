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


SpriteDemoManagerC::SpriteDemoManagerC(char spriteSheet[100], int width, int height, static Coord2D spawnPosition, int numSpritesCol, int numSpritesRow, int startRow, int startCol)
{

	spriteWidth = width;
	spriteHeight = height;
	screenPosition = spawnPosition;
	currentSpriteRow = startRow;
	currentSpriteCol = startCol;

	maxSpriteRows = numSpritesRow;
	maxSpriteCols = numSpritesCol;


/* load an image file directly as a new OpenGL texture */
	mSpriteTextureMap = SOIL_load_OGL_texture(spriteSheet, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
}

void SpriteDemoManagerC::renderSprites(Coord2D objectPosition)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, mSpriteTextureMap);
#if 1
	glBegin (GL_QUADS);
	
	GLfloat xPositionLeft = (objectPosition.x - spriteWidth/2)*4000.0f/1024.0f;
	GLfloat xPositionRight = (objectPosition.x + spriteWidth / 2)*4000.0f/1024.0f;
	GLfloat yPositionBottom = (objectPosition.y - spriteHeight / 2) * 4000.0f/768.0f;
	GLfloat yPositionTop = (objectPosition.y + spriteHeight / 2) * 4000.0f/768.0f;

	GLfloat xTextureCoord = currentSpriteCol*(1.0f/maxSpriteCols);
	GLfloat yTextureCoord = currentSpriteRow*(1.0f/maxSpriteRows);


	glColor4ub(0xFF, 0xFF, 0xFF,0xFF);
	//bottom left
	glTexCoord2f(xTextureCoord, yTextureCoord);
	glVertex3f(xPositionLeft, yPositionBottom, 0.0);
	//bottom right
	glTexCoord2f(xTextureCoord + (1.0f / maxSpriteCols), yTextureCoord);
	glVertex3f(xPositionRight, yPositionBottom, 0.0);
	//top right
	glTexCoord2f(xTextureCoord + (1.0 / maxSpriteCols), yTextureCoord + (1.0 / maxSpriteRows));
	glVertex3f(xPositionRight, yPositionTop, 0.0);
	//top left
	glTexCoord2f(xTextureCoord, yTextureCoord + (1.0 / maxSpriteRows));
	glVertex3f(xPositionLeft, yPositionTop, 0.0);
	
#else
	VertexFormatPos3Tex2 *vert = mVertexArray;

	for(int y=0; y < mHeight; ++y)
		{
			for(int x=0; x < mWidth; ++x)
			{	
				GLfloat xPositionLeft = (getXPosition(x,y) - 32.0f)*4000.0f/1024.0f;
				xPositionLeft = xPositionLeft/2.0f;
				GLfloat xPositionRight = (getXPosition(x,y) + 32.0f)*4000.0f/1024.0f;
				xPositionRight = xPositionRight/2.0f;

				GLfloat yPositionTop = (getYPosition(x,y) - 16.0f) * 1600.0f/168.0f;
				yPositionTop = yPositionTop/2.0f;
				GLfloat yPositionBottom = (getYPosition(x,y) + 16.0f) * 1600.0f/168.0f;
				yPositionBottom = yPositionBottom/2.0f;

				GLfloat xTextureCoord = getSpriteID(x,y)*(1.0f/8.0f);
				GLfloat yTextureCoord = getMood(x,y)*(1.0f/4.0f);

				vert->px = xPositionLeft;
				vert->py = yPositionTop;
				vert->pz = 0.0f;
				vert->tu = xTextureCoord;
				vert->tv = yTextureCoord;
				vert++;

				vert->px = xPositionRight;
				vert->py = yPositionTop;
				vert->pz = 0.0f;
				vert->tu = xTextureCoord + (1.0f/8.0f);
				vert->tv = yTextureCoord;
				vert++;

				vert->px = xPositionRight;
				vert->py = yPositionBottom;
				vert->pz = 0.0f;
				vert->tu = xTextureCoord + (1.0f/8.0f);
				vert->tv = yTextureCoord + (1.0f/4.0f);
				vert++;

				vert->px = xPositionLeft;
				vert->py = yPositionBottom;
				vert->pz = 0.0f;
				vert->tu = xTextureCoord;
				vert->tv = yTextureCoord + (1.0f/4.0f);
				vert++;
			}
	}

	glColor4ub(0xFF, 0xFF, 0xFF,0xFF);
    glInterleavedArrays( GL_T2F_V3F , sizeof(VertexFormatPos3Tex2) , mVertexArray ) ;
    glDrawArrays( GL_QUADS, 0 , 4*mWidth*mHeight ) ;

#endif
	glEnd ();
}
void SpriteDemoManagerC::updateSprites(DWORD milliseconds)
{
	mCurrentTime += milliseconds;

	if(mCurrentTime - mLastUpdateTime > ANIMATION_UPDATE_DELTA_TIME)
	{
		mLastUpdateTime = mCurrentTime;
		currentSpriteCol++;
		if (currentSpriteCol >= maxSpriteCols) {
			currentSpriteCol = 0;
		}
	}
}
