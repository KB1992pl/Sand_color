#pragma once
#include"../drawableObj.h"
#include"../sand_block.h"


class Sand : public DrawableObj
{
private:
	int rowCount;
	int colCount;
	Color** table; // 2d array; accessing is: table[rowNo][ColNo]
	Vector2 upperRight;
	Vector2 lowerLeft;
	SandBlock* block;
public:
	Sand(Vector2 upperRight, float height, float width);
	~Sand();
	void generateSand(Vector2 position, Color color, int size);
	void simulate();
	void resetTable();
	void fillTable();
	virtual void render();
	virtual bool isMouseOnObj(Vector2 mouseCoords) { return false; };
	long pixels;
};