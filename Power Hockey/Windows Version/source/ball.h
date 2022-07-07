class SpriteDemoManagerC;
class BallC : public ObjectC
{
public:
    BallC();
	BallC(float_t initPosX, float_t initPosY, float_t initVelX, float_t initVelY, float_t initAccelX, float_t initAccelY, float_t radius, int id, b2World* world);
    ~BallC();
	void setBallColor(long color) {mBallColor = color;};
    long getBallColor() {return mBallColor;};
    void update(DWORD milliseconds);
    virtual void render();
	void DoCollisions(DWORD milliseconds);
    void disable() {mEnabled = false;};
    void enable() {mEnabled = true;};
    void setRadius(float radius) {mCollInfo.param1 = radius;mRadius = radius;};
    float getRadius() { return mRadius; };
	void setRandomColor();
	void reduceVelocity(float_t factor);
	void moveUp();
    void setAcceleration(float x, float y) { mAcceleration.x = x; mAcceleration.y = y; };
    Coord2D* getAcceleration() { return &mAcceleration; };
    int getP1Score() { return p1Score; };
    int getP2Score() { return p2Score; };
 
    b2BodyDef ballBodyDef;
    b2Body* ballBody;
    b2CircleShape ballCircle;
    b2FixtureDef ballFixtureDef;
    float_t scalingFactor{ 0.01f };
private:
	void CollideField(DWORD milliseconds);

    Coord2D mAcceleration;
	long mBallColor;
    bool mEnabled;
    float mRadius;
	bool mAtRest;

    DWORD lastUpdateTime;
    DWORD currentUpdateTime;

    int velocityArray[2]{ -1,1 };

    SpriteDemoManagerC *sprite;
    SpriteDemoManagerC* player1Score;
    SpriteDemoManagerC* player2Score;

    bool renderObject{ true };
    bool playerScored;

    int p1Score = 0;
    int p2Score = 0;
};
