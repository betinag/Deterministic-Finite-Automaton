#include <iostream>
#include <string>
using namespace std;

struct Element {
	string state1;
	char symbol;
	string state2;
	Element* link;
};

class DFA {
private:
	Element* start;
	int numberOfSymbols;
	int numberOfStates;
	int numberOfFinalStates;
	int numberOfStartingStates;
	string* startingState;
	char* alphabet;
	string* states;
	string* finalStates;
public:
	DFA();
	DFA(DFA const&);
	void setAlphabet();
	void setStates();
	void setFinal();
	void setStartingState();
	friend ostream& operator<<(ostream&, DFA const&);
	void addConfig(string, char, string);
	void addError();
	bool execute(string);
	~DFA();
	friend DFA Union(DFA const&, DFA &);
	friend DFA interSection(DFA const&, DFA const&);
	DFA complementation();
private:
	bool checkSymbol(char);
	string transFunc(string, string);
	bool checkConfig(string, char);
};
