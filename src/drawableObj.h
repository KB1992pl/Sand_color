#pragma once
#include <raylib.h>

class DrawableObj
{
public:
	DrawableObj() {};
	virtual bool isMouseOnObj(Vector2 mouseCoords) = 0;
	virtual void render() =0;
}; 