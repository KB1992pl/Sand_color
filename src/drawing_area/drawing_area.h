#pragma once
#include "../drawableObj.h"

class DrawingArea :public DrawableObj
{
private:
	Vector2 coords[4];

public:
	Vector2 GetUpperRight() { return coords[0]; }
	Vector2 GetLowerLeft() { return coords[2]; }
	DrawingArea(Vector2 upperRight, float height, float width);
	~DrawingArea() {};
	virtual bool isMouseOnObj(Vector2 mouseCoords);
	virtual void render();
};