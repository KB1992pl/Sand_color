#pragma once
#include"src/drawing_area/drawing_area.h"
#include"src/sand/sand.h"
#include"src/file_parsers/optionParser.h"
#include"src/ui/ui.h"

typedef struct
{
	unsigned maxFps;
	unsigned frameCounter; // counts up to 'max fps'
	DrawingArea* drawingArea;
	Sand* sand;
	OptionParser* options;
	Ui* ui;
}MainRes_t;