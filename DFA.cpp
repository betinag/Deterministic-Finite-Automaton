#include <iostream>
#include <string>
using namespace std;

#include "DFA.h"

DFA::DFA() {
	start = NULL;
	startingState = NULL;
	alphabet = NULL;
	states = NULL;
	finalStates = NULL;
	numberOfSymbols = 0;
	numberOfStates = numberOfFinalStates = 0;
	numberOfStartingStates = 1;
}

void DFA::setAlphabet() {
	cout << "Alphabet:\n";
	cout << "Number of symbols: ";
	cin >> numberOfSymbols;

	alphabet = new char[numberOfSymbols];
	for(int i = 0; i < numberOfSymbols; i++) {
		do {
			cout << i + 1 << ". ";
			cin >> alphabet[i];
		} while((alphabet[i] < 48 || alphabet[i] > 122) || (alphabet[i] > 57 && alphabet[i] < 97));
	}
}

void DFA::setStates() {
	cout << "\nStates:\n";
	cout << "Number of states: ";
	cin >> numberOfStates;

	states = new string[numberOfStates];
	for(int i = 0; i < numberOfStates; i++) {
		cin >> states[i];
	}
}

void DFA::setFinal() {
	cout << "\nFinal states:\n";
	cout << "Number of final states: ";
	cin >> numberOfFinalStates;

	finalStates = new string[numberOfFinalStates];
	for(int i = 0; i < numberOfFinalStates; i++) {
		cin >> finalStates[i];
	}
}

void DFA::setStartingState() {
	cout << "\nStarting state: ";
	startingState = new string[numberOfStartingStates];
	cin >> startingState[0];
}

ostream& operator<<(ostream& out, DFA const& dfa) {
	int i;
	out << "Automata information:\n";
	out << "1.Alphabet: " << "{";
	for(i = 0; i < dfa.numberOfSymbols; i++) {
		out << dfa.alphabet[i] << " ";
	}
	out << "}";
	out << endl;

	out << "2.States: " << "{";
	for(i = 0; i < dfa.numberOfStates; i++) {
		out << dfa.states[i] << " ";
	}
	out << "}";
	out << endl;

	out << "3. Starting state(s):";
	for(i = 0; i < dfa.numberOfStartingStates; i++) {
		out << dfa.startingState[i] << " ";
	}
	out << endl;

	out << "4.Final states: " << "{";
	for(i = 0; i < dfa.numberOfFinalStates; i++) {
		out << dfa.finalStates[i] << " ";
	}
	out << "}";
	out << endl;

	out << "5.Configurations: " << endl;
	Element* tmp = dfa.start;
	while(tmp != NULL) {
		out << "(" << tmp->state1 << ","<< tmp->symbol <<"," <<tmp->state2 << ")" <<endl;
		tmp = tmp->link;
	}
	out << endl;
	return out;
}

void DFA::addConfig(string st1, char sym, string st2) {
	if(start == NULL) {
		start = new Element;
		start->state1 = st1;
		start->symbol = sym;
		start->state2 = st2;
		start->link = NULL;
	}
	else {
		if(start != NULL) {
			Element* tmp = new Element;
			tmp->state1 = st1;
			tmp->symbol = sym;
			tmp->state2 = st2;
			tmp->link = start;
			start = tmp;
		}
	}
}

void DFA::addError() {
	string err = "err";
	for(int i = 0; i < numberOfStates; i++) {
		for(int j = 0; j < numberOfSymbols; j++) {
			if(checkConfig(states[i], alphabet[j])) {
				addConfig(states[i], alphabet[j], "err");
			}
		}
	}
}
DFA::DFA(DFA const& df) {
	int i = 0;
	this->start = NULL;
	numberOfSymbols = df.numberOfSymbols;
	numberOfStates = df.numberOfStates;
	numberOfFinalStates = df.numberOfFinalStates;
	numberOfStartingStates = df.numberOfStartingStates;

	Element* tmp = df.start;
	while(tmp != NULL) {
		addConfig(tmp->state1, tmp->symbol, tmp->state2);
		tmp = tmp->link;
	}

	startingState = new string[numberOfStartingStates];
	for(i = 0; i < numberOfStartingStates; i++) {
		startingState[i] = df.startingState[i];
	}

	alphabet = new char[numberOfSymbols];
	for(i = 0; i < numberOfSymbols; i++) {
		alphabet[i] = df.alphabet[i];
	}

	states = new string[numberOfStates];
	for(i = 0; i < numberOfStates; i++) {
		states[i] = df.states[i];
	}

	finalStates = new string[numberOfFinalStates];
	for(i = 0; i < numberOfFinalStates; i++) {
		finalStates[i] = df.finalStates[i];
	}
}

