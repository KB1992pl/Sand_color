#include "sand.h"

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

	for (unsigned row = 0; row < this->rowCount; row++)
	{
		for (unsigned col = 0; col < this->colCount; col++)
		{
			this->table[row][col] = 0;
		}
	}
}

Sand::~Sand()
{
	for (unsigned ii = 0; ii < this->rowCount; ii++)
	{
		delete[] this->table[ii];
	}
	delete []this->table;
}

void Sand::generateSand(Vector2 position, unsigned color, unsigned size)
{
	const unsigned x = static_cast<unsigned>(position.x - this->upperRight.x);
	const unsigned y = static_cast<unsigned>(position.y - this->upperRight.y);

	table[y][x] = color;
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
				
				DrawPixel(xOffset + col, yOffset+ row, color);
			}
		}
	}
}