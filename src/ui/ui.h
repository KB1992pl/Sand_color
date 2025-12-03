#pragma once

#include"../drawableObj.h"

class Ui : public DrawableObj
{
private:
	Vector2 frameCoords[4];

	void drawFrame();

public:
	Vector2 GetUpperRight() { return frameCoords[0]; }
	Vector2 GetLowerLeft() { return frameCoords[2]; }
	Ui(Vector2 upperRight, float height, float width);
	~Ui() {};
	virtual void render();
	virtual bool isMouseOnObj(Vector2 mouseCoords);
};