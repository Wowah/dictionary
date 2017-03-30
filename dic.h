#ifndef DICTIONARY
#define DICTIONARY

#include <vector>
#include <iostream>

enum {MAX_SIZE = 41};

using namespace std;

class WordCard;

class Dictionary 
{
//public:
	char *name;
	int cur_pos = 0;
	vector<WordCard*> cards;
public:
	Dictionary(const char *nw_name) throw();
	Dictionary(const Dictionary & b) throw();
	~Dictionary() throw();
	Dictionary & set_name(const char *nw_name);
	char * get_name() const;
	bool is_empty() const;
	Dictionary & add_card(const char *eng, const char *rus, const char *type, int t);
	Dictionary & del_card(const char *word);
	vector<WordCard*>::const_iterator find(const char *word, const char *lang) const;
	char * operator[](const char *eng);
	Dictionary & clear();
	Dictionary & set_rate(const char *word, int a);
	char * cur_word(const char *type);
	void sort_by_rate();
	const Dictionary operator+(const Dictionary &b);
	const Dictionary operator-(const Dictionary &b);
	friend ostream & operator<<(ostream &s, const Dictionary &d);
};

class WordCard 
{
protected:
	char *eng_word;
	char *rus_word;
	int rate;
public:            
	WordCard(const char *eng, const char *rus) throw();
	WordCard(const WordCard & b) throw();
	virtual ~WordCard() throw();
	virtual void print() const;
	friend int cmp(WordCard *, WordCard *);
	friend int cmp1(WordCard *, WordCard *);
	friend Dictionary;
	friend bool wrdcmp(WordCard *a, WordCard *b);
};

class Noun: public WordCard 
{
	enum {m = 1,f,n,p} gender;
public:
	Noun(const char *eng, const char *rus, int g) throw();
	Noun(const Noun & b) throw();
	void print() const;
	~Noun() throw();
	friend Dictionary;
	friend bool wrdcmp(WordCard *a, WordCard *b);
};	

class Verb: public WordCard 
{
	bool reg;
public:
	Verb(const char *eng, const char *rus, int t) throw();
	Verb(const Verb & b) throw();
	void print() const;
	~Verb() throw();
	friend Dictionary;
	friend bool wrdcmp(WordCard *a, WordCard *b);
};


	
#endif		
