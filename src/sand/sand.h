#pragma once
#include"../drawableObj.h"
#include"../sand_block.h"


class Sand : public DrawableObj
{
private:
	int rowCount;
	int colCount;

	//variables for blocks - see sand_block.h
	const int blockWidth =50;
	const int blockHeight = 50;
	int blockRowCount;
	int blockColCount;
	// 2d array of sand blocks
	SandBlock*** block;
	
	// 2d array; accessing is: table[rowNo][ColNo]. I tried to convert it into 1D array (accesing by [row*columnCount + col]
	// but simulation on such array appeared to be ~25% slower than 2D array
	Color** table;

	Vector2 upperRight;
	Vector2 lowerLeft;

	// first method of simulate - saved just for performance comparison
	void legacySimulate();

	bool simulateBlock(int offsetX, int offsetY); //return true if simulation was performed
	void setNeighborSimulateFlag(int blockCol, int blockRow);
public:
	long pixels;

	Sand(Vector2 upperRight, float height, float width);
	~Sand();
	void generateSand(Vector2 position, Color color, int size);
	void simulate();
	void resetTable();
	virtual void render();
	virtual bool isMouseOnObj(const Vector2 mouseCoords) { return false; };
	void getBlockCoordinates(const Vector2 mouseCoords, int* row, int* col);

};