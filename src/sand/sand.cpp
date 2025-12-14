#include "sand.h"
#include <math.h>
#include <cstdlib>
#include "../utils.h"

Sand::Sand(Vector2 upperRight, float height, float width)
{
	this->rowCount = (int)height;
	this->colCount = (int)width;

	//create 2D array of color
	this->table = new Color * [this->rowCount];
	for (int ii = 0; ii < this->rowCount; ii++)
	{
		table[ii] = new Color[this->colCount];
	}
	this->upperRight = upperRight;
	this->lowerLeft.x = this->upperRight.x + width;
	this->lowerLeft.y = this->upperRight.y + height;

	this->resetTable();

	//create 2D array of blocks
	this->blockColCount = this->colCount / this->blockWidth;
	this->colCount % this->blockWidth == 0 ? this->blockColCount : this->blockColCount++;
	this->blockRowCount = this->rowCount / this->blockHeight;
	this->rowCount % this->blockHeight == 0 ? this->blockRowCount : this->blockRowCount++;
	this->block = new SandBlock** [this->blockRowCount];
	for (int row = 0; row < this->blockRowCount; row++)
	{
		this->block[row] = new SandBlock * [this->blockColCount];
		for (int col = 0; col < this->blockColCount; col++)
		{
			this->block[row][col] = new SandBlock(this->blockWidth, this->blockHeight, col * this->blockWidth, row * this->blockHeight, table, this->colCount, this->rowCount);
		}
	}
}

Sand::~Sand()
{
	for (int ii = 0; ii < this->rowCount; ii++)
	{
		delete[] this->table[ii];
	}
	delete []this->table;
}

