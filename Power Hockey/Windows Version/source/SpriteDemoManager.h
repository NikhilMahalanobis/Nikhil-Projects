struct VertexFormatPos3Tex2
{   // Custom vertex format for position+normal
	float tu , tv ;             ///< texture coordinates	
	float px , py , pz ;        ///< untransformed (world-space) position
};

	class SpriteDemoManagerC
{
public:
	
	SpriteDemoManagerC(char spriteSheet[100], int width, int height, Coord2D spawnPosition, int numSpritesCol, int numSpritesRow, int startRow, int startCol);
	SpriteDemoManagerC() {};
	~SpriteDemoManagerC() {};
	void		updateSprites(DWORD milliseconds);
	void		renderSprites(Coord2D objectPosition);
	void setStartCol(int col) { currentSpriteCol = col; };
	int* getStartCol() { return &currentSpriteCol; };

private:
	GLuint mSpriteTextureMap;

	DWORD mLastUpdateTime;
	DWORD mCurrentTime;

	int spriteWidth;
	int spriteHeight;

	Coord2D screenPosition;

	int currentSpriteRow;
	int currentSpriteCol;
	int maxSpriteRows;
	int maxSpriteCols;
};

#define ANIMATION_UPDATE_DELTA_TIME 50