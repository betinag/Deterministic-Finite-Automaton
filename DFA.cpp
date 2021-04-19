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
	Element* tempElement = dfa.start;
	while(tempElement != NULL) {
		out << "(" << tempElement->state1 << ","<< tempElement->symbol <<"," << tempElement->state2 << ")" <<endl;
		tempElement = tempElement->link;
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
			Element* tempElement = new Element;
			tempElement->state1 = st1;
			tempElement->symbol = sym;
			tempElement->state2 = st2;
			tempElement->link = start;
			start = tempElement;
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

	Element* tempElement = df.start;
	while(tempElement != NULL) {
		addConfig(tempElement->state1, tempElement->symbol, tempElement->state2);
		tempElement = tempElement->link;
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
	Element* tempElement = start;
	while(tempElement != NULL && (sym != tempElement->symbol || tempElement->state1.compare(state) != 0)) {
		tempElement = tempElement->link;
	}

	if(tempElement == NULL) {
		return true;
	}
	else  {
		return false;
	}
}

bool DFA::execute(string input) {
	/* Looking for a starting symbol */
	Element* tempElement = start;
	string temporaryState;
	while(tempElement != NULL && (tempElement->state1.compare(startingState[0]) != 0 || tempElement->symbol != input[0])) {
		tempElement = tempElement->link;
	}

	if(tempElement == NULL) {
		return false;
	}
	else {
		temporaryState = tempElement->state2;
	}

	char c;
	int i = 1;
	int size = input.size();
	while(i < size) {
		c = input[i];
		tempElement = start;
		while(tempElement != NULL && (tempElement->state1.compare(temporaryState) != 0 || tempElement->symbol != c)) {
			tempElement = tempElement->link;
		}

		if(tempElement == NULL) {
			return false;
		}

		else {
			temporaryState = tempElement->state2;
			i++;
		}
	}

	/* Check if temporary is a final state */
	int j = 0;
	while(j < numberOfFinalStates && temporaryState.compare(finalStates[j]) != 0 ) {
		j++;
	}

	if(j == numberOfFinalStates) {
		return false;
	}

	else {
		return true;
	}
}

string DFA::transFunc(string state, string sub) {
	Element* tmpElement = start;
	string temporaryState;
	int i = 0;
	while(tmpElement != NULL && (tmpElement->state1.compare(state) != 0 || tmpElement->symbol != sub[i])) {
		tmpElement = tmpElement->link;
	}

	if(tmpElement == NULL) {
		return "error";
	}
	else {
		i++;
	}

	char c;
	int size = sub.size();
	temporaryState = tmpElement->state2;
	while(i < size) {
		c = sub[i];
		tmpElement = start;
		while((tmpElement != NULL && (tmpElement->symbol != c) || (tmpElement->state1.compare(temporaryState) != 0))) {
			tmpElement = tmpElement->link;
		}

		if(tmpElement == NULL) {
			 return "error";
		}
		else {
			temporaryState = tmpElement->state2;
			i++;
		}
	}

	return temporaryState;
}

bool DFA::checkSymbol(char sym) {
	Element* tmpElement = start;
	while(tmpElement != NULL && tmpElement->symbol != sym) {
		tmpElement = tmpElement->link;
	}
	
	if(tmpElement == NULL) { 
		return false;
	}

	return true;
}

DFA::~DFA() {
	/* Delete configurations */
	while(start != NULL) {
		Element* tmpElement = start;
		start = start->link;
		delete tmpElement;
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
	Element* tempElement = df1.start;
	while(tempElement != NULL) {
		if(!tmp1->state1.compare(df1.startingState[0])) {
			newDFA.addConfig(newDFA.startingState[0], tempElement->symbol, tempElement->state2);
		}
		tempElement = tempElement->link;
	}

	tempElement = df2.start;
	while(tempElement != NULL) {
		if(tempElement->state1.compare(df2.startingState[0]) == 0) {
			newDFA.addConfig(newDFA.startingState[0], tempElement->symbol, tempElement->state2);
		}
		tempElement = tempElement->link;
	}

	tempElement = df1.start;
	while(tempElement ! = NULL) {
		newDFA.addConfig(tempElement->state1, tempElement->symbol, tempElement->state2);
		tempElement = tempElement->link;
	}

	tempElement = df2.start;
	while(tempElement != NULL) {
		newDFA.addConfig(tempElement->state1, tempElement->symbol, tempElement->state2);
		tempElement = tempElement->link;
	}

	return newDFA;
}

DFA interSection(DFA const& d1, DFA const& d2) {
	int k = 0, i, j;
	DFA newDFA;
	newDFA.numberOfStates = d1.numberOfStates * d2.numberOfStates;
	newDFA.states = new string[newDFA.numberOfStates];
	for(i = 0; i < d1.numberOfStates; i++) {
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
	Element* tempElement1 = d1.start, tempElement2;
	while(tempElement1 != NULL) {
		tempElement2 = d2.start;
		while(tempElement2 != NULL) {   
			if(tempElement1->symbol == tempElement2->symbol) {
				newDFA.addConfig(tempElement1->state1 + tempElement2->state1, tempElement1->symbol, tempElement1->state2 + tempElement2->state2);
			}
			tempElement2=tempElement2->link;
		}

		tempElement1 = tempElement1->link;	
	}

	return newDFA;
}

DFA DFA::complementation() {
	int i, j;
	DFA newDFA;

	this->addError();
	newDFA.numberOfStartingStates = numberOfStartingStates;
	newDFA.startingState = new string[numberOfStartingStates];
	newDFA.startingState[0] = startingState[0];
	newDFA.numberOfSymbols = numberOfSymbols;
	newDFA.alphabet = new char[numberOfSymbols];
	for(i = 0; i < numberOfSymbols; i++) {
		newDFA.alphabet[i] = alphabet[i];
	}

	/* Add configurations */
	Element* tempElement = start;
	while(tempElement != NULL) {
		newDFA.addConfig(tempElement->state1, tempElement->symbol, tempElement->state2);
		tempElement = tempElement->link;
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
