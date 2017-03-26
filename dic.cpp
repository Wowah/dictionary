#include "dic.h"
#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <clocale>
#include <typeinfo>
#include <fstream>

using namespace std;

ostream &
operator<<(ostream &s, const Dictionary &a)
{
	for (auto i = a.cards.cbegin(); i != a.cards.cend(); i++)
	{
		(*i) -> print();
	}
	return s;
}

int
cmp(WordCard *a, WordCard *b)
{
	return strcmp(a -> eng_word, b -> eng_word) < 0;
}

int
cmp1(WordCard *a, WordCard *b)
{
	return a -> rate < b -> rate;
}

WordCard::WordCard(const char *eng, const char *rus) throw():rate(0)
{
	eng_word = new char[strlen(eng) + 1];
	rus_word = new char[strlen(rus) + 1];
	for (uint i = 0; i <= strlen(eng) && i < MAX_SIZE; i++)
	{
		eng_word[i] = eng[i];
	}
	for (uint i = 0; i <= strlen(rus) && i < MAX_SIZE; i++)
	{
		rus_word[i] = rus[i];
	}
}

WordCard::WordCard(const WordCard & b) throw():rate(b.rate)
{
	eng_word = new char[strlen(b.eng_word) + 1];
	rus_word = new char[strlen(b.rus_word) + 1];
	for (uint i = 0; i <= strlen(b.eng_word) && i < MAX_SIZE; i++)
	{
		eng_word[i] = b.eng_word[i];
	}
	for (uint i = 0; i <= strlen(b.rus_word) && i < MAX_SIZE; i++)
	{
		rus_word[i] = b.rus_word[i];
	}
	rate = b.rate;
}
	
WordCard::~WordCard() throw()
{
	delete []eng_word;
	delete []rus_word;
	cout <<"~WordCard " << "\n";
}

void
WordCard::print() const
{
	for (uint i = 0; i < strlen(eng_word); i++)
	{
		cout << eng_word[i];
	}
	cout << ' ';
	for (uint i = 0; i < strlen(rus_word); i++)
	{
		cout << rus_word[i];
	}
	cout << endl;
}

Noun::Noun(const char *eng, const char *rus, int g) throw():WordCard(eng,rus)
{
	g = ((g - 1) % 4) + 1;
	switch (g) {
		case 1: 
		{
			gender = m;
			break;
		}
		case 2: 
		{
			gender = f;
			break;
		}
		case 3: default:
		{
			gender = n;
			break;
		}
		case 4:
		{
			gender = p;
			break;
		}
	}
}

Noun::Noun(const Noun &b) throw():WordCard(b.eng_word,b.rus_word)
{
	gender = b.gender;
}

Noun::~Noun() throw() { cout << "~Noun " << eng_word <<"\n";};

void
Noun::print() const
{
	cout << "Noun " << rate << ' ';
	for (uint i = 0; i < strlen(eng_word); i++)
	{
		cout << eng_word[i];
	}
	cout << ' ';
	for (uint i = 0; i < strlen(rus_word); i++)
	{
		cout << rus_word[i];
	}
	cout << '\t' << gender <<' ' << endl;
}
	
Verb::Verb(const char *eng, const char *rus, int y) throw():WordCard(eng,rus)
{
	reg = y;
}

Verb::Verb(const Verb &b) throw():WordCard(b.eng_word,b.rus_word),reg(b.reg) {}	

Verb::~Verb() throw() {cout << "~Verb " << eng_word << "\n";}

void
Verb::print() const
{
	cout << "Verb " << rate << ' ';
	for (uint i = 0; i < strlen(eng_word); i++)
	{
		cout << eng_word[i];
	}
	cout << ' ';
	for (uint i = 0; i < strlen(rus_word); i++)
	{
		cout << rus_word[i];
	}
	cout << '\t' << reg << endl;
}

bool
wrdcmp(WordCard *a, WordCard *b)
{
	Noun *pn1,*pn2;
	Verb *pv1,*pv2;
	bool ans = !strcmp(typeid(a).name(),typeid(b).name());
	ans = ans && !strcmp(a -> eng_word, b -> eng_word);
	ans = ans && !strcmp(a -> rus_word, b -> rus_word);
	if (!strcmp(typeid(*a).name(),"4Noun") && !strcmp(typeid(*b).name(),"4Noun")) {
		pn1 = (Noun*)a;
		pn2 = (Noun*)b;
		ans = ans && pn1 -> gender == pn2 -> gender;
	}
	if (!strcmp(typeid(*a).name(),"4Verb") && !strcmp(typeid(*b).name(),"4Verb")) {
		pv1 = (Verb*)a;
		pv2 = (Verb*)b;
		ans = ans && pv1 -> reg == pv2 -> reg;
	}
	return ans;
}

Dictionary::Dictionary(const char *nm = "NewDict") throw()
{
	name = new char[strlen(nm) + 1];
	for (uint i = 0; i <= strlen(nm); i++)
	{
		name[i] = nm[i];
	}
}

