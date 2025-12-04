#pragma once

int addWithSaturation(int x, int y, int max)
{
	int returnValue;
	int value = x + y;
	if (value > max)
	{
		returnValue = max;
	}
	else if (value < 0)
	{
		returnValue = 0;
	}
	else
	{
		returnValue = value;
	}
	return returnValue;
}