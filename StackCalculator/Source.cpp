#include <algorithm>
#include <Windows.h>
#include <utility>
#include <iostream>
#include <cstdio>
#include <conio.h>
#include <time.h>
#include <string>

using namespace std;

struct Element {
	string value = "";
	Element* next = nullptr;
};

struct Node {
	int size = 0;
	Element* list = nullptr;
};

string getByIndex(struct Node* Stack, int x) {
	Element* current = Stack->list;
	if (Stack->size == 0) return "";
	for (int i = 0; i < x && x < Stack->size; ++i) current = current->next;
	return current->value;
}

void addToStack(struct Node* Stack, string x) {
	Element* current = new Element;
	current->value = x;
	current->next = Stack->list;
	Stack->size++;
	Stack->list = current;
}

void showStack(struct Node* Stack) {
	if (Stack->size == 0) cout << "Stack = { X }" << endl;
	else {
		Element* current = Stack->list;
		cout << "Stack = { " << current->value;
		current = current->next;
		for (int i = 0; i < Stack->size - 1; ++i) {
			cout << " | " << current->value;
			current = current->next;
		}
		cout << " }" << endl;
	}
}

bool deleteFromStack(struct Node* Stack) {
	if (Stack->size == 0) return false;
	Element* current = Stack->list;
	if (Stack->size == 1) Stack->list = nullptr;
	else Stack->list = current->next;
	delete current;
	Stack->size--;
	return true;
}

void clearStack(struct Node* Stack) {
	while (Stack->size != 0) deleteFromStack(Stack);
}

bool symbolsChecker(string str) {
	string pattern = "0123456789+-*/^(),";
	bool valid;
	for (char& c : str) {
		valid = false;
		for (char& p : pattern) if (c == p) {
			valid = true;
			break;
		}
		if (valid == false) return false;
	}
	return true;
}

bool bracketsChecker(string str) {
	str.erase(remove_if(str.begin(), str.end(), [](char c) { return c != '(' && c != ')'; }), str.end());
	bool Detected;
	int last;
	do {
		last = -1;
		Detected = false;
		for (int i = 0; i < str.length(); ++i)
			if (str[i] == '(') last = i;
			else if (last > -1) {
				str.erase(i, 1);
				str.erase(last, 1);
				Detected = true;
				break;
			}
	} while (Detected == true);
	if (str.length()) return false;
	return true;
}

bool duplicatesChecker(string str) {
	string pattern = "+-*/,^";
	for (int i = 0; i < str.length() - 1; ++i)
		for (char& a : pattern) if (a == str[i]) for (char& b : pattern) if (b == str[i + 1]) return false;
	return true;
}

bool pointsChecker(string str) {
	int temp = 0;
	if (str[0] == ',') return false;
	for (int i = 0; i < str.length() - 1; ++i) {
		if (str[i] == ',') temp++;
		else if (!isdigit(str[i])) temp = 0;
		if (str[i] == ',' && !isdigit(str[i + 1])) return false;
		else if (!isdigit(str[i]) && str[i + 1] == ',') return false;
		else if (temp > 1) return false;
	}
	return true;
}

bool smartChecker(string str) {
	string pattern1 = "+-*/^,";
	string pattern3 = "0123456789,";
	if (str[0] == '*' || str[0] == '/' || str[0] == '^') return false;
	if (pattern1.find(str[str.length() - 1]) != string::npos) return false;
	for (int i = 0; i < str.length() - 1; ++i) {
		if (str[i + 1] == ')') {
			if (pattern1.find(str[i]) != string::npos)return false;
		}
		else if (str[i] == ')') if (pattern3.find(str[i + 1]) != string::npos) return false;
		if (isdigit(str[i]) && str[i + 1] == '(') return false;
	}
	return true;
}

bool hasEmptyBrackets(string str) {
	if (str.find("()") != string::npos) return true;
	return false;
}

bool hasUselessBrackets(string str) {
	str.erase(remove_if(str.begin(), str.end(), [](char c) { return c != '(' && c != ')' && c != '+' && c != '-' && c != '*' && c != '/'; }), str.end());
	if (str.find("()") != string::npos) return true;
	return false;
}

double FromStackCalculator(struct Node* Stack) {
	string pattern = "+-*/^";
	Node* TempStack = new Node;
	double temp;
	while (Stack->size != 0 || TempStack->size != 1) {
		if (pattern.find(Stack->list->value) != string::npos) {
			if (getByIndex(Stack, 0) == "+") temp = stod(getByIndex(TempStack, 1)) + stod(getByIndex(TempStack, 0));
			else if (getByIndex(Stack, 0) == "-") temp = stod(getByIndex(TempStack, 1)) - stod(getByIndex(TempStack, 0));
			else if (getByIndex(Stack, 0) == "*") temp = stod(getByIndex(TempStack, 1)) * stod(getByIndex(TempStack, 0));
			else if (getByIndex(Stack, 0) == "/") temp = stod(getByIndex(TempStack, 1)) / stod(getByIndex(TempStack, 0));
			else temp = pow(stod(getByIndex(TempStack, 1)), stod(getByIndex(TempStack, 0)));
			deleteFromStack(TempStack);
			deleteFromStack(TempStack);
			deleteFromStack(Stack);
			addToStack(TempStack, to_string(temp));
		}
		else {
			addToStack(TempStack, Stack->list->value);
			deleteFromStack(Stack);
		}
	}
	return stod(TempStack->list->value);
}