Dictionary::Dictionary(const Dictionary &b) throw():Dictionary(b.name) {};

Dictionary::~Dictionary() throw()
{
	sort(cards.begin(),cards.end(),cmp);
	ofstream fout;
	const char *typ;
	fout.open(name);
	vector<WordCard*>::iterator it = cards.begin();
	while (it != cards.end()) {
		typ = typeid(**it).name();
		if (strcmp(typ,"8WordCard")) {
			for (uint i = 1; i < strlen(typ); i++)
			{
				fout << typ[i];
			}
			fout << ' ';
		} else {
			fout << "Other ";
		}
		fout << (*it) -> eng_word << ' ' << (*it) -> rus_word << ' ' << (*it) -> rate << ' ';
		if (!strcmp(typ,"4Noun")) {
			fout << ((Noun*) (*it)) -> gender;
		}
		if (!strcmp(typ,"4Verb")) {
			fout << ((Verb *) (*it)) -> reg;
		}
		fout << endl;
		it++;
	} 
	it = cards.begin();
	while (it != cards.end()) {
		delete *it;
		it++;
	}
	delete []name;
	fout.close();
}

Dictionary &
Dictionary::set_name(const char *nm)
{
	name = new char[strlen(nm) + 1];
	for (uint i = 0; i <= strlen(nm); i++)
	{
		name[i] = nm[i];
	}
	return *this;
}

char *
Dictionary::get_name() const
{
	char *ans = new char[strlen(name) + 1];
	for (uint i = 0; i <= strlen(name); i++)
	{
		ans[i] = name[i];
	}
	return ans;
}
	
bool
Dictionary::is_empty() const
{
	return cards.size() == 0;
}


Dictionary &
Dictionary::add_card(const char *eng, const char *rus, const char *type, int t)
{
	WordCard *nw;
	if (!strcmp(type,"Noun")) {
		nw = new Noun(eng,rus,t);
	} else if (!strcmp(type,"Verb")) {
		nw = new Verb(eng,rus,t);
	} else {
		nw = new WordCard(eng,rus);
	}
	cards.push_back(nw);
	sort(cards.begin(),cards.end(),cmp);
	return *this;
}

vector<WordCard*>::const_iterator 
Dictionary::find(const char *word, const char *lang = "All") const
{
	for (auto i = cards.cbegin(); i != cards.cend(); i++)
	{
		if (!strcmp((*i) -> eng_word,word)) {
			return i;
		}
	}
	if (!strcmp(lang,"All")) {
		for (auto i = cards.cbegin(); i != cards.cend(); i++)
		{
			if (!strcmp((*i) -> rus_word,word)) {
				return i;
			}
		}
	}
	return cards.cend();
}
	
Dictionary &
Dictionary::del_card(const char *del)
{
	vector<WordCard*>::const_iterator it = find(del);
	if (it != cards.cend()) {
		cards.erase(it);
	}
	return *this;
}

char *
Dictionary::operator[](const char *eng)
{
	vector<WordCard*>::const_iterator it = find(eng,"Eng");
	if (it == cards.cend()) {
		return NULL;
	}
	char *nw = new char[strlen((*it) -> rus_word) + 1];
	for (uint i = 0; i <= strlen((*it) -> rus_word); i++)
	{
		nw[i] = ((*it) -> rus_word)[i];
	}
	return nw;
}
	
Dictionary &
Dictionary::clear()
{
	vector<WordCard*>::iterator it1 = cards.begin();
	vector<WordCard*>::iterator it2 = it1;
	it2++;
	for (;it2 != cards.end() && it1 != cards.end(); it1++,it2++)
	{
		while (it2 != cards.end() && !strcmp((*it1) -> eng_word, (*it2) -> eng_word)) {
			delete (*it2);
			it2 = cards.erase(it2);
		}	
	}
	return *this;
}

Dictionary &
Dictionary::set_rate(const char *word, int a)
{
	vector<WordCard*>::const_iterator it;
	it = find(word,"eng");
	if (it != cards.cend()) {
		((*it) -> rate) += a;
	}
	return *this;
}

void
Dictionary::sort_by_rate()
{
	sort(cards.begin(),cards.end(),cmp1);
}

char *
Dictionary::cur_word(const char *type = "All")
{
	int start = cur_pos;
	if (!strcmp(type,"Noun")) {
		while (strcmp(typeid(*cards[cur_pos]).name(),"4Noun")) {
			cur_pos++;
			cur_pos = cur_pos % cards.size();
			if (cur_pos == start) {
				throw "Sorry. There are no objects of class 'Noun' in the dictionary\n";
			}
		}
	}
	if (!strcmp(type,"Verb")) {
		while (strcmp(typeid(*cards[cur_pos]).name(),"4Verb")) {
			cur_pos++;
			cur_pos = cur_pos % cards.size();
			if (cur_pos == start) {
				throw "Sorry. There are no objects of class 'Verb' in the dictionary\n";
			}
		}
	}
	char *ans = new char[strlen(cards[cur_pos] -> eng_word) + 1];
	for (uint i = 0; i <= strlen(cards[cur_pos] -> eng_word); i++)
	{
		ans[i] = cards[cur_pos] -> eng_word[i];
	}
	//cout << cards[cur_pos] -> eng_word << ' ' << ans << endl;
	cur_pos++;
	cur_pos = cur_pos % cards.size();
	return ans;
}
	

