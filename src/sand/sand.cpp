#include "sand.h"
#include <math.h>

Sand::Sand(Vector2 upperRight, float height, float width)
{
	this->rowCount = (unsigned)height;
	this->colCount = (unsigned)width;
	this->table = new unsigned* [(unsigned)height];
	for (unsigned ii = 0; ii < height; ii++)
	{
		table[ii] = new unsigned[(unsigned)width];
	}

	this->upperRight = upperRight;
	this->lowerLeft.x = this->upperRight.x + width;
	this->lowerLeft.y = this->upperRight.y + height;

	this->resetTable();
}

Sand::~Sand()
{
	for (unsigned ii = 0; ii < this->rowCount; ii++)
	{
		delete[] this->table[ii];
	}
	delete []this->table;
}

void Sand::generateSand(Vector2 position, unsigned color, int radius)
{
	if (radius == 0)
	{
		return;
	}
	const int x = static_cast<int>(position.x - this->upperRight.x);
	const int y = static_cast<int>(position.y - this->upperRight.y);

	if (radius == 1)
	{
		if ((y >= this->rowCount) || (x >= this->colCount))
		{
			return;
		}
		table[y][x] = color;
	}
	//
	else
	{
		for (int ii = -radius; ii < radius + 1; ii++)
		{
			if ((ii + x < 0) || (ii + x > (this->colCount - 1)))
			{
				continue;
			}
			for (int jj = -radius + abs(ii); jj < radius - abs(ii) + 1; jj++)
			{
				if ((jj + y < 0) || (jj + y > (this->rowCount - 1)))
				{
					continue;
				}
				table[jj+y][ii+x] = color;
			}
		}
	}
}
void Sand::simulate()
{
	for (unsigned row = this->rowCount - 2; row > 0; row--)
	{
		for (unsigned col = 0; col < this->colCount; col++)
		{
			if (this->table[row][col])
			{
				if (this->table[row + 1][col] == 0)
				{
					this->table[row + 1][col] = this->table[row][col];
					this->table[row][col] = 0;
					continue;
				}
				if (col > 0)
				{
					if ((this->table[row + 1][col-1] == 0))
					{
						this->table[row + 1][col - 1] = this->table[row][col];
						this->table[row][col] = 0;
						continue;
					}
				}
				if (col < this->colCount - 1)
				{
					if ((this->table[row + 1][col +1] == 0))
					{
						this->table[row + 1][col + 1] = this->table[row][col];
						this->table[row][col] = 0;
						continue;
					}
				}
			}
		}
	}
}

void Sand::render()
{
	this->pixels = 0;
	const unsigned xOffset = static_cast<unsigned>(this->upperRight.x);
	const unsigned yOffset = static_cast<unsigned>(this->upperRight.y);
	for (unsigned row = 0; row < this->rowCount; row++)
	{
		for (unsigned col = 0; col < this->colCount; col++)
		{
			if (this->table[row][col])
			{

				Color color = BLUE;

				if (this->table[row][col] == 2)
				{
					color = RED;
				}
				else if (this->table[row][col] == 3)
				{
					color = GREEN;
				}
				this->pixels++;
				DrawPixel(xOffset + col, yOffset+ row, color);
			}
		}
	}
}

void Sand::resetTable()
{
	for (unsigned row = 0; row < this->rowCount; row++)
	{
		for (unsigned col = 0; col < this->colCount; col++)
		{
			this->table[row][col] = 0;
		}
	}
}
