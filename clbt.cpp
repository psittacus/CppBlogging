/*
clbt  = Command Line Blogging Tool
/dev   = psittacus
Usage = Blogging, parsing, being cool, changing the world to something better, fighting GUIs, making vim great again
*/
#include <iostream>
#include "parser.h"
#include "config.h"

using namespace std;


int main(int argc, char* argv[]) {
    Parser parser;		    //parser.cpp
    Config config("configfile");    //not in use, yet
    while(true) {
	int choice = 0;
	cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n"
	        "| clbt ver. 0.1              |\n"
			"| 1) Write a post            |\n"
			"| 2) Upload a post           |\n"
			"| 3) Change design (in work) |\n"
	        "| 4) Configure your settings |\n"
			"| 5) Exit                    |\n"
			"|____________________________|\n";
	cin >> choice;
	string path;
	string order;
	switch(choice) {
	    case 1:
		system("vim post.txt"); //later: config.texteditor();
		break;
	    case 2: {
			cout << "Name of/Path to the file: ";
			cin >> path;
			order = parser.readFile(path.c_str()); 				//parses the file
			string finalOrder = "python2.7 blog.py " + order; 	//the python files uploades the parsed content
        	system(finalOrder.c_str()); 						//command gets executed x_x
			break;
		}
	    case 3: 
		cout << "Coming s(p)oon" << endl; //in work (theoretically)
		break;
	    case 4:
		cout << "Coming s(p)oon" << endl; //in work
		break;
	    case 5:
		return 0;
		break;
	}
    }
    
    return 0;
}
