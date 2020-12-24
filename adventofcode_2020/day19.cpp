#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <assert.h>
#include <stack>

using namespace std;

struct Rule {
	bool bTerminal = false;
	vector<vector<int>> ruleArrays;
	char terminalChar;
};
unordered_map<int, Rule> ruleMap;

void displayRules()
{
	for (auto &pair : ruleMap) {
		int ruleNum = pair.first;
		auto &rule = pair.second;
		cout << ruleNum << " : ";
		if (rule.bTerminal) {
			cout << rule.terminalChar;
		}
		else {
			for (auto& ruleArray : rule.ruleArrays) {
				for (auto val : ruleArray) {
					cout << val << ",";
				}
				cout << "|";
			}
		}
		cout << endl;
	}
	cout << endl;
}

void parseRule(string& str)
{
	stringstream ss(str);
	string token;

	getline(ss, token, ':');
	int ruleNum = stoi(token);

	vector<string> tokens;
	while (getline(ss, token, ' ')) {
		if (token.length()) {
			tokens.push_back(token);
		}
	}

	Rule rule;
	vector<int> ruleArray;
	for (auto &tok : tokens) {
		if (tok == "|") {
			rule.ruleArrays.push_back(ruleArray);
			ruleArray.clear();
		}
		else if (tok[0] == '"') {
			// terminal
			rule.bTerminal = true;
			rule.terminalChar = tok[1];
		}
		else {
			ruleArray.push_back(stoi(tok));
		}
	}

	if (ruleArray.size()) {
		rule.ruleArrays.push_back(ruleArray);
	}

	ruleMap[ruleNum] = rule;
}

bool validateRules(const string& str, int ruleNum, int& index)
{
	//cout << "rule = " << ruleNum << " index = " << index << endl;

	auto& rule = ruleMap[ruleNum];

	if (rule.bTerminal) {
		if (rule.terminalChar == str[index]) {
			index++;
			return true;
		}
		else {
			return false;
		}
	}
	else {
		for (auto ruleArray : rule.ruleArrays) {
			bool result = true;
			int saveIndex = index;
			bool bRecursing = false;

			for (auto rn : ruleArray) {
				// if last rule was recursion and past end of string, allow it.
				if (index >= str.length()) {
					result = bRecursing;
					break;
				}
				bRecursing = (ruleNum == rn);
				result = result && validateRules(str, rn, index);
			}

			// no need to check "or" case if true here
			if (result) {
				return true;
			}

			// for "or" case, need to backup the index
			index = saveIndex;
		}
		return false;
	}
}

// recurse over rules until we hit a terminal char rule
//
bool  validateMessage(string& str)
{
	int index = 0;
	bool result = validateRules(str, 0, index);
	//cout << result << " " << str.length() << " " << index << endl;
	if (result && (str.length() == (index))) {
		return true;
	}
	return false;
}

void day19()
{
	fstream myfile("day19_input.txt", ios_base::in);
	string str;
	vector<string> messages;

	// read rules
	while (getline(myfile, str)) {
		//cout << str << endl;
		if (str.length() == 0) {
			break;
		}
		parseRule(str);
	}

	//displayRules();

	while (getline(myfile, str)) {
		//cout << str << endl;
		messages.push_back(str);
	}

	// part 1
	int countValid = 0;
	for (auto str : messages) {
		if (validateMessage(str)) {
			countValid++;
		}
	}

	cout << "Part 1 Valid Message = " << countValid << endl;

	// part 2
	
	// change rule 8 and 11
	ruleMap[8].ruleArrays.push_back({ 42, 8 });
	ruleMap[11].ruleArrays.push_back({ 42, 11, 31 });

	//displayRules();

	countValid = 0;
	for (auto str : messages) {
		if (validateMessage(str)) {
			countValid++;
		}
	}
	cout << "Part 2 Valid Message = " << countValid << endl;


}