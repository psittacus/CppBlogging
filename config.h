#include <iostream>
#include <fstream>

using namespace std;


class Config {
    public:
	Config(string path);
	string parser();
    private:
	string file;
};
