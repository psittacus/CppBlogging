#include <iostream>
#include <fstream>
#include "config.h"

//in development, very unimportant at the moment
using namespace std;

Config::Config(string path) {
    fstream fs;
    fs.open(path, fstream::in);
    while(getline(fs, Config::file)) {}
}

string Config::parser() {
    for(size_t i = 0; i < Config::file.length(); i++) {
    }
}
