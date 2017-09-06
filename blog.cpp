#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

string insertString(string text, const char* path) {
	ifstream ifs(path);
	string temp_text;
	string text_upper;
	string text_lower;
	bool splitPointReached = false;
	if(ifs.is_open()) {
		while(getline(ifs, temp_text)) {
			if(splitPointReached) {
				text_lower += temp_text + "\n";
			} else {
				text_upper += temp_text + "\n";
			} 
                        if(temp_text == "<!-- split here -->") {
                                splitPointReached = true;
                        }

		}
	} else {
		cerr << "Something went wrong while opening a textfile";
	}
	ifs.close();
	string final_text = text_upper + text + text_lower;
	return final_text;
}



bool readFile(char* path) {
	ifstream ifs(path);
	string line_old;
	string line_new;
	bool h3 = false, b = false, date = false, slash = false, afterDate = false;
	if(ifs.is_open()) {
		while(getline(ifs, line_old)) {
			for(char letter : line_old) {
				if(!slash) {
					if(letter == '#') {
						if(!h3) {
							line_new += "<h3>"; 	//bigger text
							h3 = true;
						} else {
							line_new += "</h3>";	//end bigger text
							h3 = false;
						}
					} else if(letter == '_') {
						if(!b) {			//start bold text
							line_new += "<b>";
							b = true;
						} else {
							line_new += "</b>";	//end bold text
							b = false;
						}
					} else if(letter == '~') {
						if(!date) {
							line_new += "<h4 id=\"date\">";  //date
							date = true;
						} else {
							line_new += "</h4>";
							date = false;
							afterDate = true;
						}	
					} else if(letter == '*') {
						line_new += "&bullet;";		//list
					} else if(letter == '/') {
						slash = true;
					} else {
						line_new += letter;
					}
				} else {
					if(letter == '/') {
						line_new += "/";     		//double slash means one slash in the html file
						slash = false;
					} else {
						line_new += letter;
						slash = false;
					}
				}
			}
			if(!afterDate) {
			   line_new += "\x3c\x62r\x3e\n";
			}
			afterDate = false;
			slash = false; 						//slash does not count over a line
		}
	}
	ifs.close();
	time_t now = time(0);
	tm *ltm = localtime(&now);
	string month;
	string day;
	string hour;
	if((1 + ltm->tm_mon) < 10) {
	    month = "0" + to_string(1+ltm->tm_mon);  //09 ... instead of 9
	} else {
	    month = to_string(1+ ltm->tm_mon);
	}
	if(ltm->tm_mday < 10) {
	    day = "0" + to_string(ltm->tm_mday);
	} else {
	    day = to_string(ltm->tm_mday);
	}
	if(ltm->tm_hour < 10) {
	    hour = "0" + to_string(ltm->tm_hour);
	} else {
	    hour = to_string(ltm->tm_hour);
	}
	string year = to_string(1900 + ltm->tm_year);
	string fullDate = year + month + day + hour;
	cout << fullDate << endl;
	ofstream ofs("posts/" + fullDate + ".html");
	ofs << line_new;
	ofs.close();
	string pathToIndex = "index.html";
	string textToInsert = insertString(line_new, pathToIndex.c_str());	//c_str() --> char*
	ofstream of(pathToIndex.c_str());
	of << textToInsert;
	of.close();
	return true;
}

	
int main(int argc, char** argv) {
	if(argc < 2) {
		cout << "usage: blogger [argument]" << endl;
	} else {
		if(readFile(argv[1])) {
			//system("python blog.py");	//just the python script for uploading the code
			cout << "changed!";
		}
	}
	return 0;
}