int getPriority(char c) {
	if (c == '-' || c == '+') return 1;
	else if (c == '*' || c == '/') return 2;
	else if (c == '^') return 3;
	return 0;
}

void ToStackCalculator(string str, struct Node* Stack) {
	Node* TempStack = new Node;
	string x;
	while (str.length() > 0) {
		x = "";
		if (str[0] == '(') {		//==============
			addToStack(Stack, "(");	//	Part 2
			str.erase(0, 1);		//	b '('
		}							//==============
		else if (str[0] == ')') {								//==============
			while (getByIndex(Stack, 0) != "(") {				//
				addToStack(TempStack, getByIndex(Stack, 0));	// 
				deleteFromStack(Stack);							//	Part 3
			}													//	b ')'
			deleteFromStack(Stack);								//		
			str.erase(0, 1);									//
		}														//============== 
		else if (!isdigit(str[0]) && str[0] != ',') {												//==============
			x = str[0];																				//
			while (Stack->size > 0 && getPriority(getByIndex(Stack, 0)[0]) >= getPriority(x[0])) {	//
				addToStack(TempStack, getByIndex(Stack, 0));										//	Part 4
				deleteFromStack(Stack);																//	+-
			}																						//
			addToStack(Stack, x);																	//
		}																							//==============
		else {															//==============
			for (char& c : str) if (isdigit(c) || c == ',') x = x + c;	//
			else break;													//	Part 1
			addToStack(TempStack, x);									//
		}																//	123
		str.erase(0, x.length());										//
	}																	//==============
	while (Stack->size > 0) {
		addToStack(TempStack, getByIndex(Stack, 0));
		deleteFromStack(Stack);
	}
	while (TempStack->size > 0) {
		addToStack(Stack, getByIndex(TempStack, 0));
		deleteFromStack(TempStack);
	}
}

void main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	string str;
	int errors;
	Node* Stack = new Node;
	bool contin = true;

	do {
		system("cls");
		str = "";
		errors = 0;
		SetConsoleTextAttribute(hConsole, 15);
		cout << "Enter an example below:" << endl;
		SetConsoleTextAttribute(hConsole, 3);
		getline(cin, str);
		SetConsoleTextAttribute(hConsole, 15);
		str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
		for (char& a : str) if (a == '.') a = ',';
		if (str[0] == '+' || str[0] == '-') str = '0' + str;
		for (int i = 1; i < str.length(); ++i) if (str[i - 1] == '(' && (str[i] == '-' || str[i] == '+')) str.insert(i, "0");

		SetConsoleTextAttribute(hConsole, 12);

		if (!symbolsChecker(str)) {
			cout << "|Error: Not processed character found!" << endl;
			errors++;
		}
		if (!bracketsChecker(str)) {
			cout << "|Error: Incorrect brackets!" << endl;
			errors++;
		}
		if (!duplicatesChecker(str)) {
			cout << "|Error: These symbols cannot be next to each other: '\+'\, '\-'\, '\*'\, '\/'\, '\,'\!" << endl;
			errors++;
		}
		if (!pointsChecker(str)) {
			cout << "|Error: Numbers entered incorrectly!" << endl;
			errors++;
		}
		if (!smartChecker(str)) {
			cout << "|Error: Incorrect placement of signs or brackets!" << endl;
			errors++;
		}
		if (hasEmptyBrackets(str)) {
			cout << "|Error: Empty parentheses found!" << endl;
			errors++;
		}

		if (errors) {
			SetConsoleTextAttribute(hConsole, 12);
			cout << "-> End of program execution..." << endl;
		}
		else {
			SetConsoleTextAttribute(hConsole, 14);
			if (hasUselessBrackets(str)) cout << "|Warning: Extra parentheses found!" << endl;
			SetConsoleTextAttribute(hConsole, 10);
			ToStackCalculator(str, Stack);
			cout << "-> Answer: " << FromStackCalculator(Stack) << endl;
		}
		SetConsoleTextAttribute(hConsole, 15);
		cout << "Introduce another example? (y/n)";
		while (char c = _getch()) if (c == 'y' || c == 'Y') break;
		else if (c == 'n' || c == 'N') {
			contin = false;
			break;
		}
	} while (contin);
}

/*
example+

-((60+45-65)+(-2)*400-(78-54))/7+4*76   = 416
55554454+34-44*(4433-445)/(3444*566)/443*(4556-55) *(0+1)   = 55554487.0854
-5   = -5
(123*(11-10)-34495/(54))   = -515.796296296
-((60+45-65)+400-(78-54))/7+4*76   = 244.571428571

example-

+
1-
--1
1.0.1

example+-

(1)   = 1

*/