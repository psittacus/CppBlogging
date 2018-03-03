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
    					"</h4><h3><a class=\"post\" href=\"http://psittacus.bplaced.net/posts/" + 
    					text + 
    					".html\">" + 
    					titel + 
    					"</a></h3><br><hr><br>\n" + 
    					text_lower;


    return final_text;
}

//char original: old character, bool state: if it is the second time, the character appears
string Parser::changeChar(char original, bool state) {
	//test on every character
	switch(original) {
		//two slashes are one slash (before that, there has to be slash)
		case '/':
			return "/";
		//a is for the beginning of a link
		case 'a':
			//if state (here *link*) is false
			if(!state) {
				//start link
				return "<a href=\"";
			} else {
				//end link
				return "\">";
			}
		//e is for the end of a link
		case 'e':
			return "</a>";
		//b is for bigger text and especially for the title
		case 'b':
			//if state (here *h3*) is false
			if(!state) {
				return "<h3>";
			} else {
				return "</h3>";
			}
		//B is for bold text
		case 'B':
			//if state (here *b*) is false
			if(!state) {
				return "<b>";
			} else {
				return "</b>";
			}
		//the most complex one - d is for date
		case 'd':
			if(!state) {
				return "<h4 id=\"date\">"; 
			} else {
				return "</h4>";
			}
		//* is for bullet
		case '*':
			return "&bullet;";
		//if it is an other character return the original one
		default:
			return "" + original;

	}
}


string Parser::readFile(const char* path) {
    ifstream ifs(path);
    string line_old;
    string line_new;
    string title = "";
    bool h3 = false, b = false, date = false, slash = false, afterDate = true, link = false;
    
    //check if file is open
    if(ifs.is_open()) {
    	//read lines and get the content of each line
    	while(getline(ifs, line_old)) { 
    		//get every character out of the string
    		for(char letter : line_old) {
    			//check if the last character was a slash
    			if(slash) {
    				//check on every letter
    				bool state = false;
    				switch(letter) {
    					case 'a':
    						//the state boolean gets the value of the asked boolean
    						state = link;
    						//boolean gets the opposite, next time the html tag gets closed again or started again
    						link = !link;
    						break;
    					case 'b':
    						state = h3;
    						h3 = !h3;
    						break;
    					case 'B':
    						state = b;
    						b = !b;
    						break;
    					case 'd':
    						state = date;
    						date = !date;
    						//this is needed for later
    						afterDate = !afterDate;
    						break;
    					default:
    						//this does not really matter
    						state = false;
    				}
    				//the line gets the new content
    				line_new += changeChar(letter, state);
    				//after that the slash gets deactivated
    				slash = false;
    			//the check if slash is true
    			} else { //slash is not true
    				//check for slash
    				if(letter == '/') {
    					slash = true;
    				} else {
    					//the slash should not be written, so here has to be the else
    					line_new += letter;
    					//check if the h3 tag is active, so the title is generated
    					if(h3) {
    						title += letter;
    					}
    				}
    			} //end of else 

    		} //end of for loop

    		//here the break line is added. Every character was parsed and a new line should start
    		if(afterDate) {
    			line_new += "<br>\n";
    		}
    	} //end of while loop
    } //end of check if file is open

    ifs.close();
    time_t now = time(0);
    tm *ltm = localtime(&now);
    //get the times for date generation
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
   	//the year should be greater than 9, i think
    string year = to_string(1900 + ltm->tm_year);
    //for inserting into the index.html file
    string niceDate = day + "/" + month + "/" + year; 
    //full date
    string fullDate = year + month + day + hour; 
    //file
    ofstream ofs("posts/" + fullDate + ".html"); 
    //the beginning of every blogpost html file
    string complete_text = "<!DOCTYPE html><html><meta charset=\"utf-8\"><head><title>Blogpost</title><link href=\"design.css\" type=\"text/css\" rel=\"stylesheet\"></head><body><h1 class=\"title\">psittacus programming Blog</h1><div class=\"content\">\n" + line_new + "\n</div><div class=\"back\"><a href=\"http://psittacus.bplaced.net/\">Home sweet home</a></div></body></html>";
    //add to the posts
    ofs << complete_text;
    ofs.close();
    string pathToIndex = "index.html";
    string textToInsert = insertString(fullDate, title, niceDate, pathToIndex.c_str());	//c_str() --> char*
    ofstream of(pathToIndex.c_str());
    of << textToInsert;
    of.close();
    return "posts/" + fullDate + ".html";
}
