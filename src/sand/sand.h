#pragma once
#include"../drawableObj.h"
#include"../sand_block.h"


class Sand : public DrawableObj
{
private:
	int rowCount;
	int colCount;
	
	// 2d array; accessing is: table[rowNo][ColNo]. I tried to convert it into 1D array (accesing by [row*columnCount + col]
	// but simulation on such array appeared to be ~25% slower than 2D array
	Color** table;

	Vector2 upperRight;
	Vector2 lowerLeft;
	SandBlock* block;
public:
	Sand(Vector2 upperRight, float height, float width);
	~Sand();
	void generateSand(Vector2 position, Color color, int size);
	void simulate();
	void resetTable();
	virtual void render();
	virtual bool isMouseOnObj(Vector2 mouseCoords) { return false; };
	long pixels;
};