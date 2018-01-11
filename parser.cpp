#include "parser.h"
#include <iostream>
#include <fstream>

using namespace std;

string Parser::insertString(string text, string titel, string date, const char* path) {
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
    
    string final_text = text_upper +
    					"\n<h4 id=\"date\">" + 
    					date + 
    					"</h4><h3><a class=\"titel\" href=\"http://psittacus.bplaced.net/posts/" + 
    					text + 
    					".html\">" + 
    					titel + 
    					"</a><h3><br><hr><br>\n" + 
    					text_lower;


    return final_text;
}

string Parser::readFile(const char* path) {
    ifstream ifs(path);
    string line_old;
    string line_new;
    string titel;
    bool h3 = false, b = false, date = false, slash = false, afterDate = false, link = false;
    if(ifs.is_open()) {
	while(getline(ifs, line_old)) {
	    for(char letter : line_old) {
		if(slash) {
			if(letter == 'a') {						//a = href
				if(!link) {
					line_new += "<a href=\"";
					link = true;
				} else {
					line_new += "\">";				// end first part of a
					link = false;
				}
			}
			if(letter == 'e') {              	
   				line_new += "</a>";					//end a
		    }
		    if(letter == 'b') {						//b = bigger text
				if(!h3) {
				    line_new += "<h3>";	    //h3
				    h3 = true;
				} else {
				    line_new += "</h3>";    //end h3
			    	h3 = false;
			}
		    } else if(letter == 'B') { 				//B = Bold text
				if(!b) {	    //start bold text
			   		line_new += "<b>";
			   	 	b = true;
				} else {
			    	line_new += "</b>";	//end bold text
			    	b = false;
				}
		    } else if(letter == 'd') {				//d = date
				if(!date) {
				    line_new += "<h4 id=\"date\">";  //date
			   		date = true;
				} else {
			   		line_new += "</h4>";
			    	date = false;
				    afterDate = true;
				}   
		    } else if(letter == '*') {
				line_new += "&bullet;";	    //list
		    } else if(h3) {
		        titel += letter;	//titel
		        line_new += letter;	    //whole document
		    } else if(letter == '/') {
		    	line_new += "/"; 		//if slash appears twice, there is only written one slash
		    } else {
				line_new += letter;
		    }
		} else {
			line_new += letter;
		}
		slash = false;
	    if(letter == '/') {
	    	slash = true;
	    }
		}
	    }
	    if(!afterDate) {
	       line_new += "\x3c\x62r\x3e\n";
	    }
	    afterDate = false;
    }
    ifs.close();
    time_t now = time(0);
    tm *ltm = localtime(&now);
    string month;
    string day;
    string hour;
/*
This is for a better handling of the files. It simply addes a 0 before a number (hour/day/month) if it is smaller than 10
In the following lines the post file is created. In it the post is written down. Later you can migrate the posts better with a script
*/
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
    string year = to_string(1900 + ltm->tm_year); //the year should be greater than 9, i think
    string niceDate = day + "/" + month + "/" + year; //for inserting into the index.html file
    string fullDate = year + month + day + hour; //full date
    ofstream ofs("posts/" + fullDate + ".html"); //file
    string complete_text = "<!DOCTYPE html><html><meta charset=\"utf-8\"><head><title>Blogpost</title><link href=\"design.css\" type=\"text/css\" rel=\"stylesheet\"></head><body><h1 class=\"title\">psittacus programming Blog</h1><div class=\"content\">\n" + line_new + "\n</div><div class=\"back\"><a href=\"http://psittacus.bplaced.net/\">Home sweet home</a></div></body></html>";
    ofs << complete_text;
    ofs.close();
    string pathToIndex = "index.html";
    string textToInsert = insertString(fullDate, titel, niceDate, pathToIndex.c_str());	//c_str() --> char*
    ofstream of(pathToIndex.c_str());
    of << textToInsert;
    of.close();
    return "posts/" + fullDate + ".html";
}
