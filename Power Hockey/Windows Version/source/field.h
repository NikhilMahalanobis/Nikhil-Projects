class SpriteDemoManagerC;
class b2World;
class b2Body;
class FieldC : public ObjectC
{
    
public:
	FieldC(float_t x, float_t y, float_t height, float_t width, uint32_t initColor, b2World *world);
    ~FieldC();
    void setColor(uint32_t color) {mFieldColor = color;};
    long getColor() {return mFieldColor;};
    void update(DWORD milliseconds);
    virtual void render();
    void setWidth(uint32_t width) {mWidth = width;};
    uint32_t getWidth() {return mWidth;};
    void setHeight(uint32_t height) {mHeight = height;};
    uint32_t getHeight() {return mHeight;};

	void moveUp();
    
private:
    uint32_t mFieldColor;
	float_t mHeight;
	float_t mWidth;
	bool8_t mEnabled;
    SpriteDemoManagerC *background;

    float scalingFactor = 0.01f;
    b2BodyDef fieldBodyDef;
    b2Body* fieldBody;
    b2PolygonShape fieldBox;
    b2FixtureDef fieldFixtureDef;
};