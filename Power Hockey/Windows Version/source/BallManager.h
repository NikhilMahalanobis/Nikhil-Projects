class BallManagerC
{
public:
	static BallManagerC	*CreateInstance();
	static BallManagerC	*GetInstance() {return sInstance;};
	~BallManagerC(){};

	int32_t	getNumBalls() {return NUM_BALLS;};
	void	init(b2World *world);
	void	shutdown();
	void	updateBalls(DWORD milliseconds);
	BallC *	getBallPtr(int32_t index);
	void	renderBalls();

	bool p1Wins = false;
	bool p2Wins = false;

private:
	BallC **ballPtrs;
	static BallManagerC *sInstance;
	static const int32_t NUM_BALLS = 1;
	BallManagerC(){};
	const int winScore = 7;
};