bool DFA::checkConfig(string state, char sym) {
	Element* tmp = start;
	while(tmp != NULL && (sym != tmp->symbol || tmp->state1.compare(state) != 0)) {
		tmp = tmp->link;
	}

	if(tmp == NULL) {
		return true;
	}
	else  {
		return false;
	}
}

bool DFA::execute(string input) {
	/* Looking for a starting symbol */
	Element* tmp = start;
	string temporary;
	while(tmp != NULL && (tmp->state1.compare(startingState[0]) != 0 || tmp->symbol != input[0])) {
		tmp = tmp->link;
	}

	if(tmp == NULL) {
		return false;
	}
	else {
		temporary = tmp->state2;
	}

	char c;
	int i = 1;
	int size = input.size();
	while(i < size) {
		c = input[i];
		tmp = start;
		while(tmp != NULL && (tmp->state1.compare(temporary) != 0 || tmp->symbol != c)) {
			tmp = tmp->link;
		}

		if(tmp == NULL) {
			return false;
		}

		else {
			temporary = tmp->state2;
			i++;
		}
	}

	/* Check if temporary is a final state */
	int j = 0;
	while(j < numberOfFinalStates && temporary.compare(finalStates[j]) != 0 ) {
		j++;
	}

	if(j == numberOfFinalStates) {
		return false;
	}

	else {
		return true;
	}
}
		
string DFA::transFunc(string st, string sub) {
	Element* tmp = start;
	string temporary;
	int i = 0;
	while(tmp != NULL && (tmp->state1.compare(st) != 0 || tmp->symbol != sub[i])) {
		tmp = tmp->link;
	}

	if(tmp == NULL) {
		return "error";
	}
	else {
		i++;
	}

	char c;
	int size = sub.size();
	temporary = tmp->state2;
	while(i < size) {
		c = sub[i];
		tmp = start;

		while(tmp != NULL && (tmp->symbol != c || tmp->state1.compare(temporary) != 0)) {
			tmp = tmp->link;
		}

		if(tmp == NULL) {
			 return "error";
		}
		else {
			temporary = tmp->state2;
			i++;
		}
	}

	return temporary;
}

bool DFA::checkSymbol(char sym) {
	Element* tmp = start;
	while(tmp != NULL && tmp->symbol != sym) {
		tmp = tmp->link;
	}
	
	if(tmp == NULL) { 
		return false;
	}

	return true;
}

DFA::~DFA() {
	/* Delete configurations */
	while(start != NULL) {
		Element* tmp = start;
		start = start->link;
		delete tmp;
	}

	delete[] startingState;
	delete[] alphabet;
	delete[] states;
	delete[] finalStates;
}

DFA Union(DFA const& df1, DFA& df2) {
	int i, j;
	DFA newDFA;
	newDFA.numberOfStartingStates = df1.numberOfStartingStates;
	newDFA.startingState = new string[newDFA.numberOfStartingStates];
	newDFA.startingState[0] = 's'; /* Add new starting state */

	newDFA.numberOfSymbols = df1.numberOfSymbols; /* Alphabet */
	newDFA.alphabet = new char[newDFA.numberOfSymbols];
	for(i = 0; i < newDFA.numberOfSymbols; i++) {
		newDFA.alphabet[i] = df1.alphabet[i];
	}

	newDFA.numberOfStates = df1.numberOfStates + df2.numberOfStates + 2; /* States handling */
	newDFA.states = new string[newDFA.numberOfStates];
	i = 0;
	while(i < df1.numberOfStates) {
		newDFA.states[i] = df1.states[i];
		i++;
	}

	for(j = 0; j < df2.numberOfStates; j++) {
		newDFA.states[i++] = df2.states[j];
	}

	newDFA.states[newDFA.numberOfStates-1] = newDFA.startingState[0]; /* Add new starting state */

	newDFA.numberOfFinalStates = df1.numberOfFinalStates + df2.numberOfFinalStates + 1; /* Final states handling */
	newDFA.finalStates = new string[newDFA.numberOfFinalStates];

	int k = 0;
	while(k < df1.numberOfFinalStates) {
		newDFA.finalStates[k] = df1.finalStates[k];
		k++;
	}

	for(j = 0; j < df2.numberOfFinalStates; j++) {
		newDFA.finalStates[k++] = df2.finalStates[j];
	}
	
	/* Add the new configurations */
	Element* tmp1 = df1.start;
	while(tmp1 != NULL) {
		if(!tmp1->state1.compare(df1.startingState[0])) {
			newDFA.addConfig(newDFA.startingState[0], tmp1->symbol, tmp1->state2);
		}
		tmp1 = tmp1->link;
	}

	tmp1 = df2.start;
	while(tmp1 != NULL) {
		if(tmp1->state1.compare(df2.startingState[0]) == 0) {
			newDFA.addConfig(newDFA.startingState[0], tmp1->symbol, tmp1->state2);
		}
		tmp1 = tmp1->link;
	}

	Element* temp = df1.start;
	while(temp! = NULL) {
		newDFA.addConfig(temp->state1, temp->symbol, temp->state2);
		temp = temp->link;
	}

	Element* temp2 = df2.start;
	while(temp2 != NULL) {
		newDFA.addConfig(temp2->state1, temp2->symbol, temp2->state2);
		temp2 = temp2->link;
	}

	return newDFA;

}

