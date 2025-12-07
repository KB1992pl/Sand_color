#pragma once

#include"../drawableObj.h"

class Ui : public DrawableObj
{
private:
	Vector2 frameCoords[4];

	float colorGradientWidth;
	float colorGradientHeight;
	float redGreenHeight;
	float greenBlueHeight;
	float blueRedHeight;


	float blackWhiteGradientWidth;
	float blackWhiteGradientHeight;

	unsigned char maxColor = 255; //white coefficient - how white/black should colors be

	void drawFrame();

	const int gradientBoxCount = 5;

public:
	void drawColorGradient();
	void drawBlackWhiteGradient();
	Vector2 GetUpperLeft() { return frameCoords[0]; }
	Vector2 GetLowerRight() { return frameCoords[2]; }
	void getColorFromGradient(Vector2 mouseCoords, Color* color);
	void getBlackFromGradient(Vector2 mouseCoords, Color* color); //todo: change current color on changing black/white gradient
	Ui(Vector2 upperRight, float height, float width);
	~Ui() {};
	virtual void render();
	virtual bool isMouseOnObj(Vector2 mouseCoords);
};