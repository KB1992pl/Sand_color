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
}

bool Ui::isMouseOnObj(Vector2 mouseCoords) 
{
	const Vector2 upperRight = this->GetUpperRight();
	const Vector2 lowerLeft = this->GetLowerLeft();
	if ((mouseCoords.x >= upperRight.x) && (mouseCoords.x <= lowerLeft.x))
	{
		if ((mouseCoords.y >= upperRight.y) && (mouseCoords.y <= lowerLeft.y))
		{
			return true;
		}
	}
	return false;
}
