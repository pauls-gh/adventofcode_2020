#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>

using namespace std;

int index = 0;
string rpn;

void rpnInsert(char ch)
{
	//cout << "RPN insert " << ch << endl;
	rpn.push_back(ch);
}
void error(string str)
{
	cout << "Parsing error at index = " << index << " " << str << endl;
}

bool isDigit(char ch)
{
	if (ch >= '0' && ch <= '9') {
		return true;
	}
	return false;
}

char peekToken(string& str)
{
	char ch = 0;
	while (index < str.length() && str[index] == ' ') {
		index++;
	}

	if (index < str.length()) {
		ch = str[index];
	}

#if 0
	if (ch) {
		cout << "Token = " << ch << endl;
	}
	else {
		cout << "Token = EOL" << endl;
	}
#endif

	return ch;
}

void advanceToken(string& str)
{
	index++;
	//cout << "Advance index to " << index << endl;
}

// evaluate reverse polish notation using stack
int64_t evalRpn()
{
	stack<int64_t> stk;
	for (auto ch : rpn) {
		if (isDigit(ch)) {
			stk.push(ch - '0');
		}
		else if (ch == '+') {
			int64_t val1 = stk.top();
			stk.pop();
			int64_t val2 = stk.top();
			stk.pop();
			stk.push(val1 + val2);
		}
		else if (ch == '*') {
			int64_t val1 = stk.top();
			stk.pop();
			int64_t val2 = stk.top();
			stk.pop();
			stk.push(val1 * val2);
		}
	}

	return stk.top();
}
//////////////////////////////////////////////////////////
// Part 1
// BNF
//
// expr -> term [('+' | '*') term] *
// term -> '(' expr ')' | number


void expr(string &str);

// term -> '(' expr ')' | number
void term(string& str)
{
	static int level = 0;
	level++;
	//cout << ">>> term " << level << endl;

	char ch = peekToken(str);

	if (isDigit(ch)) {
		advanceToken(str);
		rpnInsert(ch);
		goto EXIT;
	}
	else if (ch == '(') {
		advanceToken(str);

		expr(str);

		// expect ')'
		ch = peekToken(str);
		if (ch) {
			if (ch == ')') {
				advanceToken(str);
			}
			else {
				error("expected )");
			}
		}
	}
	else {
		error("expected digit or (");
	}

EXIT:
	//cout << "<<< term " << level << endl;
	level--;
}

// expr -> term [('+' | '*') term] *
void expr(string& str)
{
	static int level = 0;
	level++;
	//cout << ">>> expr " << level << endl;

	term(str);

	while (1) {
		char ch = peekToken(str);
		if (ch == 0) {
			break;
		}
		if (ch == '+' || ch == '*') {
			advanceToken(str);

			term(str);

			rpnInsert(ch);
		}
		else {
			break;
		}
	}

	//cout << "<<< expr " << level << endl;
	level--;

}


int64_t evalExpr(string& str)
{
	index = 0; // global
	rpn.clear();

	expr(str);

	cout << "RPN = " << rpn << endl;
	return evalRpn();
}

////////////////////////////////////////////////////////
// Part 2
//     + higher precedence than *
// BNF
//
// expr  -> term['*' term] *
// term  -> factor['+' factor] *
// factor -> '(' expr ')' | number

void expr2(string& str);

// factor -> '(' expr ')' | number
void factor2(string& str)
{
	static int level = 0;
	level++;
	//cout << ">>> factor2 " << level << endl;

	char ch = peekToken(str);

	if (isDigit(ch)) {
		advanceToken(str);
		rpnInsert(ch);
		goto EXIT;
	}
	else if (ch == '(') {
		advanceToken(str);

		expr2(str);

		// expect ')'
		ch = peekToken(str);
		if (ch) {
			if (ch == ')') {
				advanceToken(str);
			}
			else {
				error("expected )");
			}
		}
	}
	else {
		error("expected digit or (");
	}

EXIT:
	//cout << "<<< factor2 " << level << endl;
	level--;
}

// term  -> factor['+' factor] *
void term2(string& str)
{
	static int level = 0;
	level++;
	//cout << ">>> term2 " << level << endl;

	factor2(str);

	while (1) {
		char ch = peekToken(str);
		if (ch == 0) {
			break;
		}
		if (ch == '+') {
			advanceToken(str);

			factor2(str);

			rpnInsert(ch);
		}
		else {
			break;
		}
	}

	//cout << "<<< term2 " << level << endl;
	level--;

}

// expr  -> term['*' term] *
void expr2(string& str)
{
	static int level = 0;
	level++;
	//cout << ">>> expr2 " << level << endl;

	term2(str);

	while (1) {
		char ch = peekToken(str);
		if (ch == 0) {
			break;
		}
		if (ch == '*') {
			advanceToken(str);

			term2(str);

			rpnInsert(ch);
		}
		else {
			break;
		}
	}

	//cout << "<<< expr2 " << level << endl;
	level--;

}


int64_t evalExpr2(string& str)
{
	index = 0; // global
	rpn.clear();

	expr2(str);

	cout << "RPN = " << rpn << endl;
	return evalRpn();
}


void day18()
{
	fstream myfile("day18_input.txt", ios_base::in);
	string str;

	int64_t total = 0;
	while (getline(myfile, str)) {
		cout << str << endl;
		total += evalExpr2(str);

	}

	cout << "Total = " << total << endl;
}
