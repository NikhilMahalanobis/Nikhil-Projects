#include "baseTypes.h"
#include "ShapeDraw.h"
#include <windows.h>											// Header File For Windows
#include <stdio.h>												// Header File For Standard Input / Output
#include <stdarg.h>												// Header File For Variable Argument Routines
#include <math.h>												// Header File For Math Operations
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <cmath>
#include <string>
#include "box2d/b2_world.h"
#include "box2d/box2d.h"
#include "glut.h"
#include "collInfo.h"
#include "object.h"
#include "openGLFramework.h"
//#include "inputmapper.h"
#include "ball.h"
#include "BallManager.h"
#include "field.h"
#include "fieldmanager.h"
#include "gamedefs.h"
#include "gameobjects.h"
#include "random.h"
#include "stateManager.h"
#include "inputmanager.h"
#include "SpriteDemoManager.h"
#include "Player.h"
#define BALL_HIT_ACCELERATION 2


int Player::totalPlayers = 0;

Player::Player(char spritePage[100], float_t initPosX, float_t initPosY, float_t r, float_t g, float_t b, int playerId, float_t w, float_t h, playerInput inputKeys, b2World *world)
{
	pInput = inputKeys;
	id = playerId;
	mPosition.x = initPosX;
	mPosition.y = initPosY;
	mVelocity.x = 0;
	mVelocity.y = 0;
	width = w;
	height = h;
	red = r;
	green = g;
	blue = b;

	playerBodyDef.type = b2_dynamicBody;
	playerBodyDef.position.Set(initPosX * scalingFactor, initPosY*scalingFactor);
	playerBodyDef.fixedRotation = true;
	playerBodyDef.allowSleep = true;
	playerBody = world->CreateBody(&playerBodyDef);

	//Define fixture
	playerBox.SetAsBox((w*scalingFactor)/2, (h*scalingFactor)/2);

	playerFixtureDef.shape = &playerBox;
	playerFixtureDef.density = 1.0f;
	playerFixtureDef.friction = 0.0f;
	

	playerBody->CreateFixture(&playerFixtureDef);
	
	sprite = new SpriteDemoManagerC(spritePage, w * (1024.0f / 4000.0f), h * (768.0f / 4000.0f), mPosition, 1, 1, 0, 0);
}
Player::Player()
{
	totalPlayers++;
};

Player::~Player()
{
};

void Player::update(DWORD milliseconds)
{
	
	if (jabPressed)
	{
		jabTimer += milliseconds;

		if (jabTimer - prevTime > 1000)
		{
			prevTime = jabTimer;
			jabPressed = false;
		}
	}

	if (g_keys->keyDown[pInput.left] > 0) {

		b2Vec2 pos = playerBody->GetPosition();
		b2Vec2 vel = playerBody->GetLinearVelocity();

		mPosition.x = pos.x / scalingFactor;
		mPosition.y = pos.y / scalingFactor;
		
		float desiredVel = -12;
		float velChange = desiredVel - vel.x;
		float impulse = playerBody->GetMass() * velChange;
		playerBody->ApplyLinearImpulse(b2Vec2(impulse, 0), playerBody->GetWorldCenter(), true);

	}
	
	
	if (g_keys->keyDown[pInput.right] > 0) {
		
		b2Vec2 pos = playerBody->GetPosition();
		b2Vec2 vel = playerBody->GetLinearVelocity();

		mPosition.x = pos.x / scalingFactor;
		mPosition.y = pos.y / scalingFactor;

		float desiredVel = 12;
		float velChange = desiredVel - vel.x;
		float impulse = playerBody->GetMass() * velChange;
		playerBody->ApplyLinearImpulse(b2Vec2(impulse, 0), playerBody->GetWorldCenter(), true);

	}
	
	
	if (g_keys->keyDown[pInput.up] > 0) {
		
		b2Vec2 pos = playerBody->GetPosition();
		b2Vec2 vel = playerBody->GetLinearVelocity();

		mPosition.x = pos.x / scalingFactor;
		mPosition.y = pos.y / scalingFactor;

		float desiredVel = 12;
		float velChange = desiredVel - vel.y;
		float impulse = playerBody->GetMass() * velChange;
		playerBody->ApplyLinearImpulse(b2Vec2(0, impulse), playerBody->GetWorldCenter(), true);

	}
	if (g_keys->keyDown[pInput.down] > 0) {
		b2Vec2 pos = playerBody->GetPosition();
		b2Vec2 vel = playerBody->GetLinearVelocity();

		mPosition.x = pos.x / scalingFactor;
		mPosition.y = pos.y / scalingFactor;

		float desiredVel = -12;
		float velChange = desiredVel - vel.y;
		float impulse = playerBody->GetMass() * velChange;
		playerBody->ApplyLinearImpulse(b2Vec2(0, impulse), playerBody->GetWorldCenter(), true);
	}

	if (g_keys->keyDown[pInput.left] <= 0 && g_keys->keyDown[pInput.right] <= 0 && g_keys->keyDown[pInput.down] <= 0 && g_keys->keyDown[pInput.up] <= 0)
	{
		playerBody->SetLinearVelocity(b2Vec2(0, 0));
	}

	if (g_keys->keyDown[pInput.jab] > 0 && jabPressed == false)
	{
		b2Vec2 pos = playerBody->GetPosition();
		b2Vec2 vel = playerBody->GetLinearVelocity();

		mPosition.x = pos.x / scalingFactor;
		mPosition.y = pos.y / scalingFactor;

		b2Vec2 desiredChange;
		desiredChange.x = vel.x * 8;
		desiredChange.y = vel.y * 8;
		b2Vec2 impulse = playerBody->GetMass() * desiredChange;
		playerBody->ApplyLinearImpulse(impulse, playerBody->GetWorldCenter(), true);

		jabPressed = true;
	}

}
void Player::render()
{
	glColor3f(red, green, blue);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	float posX = playerBody->GetPosition().x;
	float posY = playerBody->GetPosition().y;
	posX /= scalingFactor;
	posY /= scalingFactor;
	sprite->renderSprites({ mPosition.x * (1024.0f / 4000.0f), mPosition.y * (768.0f / 4000.0f) });
	
}

void Player::DoCollisions(DWORD milliseconds)
{
	
}

void Player::CollideHalf()
{
	b2Vec2 vel = playerBody->GetLinearVelocity();
	b2Vec2 pos = playerBody->GetPosition();

	//moving right
	if (vel.x > 0)
	{
				
	}
	//movign left
	else if (vel.x < 0)
	{

	}
}






