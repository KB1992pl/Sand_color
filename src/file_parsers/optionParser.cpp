#include "optionParser.h"

#include <fstream>
#include <string>
#include <algorithm>


#ifdef _WIN32
#include <direct.h> // _mkdir
#else
#endif
#include <sys/stat.h> // mkdir
#include <sys/types.h>
#include <iostream> //just for cout with warning... could be removed?

static void Upper(std::string &text)
{
	std::transform(text.begin(), text.end(), text.begin(), ::toupper);
}

OptionParser::OptionParser(const char* binaryPath) {
	mainPath = std::string(binaryPath);
	mainPath = mainPath.substr(0, mainPath.find_last_of("\\/"));
	optionFilePath = mainPath + "\\config\\options.cfg";
	s_mapStringValueOptions["FULLSCREEN"] = optFullScreen;
	s_mapStringValueOptions["WIDTH"] = optWidth;
	s_mapStringValueOptions["HEIGHT"] = optHeight;
	// invalid values to indicate that there was a problem loading cfg file
	this->screenHeight = -1;
	this->screenWidth = -1;

	this->load_options();
}
OptionParser::~OptionParser() {
	// Default destructor
}

/*
* Function converts token string to boolean value
*/
bool OptionParser::stringToBool(std::string token) {
	if (token == "TRUE" || token == "1") {
		return true;
	}
	else if (token == "FALSE" || token == "0") {
		return false;
	}
	// TODO: print some warning?
	return false;
}

/*
* Function gets a line from options file, parses it and applies the values into local variables
*/
void OptionParser::parse(std::string input)
{
	// Placeholder for parsing logic
	std::string optionName, value;
	size_t position; // position of '='

	position = input.find_first_of("=");

	if (position == std::string::npos) 
	{
		// No '=' found
		return;
	}
	optionName = input.substr(0, position);
	// Trim spaces from token
	while (optionName.back() == ' ') 
	{
		optionName.pop_back();
	}

	// Extract value
	value = input.substr(position, input.size());
	while 
		(value.front() == ' ' || value.front() == '=')
	{
		value.erase(0, 1);
	}
	while
		(value.back() == ' ') 
	{
		value.pop_back();
	}

	this->applyValues(optionName, value);
}

/*
* Function applies parsed option values into local variables
*/
void OptionParser::applyValues(std::string nameString, std::string valueString)
{
	switch (s_mapStringValueOptions[nameString])
	{
	case optFullScreen:
		this->fullscreen = this->stringToBool(valueString);
		break;
	case optWidth:
		this->screenWidth = std::stoi(valueString);
		break;
	case optHeight:
		this->screenHeight = std::stoi(valueString);
		break;
	default:
		break;
	}
}

/*
* Load options from file or create default file if it does not exist
*/
void OptionParser::load_options() {
	// Load options from file
	if (FileExists(this->optionFilePath.c_str()))
	{
		//read file and parse options (placeholder)
		std::ifstream optionsFile((this->optionFilePath).c_str());
		if (optionsFile) {
			std::string line;
			while (std::getline(optionsFile, line)) {
				if (line[0] == ';' || line[0] == '#') {
					// Skip comments
					continue;
				}
				Upper(line);
				this->parse(line);

			}
			optionsFile.close();
		}
		else
		{
			std::cout << "Could not open options file for reading: " << this->optionFilePath << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		//file does not exist - create default options file

		// Ensure config directory exists (parent of optionFilePath).
		const std::string dir = mainPath + "\\config";
#ifdef _WIN32
		// _mkdir returns 0 on success; ignore error if already exists
		_mkdir(dir.c_str());
#else
		// mkdir returns 0 on success; ignore error if already exists
		mkdir(dir.c_str(), 0755);
#endif

		// Default configuration content
		std::ofstream out(this->optionFilePath.c_str(), std::ofstream::out | std::ofstream::trunc);
		if (out) {
			out << this->generateDefaultConfig();
			out.close();
		}
		// Load from file so default options are aligned with contents of 'optionParser.h'
		this->load_options();
	}
}

/*
* Returns string with default config based on current environment (screen resolution, etc.)
*/
std::string OptionParser::generateDefaultConfig()
{
	InitWindow(100, 100, "gathering data..."); //create window so we can gather monitor data...
	int currentMonitor = GetCurrentMonitor();

	std::string defaultCfg = R"(; Default configuration file for the application.
		; Lines starting with ';' or '#' are comments.)";
	defaultCfg += "\n\n[Display]\n";
	defaultCfg += "; Use fullscreen mode (true/false)\n";
	defaultCfg += "fullscreen = true\n";
	defaultCfg += "; Window width in pixels (integer > 0)\n";
	defaultCfg += ("width = " + std::to_string(GetMonitorWidth(currentMonitor)) +'\n');
	defaultCfg += "; Window height in pixels (integer > 0)\n";
	defaultCfg += ("height = " + std::to_string(GetMonitorHeight(currentMonitor)) + '\n');

	CloseWindow(); //close window - will be reloaded with default cfg
	return defaultCfg;
}
