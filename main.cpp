#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>
#include <stdlib.h>
#include <algorithm>

using namespace std;

char input;							//THE INPUT FOR THE MENU
string WordLowerCase;				//THE WORD TO GUESS BUT IN LOWERCASE
string WordActual;					//THE WORD TO GUESS
vector<string> DisplayChars;		//THE ACTUAL WORD BUT HIDDEN
vector<string> CharsNotFound;		//LIST OF WRONGLY ATTEMPTED LETTERS
vector<string> stage;				//WHAT LEVEL OF THE GAME TO DISPLAY
vector<string> dictionary;			//ENTIRE LIST OF AVAILABLE WORDS
bool start = false;					//CHECKS IF GAME HAS BEEN STARTED
bool done = false;					//CHECKS IF GAME HAS BEEN ENDED
bool found = false;					//CHECKS IF ATTEMPTED LETTER HAS BEEN FOUND
bool streak = false;				//CHECKS IF THERE IS CURRENTLY A STREAK GOING
double StreakCount = 1;				//KEEPS A MULTIPLIER OF THE STREAK
int Count = 1;						//KEEPS A COUNT OF THE STREAK
int StageIndex = 0;					//INDEX OF THE LEVEL THAT IS DISPLAYED
int score = 0;						//THE CURRENT SCORE


//SETTING UP THE GAME
void setup() {
	
	ifstream filestream;
	string category;
	bool correct = false;
	int num;
	string text;
	string input;
	
	cout << endl << "1. Movies" 
		 << endl << "2. General Dictionary"
		 << endl << "3. User Defined Dictionary"
		 << endl 
		 << endl << "Choose a category: ";
			
	while(!correct) {
		
        cin >> input;
		
		try {
			num = stoi(input); correct = true;
		} catch(const invalid_argument& error) {
			cout << "Invalid entry. Try again: ";
			cin >> input;
		}
		
		switch(num) {
			case 1:
				category = "movies.txt"; correct = true; break;
			case 2:
				category = "dictionary.txt"; correct = true; break;
			case 3:
				cout << "Enter file name (with extension): ";
				cin >> category; correct = true; break;
			default:
                cout << "Invalid entry. Try again: ";
				correct = false; break;
		}
		
	}
	
	filestream.open(category);
	
	if (!filestream.is_open()) {
		cout << "Cannot find dictionary. Closing." << endl;
		exit(-1);
	}
	
	if (category == "movies.txt") {
		while(getline(filestream, text)) {
			dictionary.push_back(text);
		}
		
	} else {
		while(getline(filestream, text)) {
			if (text.size() < 5) {
				dictionary.push_back(text);
			}
		}
	}
	
	stage.push_back("_______\n|     |\n|     O\n|    /|\\\n|     |\n|    / \\\n|__________\n");
	stage.push_back("_______\n|     |\n|     O\n|    /|\\\n|     |\n|      \\\n|__________\n");
	stage.push_back("_______\n|     |\n|     O\n|    /|\\\n|      \n|       \n|__________\n");
	stage.push_back("_______\n|     |\n|     O\n|     |\\\n|      \n|       \n|__________\n");
	stage.push_back("_______\n|     |\n|     O\n|      \n|      \n|       \n|__________\n");
	stage.push_back("_______\n|     |\n|      \n|      \n|      \n|       \n|__________\n");
	
}

//RETURNS IF GAME IS LOST
bool lost() {
	return (StageIndex > 4) ? true : false;
}

//RETURNS IF GAME IS WON
bool won() {
	for (int i = 0; i < DisplayChars.size(); i++) {
		if (DisplayChars.at(i) == "_") {
			return false;
		}
	}
	return true;
}

//CHECKS IF THE CHAR IS NOT FOUND IN THE WORD
void notfound(char c) {
	
	string CharToString(1, c);
	bool f = false;
	
	for (int i = 0; i < CharsNotFound.size(); i++) {
		if (CharsNotFound.at(i) == CharToString) {
			f = true;
		}
	}
	
	if (!f) {
		CharsNotFound.push_back(CharToString);
	}
}