void Sand::generateSand(Vector2 position, Color color, int radius)
{
	Color outputColor;
	if (radius == 0)
	{
		return;
	}
	const int x = static_cast<int>(position.x - this->upperRight.x);
	const int y = static_cast<int>(position.y - this->upperRight.y);
	int blockRow = -1;
	int blockCol = -1;
	this->getBlockCoordinates(position, &blockRow, &blockCol);
	//this algorithm will work only for brush radius lower than half of blockwidth or block height.
	this->block[blockRow][blockCol]->needToSimulate = true;
	this->setNeighborSimulateFlag(blockCol, blockRow);


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
				const int MAXRAND = 30;
				int random= (rand() % MAXRAND) - MAXRAND / 2;
				outputColor = color;
				outputColor.r = static_cast<unsigned char>(addWithSaturation(outputColor.r, random, 255));
				random = (rand() % MAXRAND) - MAXRAND / 2;
				outputColor.g = static_cast<unsigned char>(addWithSaturation(outputColor.g, random, 255));
				random = (rand() % MAXRAND) - MAXRAND / 2;
				outputColor.b = static_cast<unsigned char>(addWithSaturation(outputColor.b, random, 255));

				table[jj+y][ii+x] = outputColor;
			}
		}
	}
}
void Sand::simulate()
{
	for (int blockRow = this->blockRowCount - 1; blockRow >= 0; blockRow--)
	{
		for (int blockCol = 0; blockCol < this->blockColCount; blockCol++)
		{
			if ((this->block[blockRow][blockCol]->needToSimulate)||(this->block[blockRow][blockCol]->neighborSimulate))
			{
				this->block[blockRow][blockCol]->needToSimulate = false;
				this->block[blockRow][blockCol]->neighborSimulate = false;
				bool needSimulation = simulateBlock(blockCol, blockRow); // another simulation is needed?
				if (needSimulation)
				{
					//set current block simulation flag to true
					this->block[blockRow][blockCol]->needToSimulate = true;

					//set neighbor block simulation flag to true
					//for falling we could skip blocks above, but for future use 
					//(adding liquids, gases) it set flags for all blocks.
					//Can be changed for more performance
					this->setNeighborSimulateFlag(blockCol, blockRow);
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
	for (int row = 0; row < this->rowCount; row++)
	{
		for (int col = 0; col < this->colCount; col++)
		{
			if (this->table[row][col].a)
			{
				Color color{ this->table[row][col] };
				this->pixels++;
				DrawPixel(xOffset + col, yOffset+ row, color);
			}
		}
	}
}

void Sand::resetTable()
{
	for (int row = 0; row < this->rowCount; row++)
	{
		for (int col = 0; col < this->colCount; col++)
		{
			this->table[row][col].a = 0;
		}
	}
}

void Sand::getBlockCoordinates(const Vector2 mouseCoords, int* row, int* col)
{
	const int x = static_cast<int>(mouseCoords.x - this->upperRight.x);
	const int y = static_cast<int>(mouseCoords.y - this->upperRight.y);

	*row = y / this->blockHeight;
	*col = x / this->blockWidth;

	// if mouse is on the border of drawing (or slightly outside)
	// it can happen that row or column will be negative or
	// outside max values. Fix is below
	if (*col < 0)
	{
		*col = 0;
	}
	else if (*col >= this->blockColCount)
	{
		*col = this->blockColCount-1;
	}

	if (*row < 0)
	{
		*row = 0;
	}
	else if (*row >= this->blockRowCount)
	{
		this->blockRowCount-1;
	}
}

void Sand::legacySimulate()
{
	for (int row = this->rowCount - 2; row > 0; row--)
	{
		for (int col = 0; col < this->colCount; col++)
		{
			if (this->table[row][col].a != 0)
			{
				if (this->table[row + 1][col].a == 0)
				{
					this->table[row + 1][col] = this->table[row][col];
					this->table[row][col].a = 0;
					continue;
				}
				if (col > 0)
				{
					if ((this->table[row + 1][col - 1].a == 0))
					{
						this->table[row + 1][col - 1] = this->table[row][col];
						this->table[row][col].a = 0;
						continue;
					}
				}
				if (col < this->colCount - 1)
				{
					if ((this->table[row + 1][col + 1].a == 0))
					{
						this->table[row + 1][col + 1] = this->table[row][col];
						this->table[row][col].a = 0;
						continue;
					}
				}
			}
		}
	}
}

bool Sand::simulateBlock(int currentCol, int currentRow)
{
	//keep in mind that outermost blocks can have other row/col count than requested
	int blockRowCount = this->block[currentRow][currentCol]->blockHeight;
	int blockColCount = this->block[currentRow][currentCol]->blockWidth;
	int offsetRow = currentRow * this->blockHeight;
	int offsetCol = currentCol * this->blockWidth;
	bool simulated = false;
	for (int row = blockRowCount-1; row >= 0; row--)
	{
		for (int col = 0; col < blockColCount; col++)
		{
			//current row and column
			const int rr = row + offsetRow;
			const int cc = col + offsetCol;
			if (rr + 1 < this->rowCount)
			{
				if (this->table[rr][cc].a != 0)
				{

					if (this->table[rr + 1][cc].a == 0)
					{
						this->table[rr + 1][cc] = this->table[rr][cc];
						this->table[rr][cc].a = 0;
						simulated = true;
						continue;
					}

					if (cc > 0)
					{
						if ((this->table[rr + 1][cc - 1].a == 0))
						{
							this->table[rr + 1][cc - 1] = this->table[rr][cc];
							this->table[rr][cc].a = 0;
							simulated = true;
							continue;
						}
					}
					if (cc < this->colCount - 1)
					{
						if ((this->table[rr + 1][cc + 1].a == 0))
						{
							this->table[rr + 1][cc + 1] = this->table[rr][cc];
							this->table[rr][cc].a = 0;
							simulated = true;
							continue;
						}
					}
				}
			}
		}
	}
	return simulated;
}

void Sand::setNeighborSimulateFlag(int blockCol, int blockRow)
{
	for (int blockY = -1; blockY <= 1; blockY++)
	{
		for (int blockX = -1; blockX <= 1; blockX++)
		{
			if ((blockY == 0) && (blockX == 0))
			{
				continue; //this is central block - it have "need to simulate" flag set
			}
			if ((blockRow + blockY < 0) || (blockRow + blockY >= this->blockRowCount))
			{
				continue;
			}
			if ((blockCol + blockX < 0) || (blockCol + blockX >= this->blockColCount))
			{
				continue;
			}
			this->block[blockRow + blockY][blockCol + blockX]->neighborSimulate = true;
		}
	}
}