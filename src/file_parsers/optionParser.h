#include <inttypes.h>
#include <raylib.h>
#include <map>
#include<string>


class OptionParser {
private:
	enum StringValueOptions {
		optFullScreen,
		optWidth,
		optHeight
	};
	std::map<std::string, StringValueOptions> s_mapStringValueOptions;
	std::string optionFilePath, mainPath; //mainPath - path to main binary
	void parse(std::string);
	void applyValues(std::string nameString, std::string valueString);
	bool stringToBool(std::string token);

	std::string generateDefaultConfig();

public:
	//options variables
	bool fullscreen;
	int screenWidth;
	int screenHeight;

	OptionParser(const char* binaryPath);
	~OptionParser(void);
	void load_options(void);
};