const Dictionary
Dictionary::operator+(const Dictionary &b)
{
	Dictionary nw("op+");
	vector<WordCard*>::const_iterator it = cards.cbegin();
	Noun *cpn;
	Verb *cpv;
	while (it != cards.cend()) {
		if (!strcmp(typeid(*(*it)).name(),"4Noun")) {
			cpn = (Noun *) (*it);
			nw.add_card(cpn -> eng_word, cpn -> rus_word, "Noun", cpn -> gender);
		} else if (!strcmp(typeid(*(*it)).name(),"4Verb")) {
			cpv = (Verb *) (*it);
			nw.add_card(cpv -> eng_word, cpv -> rus_word, "Verb", cpv -> reg);
		} else {
			nw.add_card((*it) -> eng_word, (*it) -> rus_word, "Other", 0);
		}
		it++;
	}
	it = b.cards.cbegin();
	while (it != b.cards.cend()) {
		if (!strcmp(typeid(*(*it)).name(),"4Noun")) {
			cpn = (Noun *) (*it);
			nw.add_card(cpn -> eng_word, cpn -> rus_word, "Noun", cpn -> gender);
		} else if (!strcmp(typeid(*(*it)).name(),"4Verb")) {
			cpv = (Verb *) (*it);
			nw.add_card(cpv -> eng_word, cpv -> rus_word, "Verb", cpv -> reg);
		} else {
			nw.add_card((*it) -> eng_word, (*it) -> rus_word, "Other", 0);
		}
		it++;
	}
	return nw;
}
	
const Dictionary
Dictionary::operator-(const Dictionary &b)
{
	vector<WordCard*>::const_iterator it = cards.cbegin();
	Dictionary nw;
	int l, r, m;
	Noun *cpn;
	Verb *cpv;
	while (it != cards.cend()) {
		l = -1;
		r = b.cards.size();
		while (r - l > 1) {
			m = (r + l) / 2;
			if (strcmp(b.cards[m] -> eng_word,(*it) -> eng_word) >= 0) {
				r = m;
			} else {
				l = m;
			}
		}
		if (r == (int) b.cards.size() || strcmp(b.cards[r] -> eng_word,(*it) -> eng_word)) {
			if (!strcmp(typeid(*(*it)).name(),"4Noun")) {
				cpn = (Noun *) (*it);
				nw.add_card(cpn -> eng_word, cpn -> rus_word, "Noun", cpn -> gender);
			} else if (!strcmp(typeid(*(*it)).name(),"4Verb")) {
				cpv = (Verb *) (*it);
				nw.add_card(cpv -> eng_word, cpv -> rus_word, "Verb", cpv -> reg);
			} else {
				nw.add_card((*it) -> eng_word, (*it) -> rus_word, "Other", 0);
			}
		}
		it++;
	}
	return nw;
}
	
//int
//main(void)
//{
	//Dictionary a("Dic_a");
	//a.add_card("apple","yabloko","Noun",3);
	//a.add_card("write","pisat","Verb",0);
	//a.add_card("apple","yablochko","Noun",3);
	//a.set_rate("write",1);
	//a.set_rate("write",1);
	//a.set_rate("write",1);
	//a.set_rate("write",-1);
	//cout << a;
	//Dictionary b("Dic_b");
	//b.add_card("apple","yablochochek","Noun",3);
	//b.add_card("app","prilozhenie","Noun",3);
	//b.add_card("application","prilozhenie","Verb",3);	
	//Dictionary c = a + b;
	//Dictionary d = a - b;
	//c.set_name("Dic_c");
	//cout << c;
	//cout << endl;
	//cout << d;
	//cout << "---------------------------------------------------------\n";
	//if (c["ap"] != NULL) {
		//cout << c["ap"] << endl;
	//}
	//c.clear();
	//cout << c;
	//cout << "---------------------------------------------------------\n";
	//c.set_rate("app",1);
	//c.set_rate("app",1);
	//c.set_rate("appl",-1);
	//c.set_rate("apple",-1);
	//cout << c;
	//cout << "---------------------------------------------------------\n";
	//WordCard a("ww","ww"), b = a;
	//Noun c("a","a",1),f("a","a",2);
	//Verb d("a","a",1),e("a","a",0);
	//bool ans = wrdcmp(&c,&f);
	//cout << ans << endl;
	//return 0;
//}
