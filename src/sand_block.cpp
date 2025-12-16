#include "sand_block.h"

SandBlock::SandBlock(int blockWidth, //block of sand width 
	int blockHeight, //block of sand height
	int offsetX, //offsetX on drawing area
	int offsetY, //offsetY on drawing area
	Color** frame, //2D array with pointers to "oryginal" drawing area
	int frameWidth, //drawing area width - for calculation whether block should be smaller than requested
	int frameHeight)
{
	this->image.data = NULL;
	this->texture.id = -1;
	this->needToSimulate = false;
	this->neighborSimulate = false;
	this->imageCreated = false;
	this->blockFull = false;
	this->blockEmpty = true;
	
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
	this->table = new Color *[this->blockHeight * this->blockWidth];

	// copy pointers
	for (int row = 0; row < this->blockHeight; row++)
	{
		for (int col = 0; col < this->blockWidth; col++)
		{
			this->table[row* this->blockWidth + col] =  &(frame[offsetY+row]  [offsetX + col]);
		}
	}

	//create buffer for image
	this->imageBuffer = new Color [this->blockHeight * this->blockWidth];

}

void SandBlock::render()
{
	this->pixels = 0;
	if (this->blockEmpty)
	{
		//nothing to draw
		return;
	}
	if (this->blockFull)
	{
		//this->pixels = this->blockWidth * this->blockHeight; //???
		if (!this->imageCreated)
		{
			this->createImage();
			this->texture = LoadTextureFromImage(image);
		}
		if (this->imageCreated)
		{
			DrawTexture(this->texture, this->offsetX, offsetY, WHITE);
		}
	}
	else
	{
		for (int row = 0; row < this->blockHeight; row++)
		{
			for (int col = 0; col < this->blockWidth; col++)
			{
				if (this->table[row * this->blockWidth + col]->a)
				{
					DrawPixel(this->offsetX + col, offsetY + row, *this->table[row * this->blockWidth + col]);
					this->pixels++;
				}
			}
		}
	}
}



void SandBlock::checkBlockFull()
{
	this->blockFull = true;
	this->blockEmpty = true;
	for (int row = 0; row < this->blockHeight; row++)
	{
		for (int col = 0; col < this->blockWidth; col++)
		{
			Color* c{ this->table[row * this->blockWidth + col] };
			if (c->a == 0)
			{
				// at least one lacking pixel means that block is not full
				this->blockFull = false;
			}
			else
			{
				// at least one pixel means that block is not empty
				this->blockEmpty = false;
			}
			if ((!this->blockFull) && (!this->blockEmpty))
			{
				return;
			}
		}
	}
}

void SandBlock::createImage()
{
	for (int row = 0; row < this->blockHeight; row++)
	{
		for (int col = 0; col < this->blockWidth; col++)
		{
			Color* c{ this->table[row * this->blockWidth + col] };
			this->imageBuffer[row * this->blockWidth + col] = *c ;
		}
	}
	this->imageCreated = true;
	this->image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
	this->image.data = this->imageBuffer;
	this->image.height = this->blockHeight;
	this->image.width = this->blockWidth;
	this->image.mipmaps = 1;
}
