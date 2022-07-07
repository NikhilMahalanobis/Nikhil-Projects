
class SpeedBoost : public PowerUpManager
{
public:
	SpeedBoost();
	SpeedBoost(char spritePage[100], int width, int height, int numCol, int numRow, int initialCol, int initialRow);
	~SpeedBoost();

	void updateSprite(DWORD milliseconds, SpeedBoost *speed);
	void renderSprite();
	void respawnTimer(DWORD milliseconds);
	void newSpawnLocation(SpeedBoost *speed);

private:
};

