#include "sand_block.h"

SandBlock::SandBlock(int blockWidth, //block of sand width 
	int blockHeight, //block of sand height
	int offsetX, //offsetX on drawing area
	int offsetY, //offsetY on drawing area
	Color** frame, //2D array with pointers to "oryginal" drawing area
	int frameWidth, //drawing area width - for calculation whether block should be smaller than requested
	int frameHeight)
{
	this->imageCreated = false;
	this->blockWidth = blockWidth;
	this->blockHeight = blockHeight;
	this->offsetX = offsetX;
	this->offsetY = offsetY;
	// Check if requested block can be created or should be smaller
	if (blockWidth + offsetX > frameWidth)
	{
		this->blockWidth = frameWidth - offsetX;
	}
	if (blockHeight + offsetY > frameHeight)
	{
		this->blockHeight = frameHeight - offsetY;
	}

	// create 2D array with pointers
	this->table = new Color **[this->blockHeight];
	for (int ii = 0; ii < this->blockHeight; ii++)
	{
		this->table[ii] = new Color*[this->blockWidth];
	}

	// copy pointers
	for (int row = 0; row < this->blockHeight; row++)
	{
		for (int col = 0; col < this->blockWidth; col++)
		{
			this->table[row][col] =  &(frame[offsetY + row][offsetX + col]);
		}
	}

	//create buffer for image
	this->imageBuffer = new Color *[this->blockHeight];
	for (int ii = 0; ii < this->blockHeight; ii++)
	{
		this->imageBuffer[ii] = new Color [this->blockWidth];
	}

	//this->createBorders();
}

void SandBlock::render()
{
	for (unsigned row = 0; row < this->blockHeight; row++)
	{
		for (unsigned col = 0; col < this->blockWidth; col++)
		{
			if (this-> table[row][col]->a)
			{
				Color *color{ this->table[row][col] };
				DrawPixel(this->offsetX + col, offsetY + row, *color);
			}
		}
	}
	if (this->isBlockFull())
	{
		if (!this->imageCreated)
		{
			this->createImage();
			this->texture = LoadTextureFromImage(image);
		}
	}
	if (this->imageCreated)
	{
		DrawTexture(this->texture, 200, 200, WHITE);
	}
}

void SandBlock::createBorders()
{
	for (unsigned row = 0; row < this->blockHeight; row++)
	{
		for (unsigned col = 0; col < this->blockWidth; col++)
		{
			Color* c{ this->table[row][col] };
			*c = BLACK;
		}
	}
}
bool SandBlock::isBlockFull()
{
	bool retVal = true;
	for (unsigned row = 0; row < this->blockHeight; row++)
	{
		for (unsigned col = 0; col < this->blockWidth; col++)
		{
			Color* c{ this->table[row][col] };
			if (c->a == 0)
			{
				return false;
			}
		}
	}
	return true;
}

void SandBlock::createImage()
{
	for (unsigned row = 0; row < this->blockHeight; row++)
	{
		for (unsigned col = 0; col < this->blockWidth; col++)
		{
			Color* c{ this->table[row][col] };
			this->imageBuffer[row][col] = *c ;
		}
	}
	this->imageCreated = true;
	this->image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
	this->image.data = this->imageBuffer;
	this->image.height = this->blockHeight;
	this->image.width = this->blockWidth;
	this->image.mipmaps = 1;
}
