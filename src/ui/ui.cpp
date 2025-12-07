#include "ui.h"

Ui::Ui(Vector2 upperRight, float height, float width)
{
	this->frameCoords[0] = upperRight;

	this->frameCoords[1].x = this->frameCoords[0].x + width;
	this->frameCoords[1].y = this->frameCoords[0].y;

	this->frameCoords[2].x = this->frameCoords[0].x + width;
	this->frameCoords[2].y = this->frameCoords[0].y + height;

	this->frameCoords[3].x = this->frameCoords[0].x;
	this->frameCoords[3].y = this->frameCoords[0].y + height;

	this->colorGradientWidth = width;
	this->blackWhiteGradientWidth = width / 3;

	this->colorGradientHeight = height / 2;
	this->blackWhiteGradientHeight = height / 4;

}

void Ui::drawFrame()
{
	unsigned firstIdx = 0, secondIdx = 1;
	for (; firstIdx < 4; firstIdx++, secondIdx++)
	{
		secondIdx %= 4;
		DrawLineV(this->frameCoords[firstIdx], this->frameCoords[secondIdx], BLACK);
	}
}

void Ui::render()
{
	drawFrame();
	drawColorGradient();
	drawBlackWhiteGradient();
}

bool Ui::isMouseOnObj(Vector2 mouseCoords) 
{
	const Vector2 upperRight = this->GetUpperLeft();
	const Vector2 lowerLeft = this->GetLowerRight();
	if ((mouseCoords.x >= upperRight.x) && (mouseCoords.x <= lowerLeft.x))
	{
		if ((mouseCoords.y >= upperRight.y) && (mouseCoords.y <= lowerLeft.y))
		{
			return true;
		}
	}
	return false;
}

void Ui::getColorFromGradient(Vector2 mouseCoords, Color *color) 
{
	const float gradientX = mouseCoords.x - this->GetUpperLeft().x;
	const float gradientY = mouseCoords.y - this->GetUpperLeft().y;
	const float singleGradientHeight = this->colorGradientHeight / gradientBoxCount;
	if ((gradientX < 0) || (gradientX > this->colorGradientWidth))
	{
		// x coordinates outside of gradient
		return;
	}
	if ((gradientY < 0) || (gradientY > this->colorGradientHeight))
	{
		return;
	}

	float partWhite = gradientX / this->colorGradientWidth;
	// if RED/RED+GREEN gradient
	if ((gradientY >= 0) && (gradientY <= singleGradientHeight))
	{
		float partcolor = gradientY / singleGradientHeight; // percetnage where was clicked
		float green = partcolor * this->maxColor;
		color->r = this->maxColor;
		color->g = green + (partWhite*(this->maxColor - green));
		color->b = partWhite * this->maxColor;
	}

	// if RED+GREEN/GREEN gradient
	else if ((gradientY >= singleGradientHeight) && (gradientY <= singleGradientHeight*2))
	{
		float partcolor = (gradientY- singleGradientHeight) / singleGradientHeight; // percetnage where was clicked
		float red = (1.0f - partcolor) * this->maxColor;
		color->r = red + partWhite*(this->maxColor -red);
		color->g = this->maxColor;
		color->b = partWhite * this->maxColor;
	}

	// if GREEN/GREEN+BLUE gradient
	else if ((gradientY >= singleGradientHeight*2) && (gradientY <= singleGradientHeight*3))
	{
		float partcolor = (gradientY-(2*singleGradientHeight)) / singleGradientHeight; // percetnage where was clicked
		float blue = partcolor * this->maxColor;
		color->r = partWhite * this->maxColor;
		color->g = this->maxColor;
		color->b = blue + partWhite* (this->maxColor - blue);
	}

	// if GREEN+BLUE/BLUE gradient
	else if ((gradientY >= singleGradientHeight * 3) && (gradientY <= singleGradientHeight * 4))
	{
		float partcolor = (gradientY - (3 * singleGradientHeight)) / singleGradientHeight; // percetnage where was clicked
		float green = (1.0f - partcolor) * this->maxColor;
		color->r = partWhite * this->maxColor;
		color->g = green + partWhite*(this->maxColor - green);
		color->b = this->maxColor;
	}

	// if BLUE/BLUE+RED gradient
	else if ((gradientY >= singleGradientHeight * 4) && (gradientY <= singleGradientHeight * 5))
	{
		float partcolor = (gradientY - (4 * singleGradientHeight)) / singleGradientHeight; // percetnage where was clicked
		float red = partcolor * this->maxColor;
		color->r = red + partWhite*(this->maxColor -red);
		color->g = partWhite * this->maxColor;
		color->b = this->maxColor;
	}
	color->a = 255;
}

void Ui::getBlackFromGradient(Vector2 mouseCoords, Color* color)
{
	const float gradientX = mouseCoords.x - this->GetUpperLeft().x;
	const float gradientY = mouseCoords.y - (this->GetUpperLeft().y + this->colorGradientHeight);
	if ((gradientX < 0) || (gradientX > this->blackWhiteGradientWidth))
	{
		// x coordinates outside of gradient
		return;
	}
	if ((gradientY < 0) || (gradientY > this->blackWhiteGradientHeight))
	{
		return;
	}
	
	this->maxColor = unsigned char(255 * (1-(gradientY / this->blackWhiteGradientHeight)));

	
}

void Ui::drawColorGradient()
{

	const float colorHeight = this->colorGradientHeight / gradientBoxCount;
	Rectangle rec{ this->GetUpperLeft().x,this->GetUpperLeft().y, this->colorGradientWidth,colorHeight };

	DrawRectangleGradientEx(rec, { this->maxColor,0,0,255 }, { this->maxColor,this->maxColor,0,255}, WHITE, WHITE);
	rec.y += colorHeight;
	DrawRectangleGradientEx(rec, { this->maxColor,this->maxColor,0,255 }, {0,this->maxColor,0,255}, WHITE, WHITE);
	rec.y += colorHeight;
	DrawRectangleGradientEx(rec, { 0,this->maxColor,0,255 }, { 0,this->maxColor,this->maxColor,255 }, WHITE, WHITE);
	rec.y += colorHeight;
	DrawRectangleGradientEx(rec, { 0,this->maxColor,this->maxColor,255 }, { 0,0,this->maxColor,255 }, WHITE, WHITE);
	rec.y += colorHeight;
	DrawRectangleGradientEx(rec, { 0,0,this->maxColor,255 }, { this->maxColor,0,this->maxColor,255 }, WHITE, WHITE);
}

void Ui::drawBlackWhiteGradient()
{
	Rectangle rec{ this->GetUpperLeft().x,this->GetUpperLeft().y+ this->colorGradientHeight, this->blackWhiteGradientWidth,this->blackWhiteGradientHeight };
	DrawRectangleGradientV(rec.x, rec.y, rec.width, rec.height, WHITE, BLACK);
}
