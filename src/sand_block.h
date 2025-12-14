#include "drawableObj.h"
#include "raylib.h"


class SandBlock :public DrawableObj
{
private:
	int blockWidth, blockHeight, offsetX, offsetY;
	Color** table; //array of pointers to data from screen. 2D array converted into 1D table[width*height] instead of [height][width]
	Color* imageBuffer; //array to colors; 
	Texture2D texture;
	bool imageCreated;
	void createBorders();
	bool isBlockFull();
	void createImage(); //creates image from current bytes
public:
	Image image;
	SandBlock(int blockWidth, //block of sand width 
		int blockHeight, //block of sand height
		int offsetX, //offsetX on drawing area
		int offsetY, //offsetY on drawing area
		Color **frame, //2D array with pointers to "oryginal" drawing area
		int frameWidth, //drawing area width - for calculation whether block should be smaller than requested
		int frameHeight); //drawing area height - for calculation whether block should be smaller than requested
	~SandBlock() {};

	virtual void render();
	virtual bool isMouseOnObj(Vector2 mouseCoords) { return false; };
};