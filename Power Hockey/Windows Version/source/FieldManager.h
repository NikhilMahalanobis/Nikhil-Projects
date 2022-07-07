class FieldManagerC
{
public:
	static FieldManagerC	*CreateInstance();
	static FieldManagerC	*GetInstance() {return sInstance;};
	~FieldManagerC(){};

	FieldC* getFieldPtr(int32_t index);
	void init(b2World *world);
	void renderField();
	void shutdown();

	b2BodyDef halfBodyDef;
	b2Body* halfBody;
	b2PolygonShape halfShape;
	b2FixtureDef halfFixtureDef;
private:
	static FieldManagerC *sInstance;
	FieldManagerC(){};
	static const int32_t NUM_FIELDS = 8;
	FieldC **mFieldPtr;
};