#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include "dic.h"
#include <typeinfo>
#include <ios>

using namespace std;

class add_mode
{
	Dictionary *dic;
public:
	add_mode(const char *name) throw();
	void on();
	~add_mode() throw();
};

class train_mode
{
	Dictionary *dic;
public:
	train_mode(const char *name) throw();
	void init();
	void on();
	~train_mode() throw();
};

add_mode::add_mode(const char *name) throw()
{
	int rate, wrd_type;
	char *word = new char[MAX_SIZE];
	char *eng = new char[MAX_SIZE];
	char *rus = new char[MAX_SIZE];
	dic = new Dictionary(name);
	ifstream fin(name);
	if (fin.is_open()) {
		while (!fin.eof()) {
			fin >> word >> eng >> rus >> rate;
			if (strcmp(word,"Other")) {
				fin >> wrd_type;
			}
			dic -> add_card(eng,rus,word,wrd_type);
			dic -> set_rate(eng,rate);
		}
		fin.close();
	}
	delete []eng;
	delete []rus;
	delete []word;
	
}

void
add_mode::on()
{
bool work = true;
	bool err = false;
	char ch;
	int wrd_type;
	char *word = new char[MAX_SIZE];
	char *eng = new char[MAX_SIZE];
	char *rus = new char[MAX_SIZE];
	while (work) {
		try {
			if (!err) {
				system("clear");
			}
			cout << "Enter word type [Noun|Verb|Other]:\n";
			cin >> word;
			if (strcmp(word,"Noun") && strcmp(word,"Verb") && strcmp(word,"Other")) {
				throw "Error! Unknown word type\n";
			}
			system("clear");
			cout << "Enter english word:\n";
			cin >> eng;
			if (strlen(eng) > MAX_SIZE) {
				throw "Error! English word too long\n";
			}
			for (uint i = 0; i < strlen(eng); i++)
			{
				if (eng[i] >= 'A' && eng[i] <= 'Z') {
					eng[i] = eng[i] - 'A' + 'a';
				}
				if (eng[i] < 'a' || eng[i] > 'z') {
					throw "Error! English word can only contain ['a'..'z'|'A'..'Z'] symbols\n";
				}
			}
			system("clear");
			cout << "Enter russian word:\n";
			cin >> rus;
			if (strlen(rus) > MAX_SIZE) {
				throw "Error! Russian word too long\n";
			}
			if (strcmp(word,"Other")) {
				cout << "Enter number for english word\n";
				cout << "for Noun (1 - masculine; 2 - feminine; 3 - neuter; 4 - plural)\n";
				cout << "for Verb (1 - regular; 0 - irregular)\n";
				cin >> wrd_type;
			}
			dic -> add_card(eng,rus,word,wrd_type);
			system("clear");
			cout << "New word was insert. Do you want continue? [Y/n]\n";
			cin >> ch;
			work = ch == 'Y' || ch == 'y';
			err = false;
		}
		catch (const char *str) {
			system("clear");
			err = true;
			cout << str;
			cout << "Please, try again\n";
		}
	}
	dic -> clear();
	delete []eng;
	delete []rus;
	delete []word;
}

add_mode::~add_mode() throw()
{
	delete dic;
}

train_mode::train_mode(const char *name) throw() 
{
	dic = new Dictionary(name); 
}

void
train_mode::init()
{
	char *dic_name = dic -> get_name();
	ifstream fin(dic_name);
	delete []dic_name;
	if (fin.is_open()) {
		int rate, wrd_type;
		char *word = new char[MAX_SIZE];
		char *eng = new char[MAX_SIZE];
		char *rus = new char[MAX_SIZE];
		while (!fin.eof()) {
			fin >> word >> eng >> rus >> rate;
			if (strcmp(word,"Other")) {
				fin >> wrd_type;
			}
			dic -> add_card(eng,rus,word,wrd_type);
			dic -> set_rate(eng,rate);
		}
		if (dic -> is_empty()) {
			throw "Error! Dictionary is empty\n";
		}
		fin.close();
		dic -> clear();
		delete []word;
		delete []eng;
		delete []rus;
	} else {
		throw "Error! File can't be opened\n";
	}
}

