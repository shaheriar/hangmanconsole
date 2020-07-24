#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>
#include <algorithm>

using namespace std;

string drawful = "_______\n|     |\n|     O\n|    /|\\\n|     |\n|    / \\\n|__________\n";
string drawone = "_______\n|     |\n|     O\n|    /|\\\n|     |\n|      \\\n|__________\n";
string drawtwo = "_______\n|     |\n|     O\n|    /|\\\n|      \n|       \n|__________\n";
string drawthree = "_______\n|     |\n|     O\n|     |\\\n|      \n|       \n|__________\n";
string drawfour = "_______\n|     |\n|     O\n|      \n|      \n|       \n|__________\n";
string drawfive = "_______\n|     |\n|      \n|      \n|      \n|       \n|__________\n";

char input;
string n;
string nn;
vector<string> v;
vector<string> nf;
vector<string> stage;
vector<string> dictionary;
bool done = false;
bool found = false;
int index = 0;

void setup() {
	ifstream in;
	string category;
	bool correct = false;
	int num;
	string text;
	string input;
	cout << "Choose a category: \n1. Movies\n2. General Dictionary\n3. User Defined Dictionary" << endl;
	cin >> input;
	while(!correct) {
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
				cin >> category;
				correct = true;
				break;
			default:
				correct = false; break;
		}
	}
	in.open(category);
	if (!in.is_open()) {
		cout << "Cannot find dictionary. Closing." << endl;
		exit(-1);
	}
	int i = 0;
	while(getline(in, text)) {
		if (text.size() > 3) {
			dictionary.push_back(text);
		}
	}
	stage.push_back(drawful);
	stage.push_back(drawone);
	stage.push_back(drawtwo);
	stage.push_back(drawthree);
	stage.push_back(drawfour);
	stage.push_back(drawfive);
}

bool lost(string s) {
	if (index > 4) {
		cout << endl << "Game lost" << endl;
		cout << endl << "The word was: " << s << endl;
		return true;
	}
	return false;
}

bool isdone() {
	for (int i = 0; i < v.size(); i++) {
		if (v.at(i) == "_") {
			return false;
		}
	}
	cout << endl << "Word guessed!" << endl;
	return true;
}

void print() {
	cout << stage.at(index);
	for (int i = 0; i < v.size(); i++) {
		cout << v.at(i) << ' ';
	}
	cout << endl;
	for (int j = 0; j < nf.size(); j++) {
		cout << nf.at(j) << ' ';
	}
	cout << endl;
}

void notfound(char inn) {
	string in(1, inn);
	bool f = false;
	for (int i = 0; i < nf.size(); i++) {
		if (nf.at(i) == in) {
			f = true;
		}
	}
	if (!f) {
		nf.push_back(in);
	}
}

int main() {
	bool newgame = true;
	char choice;
	setup();
	while(newgame) {
		unsigned seed = time(0);
		srand(seed);
		cout << endl << "-- H A N G  M A N --\n";
		n = dictionary.at(rand() % dictionary.size());
		nn = n;
		transform(n.begin(), n.end(), n.begin(), ::tolower);
		char r = n[rand() % n.size()];
		string rr(1, r);
		for (int i = 0; i < n.size() - 1; i++) {
			if (r == n[i]) {
				v.push_back(rr);
			} else if (n[i] == ' ' || n[i] == ':' || n[i] == '\'' || n[i] == ',' || n[i] == '-') {
				v.push_back(string(1, n[i]));
			}	else {
				v.push_back("_");
			}
		}
		print();
		while(!done) {
			cout << endl << "Guess one letter of the word: ";
			cin >> input;
			cout << endl << "==========================================" << endl;
			for (int i = 0; i < n.size(); i++) {
				if (input == n[i]) {
					cout << endl << "Letter found!" << endl;
					v.at(i) = input;
					found = true;
				}
			}
			if (!found) {
				cout << endl << "Oops! Letter not found!" << endl << endl << "Tries left: " << 4-index << endl;
				index++;
				notfound(input);
			}
			done = isdone() || lost(nn);
			found = false;
			cout << endl;
			print();
		}
		cout << endl << "New game? (Y/N): ";
		cin >> choice;
		while ((choice != 'Y' && choice != 'y') && (choice != 'N' && choice != 'n')) {
			if ((choice != 'Y' && choice != 'y') && (choice != 'N' && choice != 'n')) {
				cout << endl << "Please enter either Y or N: "; 
				cin >> choice;
				cout << endl;
			}
		}
		if (choice == 'Y' || choice == 'y') {
			newgame = true;
			v.clear();
			nf.clear();
			index = 0;
			done = false;
		}
		else if (choice == 'N' || choice == 'n') {
			break;
		}
	}
	return 0;
}