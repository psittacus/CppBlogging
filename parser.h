#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Parser {
    public:
	string readFile(const char* path);
    private:
	string insertString(string text, string titel, string date, const char* path);
	string changeChar(char original, bool state);
};
