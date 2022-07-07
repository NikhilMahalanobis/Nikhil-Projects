class spriteDemoManagerC;
class Player : public ObjectC
{
	struct playerInput {
		int left, right, up, down, jab;
	};
public:
	Player();
	Player(char spritePage[100], float_t initPosX, float_t initPosY, float_t r, float_t g, float_t b, int playerId, float_t w, float_t h, playerInput inputKeys, b2World *world);
	~Player();
	virtual void render();
	void DoCollisions(DWORD milliseconds);
	void CollideHalf();
	void CollideBall(DWORD milliseconds);
	void update(DWORD milliseconds);
	void Player::myKeyboardFunction(unsigned char key, int x, int y);

	b2BodyDef playerBodyDef;
	b2Body* playerBody;
	b2PolygonShape playerBox;
	b2FixtureDef playerFixtureDef;

	float_t width;
	float_t height;
private:
	static int totalPlayers;
	int id;
	float_t scalingFactor{ 0.01 };
	
	float_t moveSpeed{ 15.0f };
	playerInput pInput;

	float_t red;
	float_t green;
	float_t blue;
	DWORD jabTimer;
	DWORD prevTime;


	bool jabPressed = false;
	SpriteDemoManagerC* sprite;
	SpriteDemoManagerC* player1sprite;
	SpriteDemoManagerC* player2sprite;
};



