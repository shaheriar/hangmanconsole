#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>
#include <stdlib.h>
#include <algorithm>

using namespace std;

char input;
string n;
string nn;
vector<string> v;
vector<string> nf;
vector<string> stage;
vector<string> dictionary;
bool start = false;
bool done = false;
bool found = false;
int ind = 0;
int score = 0;

void setup() {
	ifstream in;
	string category;
	bool correct = false;
	int num;
	string text;
	string input;
	cout << "\n1. Movies\n2. General Dictionary\n3. User Defined Dictionary\n\nChoose a category: ";
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
	in.open(category);
	if (!in.is_open()) {
		cout << "Cannot find dictionary. Closing." << endl;
		exit(-1);
	}
	while(getline(in, text)) {
		if (text.size() > 3) {
			dictionary.push_back(text);
		}
	}
	stage.push_back("_______\n|     |\n|     O\n|    /|\\\n|     |\n|    / \\\n|__________\n");
	stage.push_back("_______\n|     |\n|     O\n|    /|\\\n|     |\n|      \\\n|__________\n");
	stage.push_back("_______\n|     |\n|     O\n|    /|\\\n|      \n|       \n|__________\n");
	stage.push_back("_______\n|     |\n|     O\n|     |\\\n|      \n|       \n|__________\n");
	stage.push_back("_______\n|     |\n|     O\n|      \n|      \n|       \n|__________\n");
	stage.push_back("_______\n|     |\n|      \n|      \n|      \n|       \n|__________\n");
}

bool lost() {
	if (ind > 4) {
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
	return true;
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

void print() {
	cout << "\n-- H A N G  M A N --\n\nScore: " << score << endl;
	cout << stage.at(ind) << endl;
	for (int i = 0; i < v.size(); i++) {
		cout << v.at(i) << ' ';
	}
	cout << endl;
	for (int j = 0; j < nf.size(); j++) {
		cout << nf.at(j) << ' ';
	}
	cout << endl;
	if (!found && start) {
		cout << endl << "Oops! Letter not found!\n\nTries left: " << 5-ind << endl;
	} else if (found && start) {
		cout << "\nLetter found!\n";
	}
	if (lost()) {
		if (score > 0) {
			score--;
		}
		cout << "\n-- G A M E  L O S T --\n\nThe word was: " << nn << "\n\nScore: " << score << endl;
	} else if (isdone()) {
		score++;
		cout << "\n-- W O R D  G U E S S E D! --\n\nScore: " << score << endl;
	}
}

int main() {
	bool newgame = true;
	unsigned seed = time(0);
	srand(seed);
	char choice;
	setup();
	while(newgame) {
		n = dictionary.at(rand() % dictionary.size());
		nn = n;
		transform(n.begin(), n.end(), n.begin(), ::tolower);
		char r = n[rand() % n.size()];
		string rr(1, r);
		for (int i = 0; i < n.size() - 1; i++) {
			if (r == n[i]) {
				v.push_back(string(1, nn[i]));
			} else if (n[i] == ' ' || n[i] == ':' || n[i] == '\'' || n[i] == ',' || n[i] == '-' || n[i] == '.') {
				v.push_back(string(1, n[i]));
			}	else {
				v.push_back("_");
			}
		}
		while(!done) {
			system("clear"); //remove this or change this to "cls" if using windows console
			print();
			cout << endl << "Guess one letter of the word: ";
			found = false;
			cin >> input;
			for (int i = 0; i < n.size(); i++) {
				if (input == n[i]) {
					v.at(i) = nn[i];
					found = true;
				}
			}
			if (!found) {
				ind++;
				notfound(input);
			}
			done = isdone() || lost();
			cout << endl;
			start = true;
		}
		system("clear"); //remove this or change this to "cls" if using windows console
		print();
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
			ind = 0;
			done = false;
			start = false;
		}
		else if (choice == 'N' || choice == 'n') {
			break;
		}
	}
	return 0;
}
