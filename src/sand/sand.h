#pragma once
#include"../drawableObj.h"


class Sand : public DrawableObj
{
private:
	unsigned rowCount;
	unsigned colCount;
	unsigned** table; // 2d array; accessing is: table[rowNo][ColNo]
	Vector2 upperRight;
	Vector2 lowerLeft;
public:
	Sand(Vector2 upperRight, float height, float width);
	~Sand();
	void generateSand(Vector2 position, unsigned color, unsigned size);
	void simulate();
	virtual void render();
	virtual bool isMouseOnObj(Vector2 mouseCoords) { return false; };
};