//THE DISPLAY
void print() {
	
	//TITLE
	cout 
	<< endl << "-- H A N G  M A N --" << endl 
	<< endl << "Score: " << score << endl;
	
	//HANG-MAN IMAGE
	cout << stage.at(StageIndex) << endl;
	
	//THE WORD TO GUESS
	for (int i = 0; i < DisplayChars.size(); i++) {
		cout << DisplayChars.at(i) << ' ';
	}
	
	cout << endl;
	
	//WRONGLY GUESSED LETTERS
	for (int j = 0; j < CharsNotFound.size(); j++) {
		cout << CharsNotFound.at(j) << ' ';
	}
	
	cout << endl;
	
	//WINNING STREAK
	if (streak) {
		cout << endl << "Winning streak: " << Count << endl;
	}
	
	//IF LETTER IS FOUND OR NOT
	if (!found && start) {
		cout << endl << "Oops! Letter not found!" << endl;
	} else if (found && start) {
		cout << endl << "Letter found!" << endl;
	}
	
	//LOSING SCREEN
	if (lost()) {
		
		streak = false;
		StreakCount = 1;
		Count = 1;
		
		if (score > 0) { score = score - (WordLowerCase.size()*3); }
		if (score < 0) { score = 0; }
			
		cout 
		<< endl << "-- G A M E  L O S T --" << endl 
		<< endl << "The word was: " << WordActual << endl 
		<< endl << "Score: " << score << endl;
	
	//WINNING SCREEN
	} else if (won()) {
		if (streak) {
			StreakCount *= 1.2;
			Count++;
		}
		streak = true;
		score = score + (WordLowerCase.size()*StreakCount) + (5-StageIndex);
		
		cout << endl << "-- W O R D  G U E S S E D! --"   << endl 
			 << endl << "Guesses left: +" << 5-StageIndex << endl;
		
		if (streak) {
			cout << "Word size + streak bonus: +" << int(WordLowerCase.size()*StreakCount) << endl;
		} else {
			cout << "Word size: +" << WordLowerCase.size() << endl;
		}
		
		cout << endl << "Score: " << score << endl;
	}
	
	//NUMBER OF TRIES LEFT
	cout << endl << "Tries left: " << 5-StageIndex << endl;
}

//CHECKS IF CHAR IS A SPECIAL CHARACTER
bool specialchar(char c) {
	return (c == ' ' || 
			c == ':' || 
			c == '\'' || 
			c == ',' || 
			c == '-' || 
			c == '.')   ? true : false;
}

//THE ACTUAL GAME
void play(bool& newgame) {
	
	char choice;
	
	//CHOOSE A RANDOM WORD FROM THE DICTIONARY TO GUESS
	WordLowerCase = dictionary.at(rand() % dictionary.size());
	WordActual = WordLowerCase;
	
	//TURN THE WORD INTO LOWERCASE
	transform(WordLowerCase.begin(), WordLowerCase.end(), WordLowerCase.begin(), ::tolower);
	
	//CHOOSE A RANDOM STARTING LETTER AS A HINT
	char RandomChar = WordLowerCase[rand() % (WordLowerCase.size()-1)];
	
	//IF ANY LETTER IS A SPECIAL CHARACTER THEN SHOW IT
	if (specialchar(RandomChar)) {
		while (specialchar(RandomChar)) {
			RandomChar = WordLowerCase[rand() % WordLowerCase.size()];
		}
	}
	
	//DEVELOP THE DISPLAYED WORD
	for (int i = 0; i < WordLowerCase.size() - 1; i++) {
		if (RandomChar == WordLowerCase[i]) {
			DisplayChars.push_back(string(1, WordActual[i]));
		} else if (specialchar(WordLowerCase[i])) {
			DisplayChars.push_back(string(1, WordLowerCase[i]));
		} else {
			DisplayChars.push_back("_");
		}
	}
	
	while(!done) {
		
		system("clear"); //remove this or change this to "cls" if using windows console
		print();
		cout << endl << "Guess one letter of the word: ";
		found = false;
		
		//INPUT A LETTER AND CHECK IF ITS FOUND
		cin >> input;
		
		for (int i = 0; i < WordLowerCase.size(); i++) {
			if (input == WordLowerCase[i]) {
				DisplayChars.at(i) = WordActual[i];
				found = true;
			}
		}
		
		//IF NOT FOUND THEN REMOVE A BODY PART
		if (!found) {
			StageIndex++;
			notfound(input);
		}
		
		//CHECK IF THE GAME IS DONE
		done = won() || lost();
		start = true;
	}
	
	//IF GAME IS DONE THEN ASK IF USER WANTS TO START NEW GAME
	
	system("clear"); //remove this or change this to "cls" if using windows console
	print();
	cout << endl << "New game? (Y/N): ";
	cin >> choice;
	
	while ((choice != 'Y' && choice != 'y') && (choice != 'N' && choice != 'n')) {
		cout << endl << "Please enter either Y or N: "; 
		cin >> choice;
		cout << endl;
	}
	
	if (choice == 'Y' || choice == 'y') {
		newgame = true;
		DisplayChars.clear();
		CharsNotFound.clear();
		StageIndex = 0;
		done = false;
		start = false;
	}
	
	else if (choice == 'N' || choice == 'n') {
		newgame = false;
	}
	
}	

int main() {
	bool newgame = true;
	unsigned seed = time(0);
	srand(seed);
	setup();
	while(newgame) {
		play(newgame);
	}
	return 0;
}
