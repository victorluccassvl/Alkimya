class Gamestate
{
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void load() = 0;
	virtual void unload() = 0;
	virtual void treatInput() = 0;
}