class SpeedBoost;
class BombClass;
class PowerUpManager
{
	friend class SpeedBoost;
	friend class BombClass;
public:
	PowerUpManager(char spritePage[100], int width, int height, int numCol, int numRow, int initialCol, int initialRow);
	PowerUpManager();
	~PowerUpManager();

	void updateSprite(DWORD milliseconds);
	void renderSprite();

	Coord2D* getPosition() { return &powerUpPositions; };
	int* getWidth() { return &powerWidth; };
	int* getHeight() { return &powerHeight; };
	bool* getSpawned() { return &isSpawned; };
	

private:
	int currentSprite; 
	DWORD lastUpdateTime;
	DWORD currentUpdateTime;

    Coord2D powerUpPositions;
	Coord2D currentPos;
	SpriteDemoManagerC* sprite;

	bool isSpawned;

	int powerHeight;
	int powerWidth;


};
#define RESPAWN_TIME 5000

