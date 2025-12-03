#include "drawing_area.h"

DrawingArea::DrawingArea(Vector2 upperRight, float height, float width)
{
	this->coords[0] = upperRight;

	this->coords[1].x = this->coords[0].x + width;
	this->coords[1].y = this->coords[0].y;

	this->coords[2].x = this->coords[0].x + width;
	this->coords[2].y = this->coords[0].y + height;

	this->coords[3].x = this->coords[0].x;
	this->coords[3].y = this->coords[0].y + height;
}

bool DrawingArea::isMouseOnObj(Vector2 mouseCoords)
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

void DrawingArea::render()
{
	unsigned firstIdx = 0, secondIdx = 1;
	for (; firstIdx < 4; firstIdx++, secondIdx++)
	{
		secondIdx %= 4;
		DrawLineV(this->coords[firstIdx], this->coords[secondIdx], BLACK);
	}
}