DFA interSection(DFA const& d1, DFA const& d2){
	DFA newDFA;
	newDFA.numberOfStates = d1.numberOfStates * d2.numberOfStates;
	newDFA.states = new string[newDFA.numberOfStates];
	int k = 0, i = 0, j;
	for(; i < d1.numberOfStates; i++) {
		for(j = 0; j < d2.numberOfStates; j++) {
			newDFA.states[k++] = d1.states[i] + d2.states[j];
		}
	}

	newDFA.numberOfSymbols = d1.numberOfSymbols;
	newDFA.alphabet = new char[newDFA.numberOfSymbols];
	for(i = 0; i < newDFA.numberOfSymbols; i++) {
		newDFA.alphabet[i] = d1.alphabet[i];
	}

	newDFA.numberOfStartingStates = 1;
	newDFA.startingState = new string[newDFA.numberOfStartingStates];
	newDFA.startingState[0] = d1.startingState[0] + d2.startingState[0];
	
	newDFA.numberOfFinalStates = d1.numberOfFinalStates * d2.numberOfFinalStates;
	newDFA.finalStates = new string[newDFA.numberOfFinalStates];
	k = 0;
	for(i = 0; i < d1.numberOfFinalStates; i++) {
		for(j = 0; j < d2.numberOfFinalStates; j++) {
			newDFA.finalStates[k++] = d1.finalStates[i] + d2.finalStates[j];
		}
	}
	
	/* Add configurations */
	Element* temp = d1.start;
	while(temp != NULL) {
		Element* temp1 = d2.start;
		while(temp1 != NULL) {   
			if(temp->symbol == temp1->symbol) {
				newDFA.addConfig(temp->state1 + temp1->state1, temp->symbol, temp->state2 + temp1->state2);
			}
			temp1=temp1->link;
		}

		temp = temp->link;	
	}

	return newDFA;
}

DFA DFA::complementation(){
	int i, j;
	this->addError();
	DFA newDFA;

	newDFA.numberOfStartingStates = numberOfStartingStates;
	newDFA.startingState = new string[numberOfStartingStates];
	newDFA.startingState[0] = startingState[0];

	newDFA.numberOfSymbols = numberOfSymbols;
	newDFA.alphabet = new char[numberOfSymbols];
	for(i = 0; i < numberOfSymbols; i++) {
		newDFA.alphabet[i] = alphabet[i];
	}

	/* Add configurations */
	Element* tmp = start;
	while(tmp != NULL) {
		newDFA.addConfig(tmp->state1, tmp->symbol, tmp->state2);
		tmp = tmp->link;
	}

	
	/* States handling */
	newDFA.numberOfStates = numberOfStates + 1;
	newDFA.states = new string[newDFA.numberOfStates];
	for(i = 0; i < numberOfStates; i++) {
		newDFA.states[i] = states[i];
	}
	newDFA.states[numberOfStates] = "err";

	/* Final states handling */
	int k = 0;
	newDFA.numberOfFinalStates = numberOfStates - numberOfFinalStates + 1;
	newDFA.finalStates = new string[newDFA.numberOfFinalStates];

	for(i = 0; i < numberOfStates; i++) {
		j = 0;
		while(j < numberOfFinalStates && states[i] != finalStates[j]) {
			j++;
		}

		if(j == numberOfFinalStates) {
			newDFA.finalStates[k++] = states[i];
		}

	}

	newDFA.finalStates[numberOfStates - numberOfFinalStates] = "err";

	return newDFA;
}

int main() {
	DFA automaton;
	int size, iter = 0;
	char symbol;
	string state1, state2;


	automaton.setAlphabet();
	automaton.setStates();
	automaton.setStartingState();
	automaton.setFinal();
	cout << automaton;

	cout << "Please insert configurations.\n";
	cout << "Configurations: ";
	cin >> size;
	while(iter < size) {
		cout << "State: ";
		cin >> state1;
		cout << "\nSymbol: ";
		cin >> symbol;
		cout<<"\nState: ";
		cin >> state2;
		cout << endl;
		automaton.addConfig(state1, symbol, state2);
		iter++;
	}

	return 0;
}