void
train_mode::on()
{
	int score = 0;
	vector<WordCard*>::const_iterator it;
	bool work = true, err = false;
	char ch;
	WordCard *ptr;
	char *wrd_type = new char[MAX_SIZE];
	char *word = new char[MAX_SIZE];
	char *eng;
	char *type;
	char *rus = new char[MAX_SIZE];
	dic -> sort_by_rate();
	while (work) {
		try {
			if (!err) {
				system("clear");
			}
			cout << "Please, choose game mode [All|Noun|Verb]:\n";
			cin >> word;
			if (strcmp(word,"All") && strcmp(word,"Noun") && strcmp(word,"Verb")) {
				throw "Error! Unknown command.\n";
			}
			while (score != 5) {
				eng = dic -> cur_word(word);
				it = dic -> find(eng,"eng");
				type = (char *)typeid(**it).name();
				cout << "Your score: " << score << endl;
				cout << "Current word ";
				cout << "[";
				if (!strcmp(type,"4Noun")) {
					cout << "Noun";
				} else if (!strcmp(type,"4Verb")) {
					cout << "Verb";
				} else {
					cout << "Other";
				}
				cout << "]: " << eng << endl;
				cout << "Enter russian translate:\n";
				cin >> rus;
				if (!strcmp(type,"4Noun")) {
					cout << "Enter number for gender determination ";
					cout << "(1 - masculine; 2 - feminine; 3 - neuter; 4 - plural):\n";
					do {
						err = false;
						cin >> wrd_type;
						if (strlen(wrd_type) != 1 || wrd_type[0] < '1' || wrd_type[0] > '4') {
							err = true;
						}
					} while (err);
					ptr = new Noun(eng,rus,wrd_type[0] - '1' + 1);
				} else if (!strcmp(type,"4Verb")) {
					cout << "Enter number for Verb regularity determination ";
					cout << "(0 - irregular verb; 1 - regular verb) :\n";
					do {
						err = false;
						cin >> wrd_type;
						if (strlen(wrd_type) != 1 || (wrd_type[0] != '1' && wrd_type[0] > '0')) {
							err = true;
						}
					} while (err);
					ptr = new Verb(eng,rus,wrd_type[0] - '1' + 1);
				} else {
					ptr = new WordCard(eng,rus);
				}
				system("clear");
				if (wrdcmp(ptr,(*it))) {
					cout << "Excelent!\n";
					dic -> set_rate(eng,1);
					score++;
				} else {
					cout << "Don't worry. You can better!\n";
					dic -> set_rate(eng,-1);
				}
				delete []eng;
				delete ptr;
			}
			cout << "Congratulation! You win! Do you want replay? [Y|n]\n";
			cin >> ch;
			work = ch == 'Y' || ch == 'y';
			err = false;
		}
		catch(const char *a) {
			system("clear");
			cout << a;
			cout << "Please, try again\n";
			err = true;
		}
	}
	delete []word;
	delete []rus;
}

train_mode::~train_mode() throw()
{
	delete dic;
}

int
main(int argc, char **argv)
{
	if (argc < 2) {
		cout << "Error! Too few arguments\n";
		return -1;
	}
	char *cmd = new char[MAX_SIZE];
	while (1) {
		try {
			cout << "Dicitonary: " << argv[1] << endl;
			cout << "If you want to add new words to the dictionary write 'add'\n";
			cout << "If you want to test your knowledge write 'train'\n";
			cin >> cmd;
			for (uint i = 0; i < strlen(cmd); i++)
			{
				if (cmd[i] >= 'A' && cmd[i] <= 'Z') {
					cmd[i] = cmd[i] - 'A' + 'a';
				}
			}
			if (strcmp(cmd,"add") && strcmp(cmd,"train")) {
				throw 1;
			} else if (!strcmp(cmd,"add")) {		
				add_mode add(argv[1]);
				add.on();
			} else {
				train_mode train(argv[1]);
				train.init();
				train.on();
			}
			break;
		}
		catch(int) {
			system("clear");
			cout << "Error! Unknown command\n Please, try again\n";
		}
		catch(const char *a) {
			cout << a;
		}
	}
	delete []cmd;
	return 0; 
}
