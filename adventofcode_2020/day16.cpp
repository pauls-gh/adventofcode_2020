#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <assert.h>

using namespace std;

struct RuleInfo {
	int low1 = 0;
	int high1 = 0;
	int low2 = 0;
	int high2 = 0;
	vector<int> positions;
	bool bConverged = false;
};

unordered_map<string, RuleInfo> ruleMap;

void displayRulesMap()
{
	for (auto &rule : ruleMap) {

		string ruleName = rule.first;
		RuleInfo &ruleInfo = rule.second;

		cout << ruleName << " " << ruleInfo.low1 << "," << ruleInfo.high1;
		cout << " " << ruleInfo.low2 << "," << ruleInfo.high2 << " ";
		cout << " Positions = ";
		for (auto pos : ruleInfo.positions) {
			cout << pos << ", ";
		}
		cout << endl;
	}
}


int validateTicket(vector<int> &ticketValues)
{
	int result = 0;
	for (auto val : ticketValues) {
		bool bValid = false;

		// make sure val appears in at least one rule
		for (auto rule : ruleMap) {
			if ((val >= rule.second.low1 && val <= rule.second.high1) ||
			(val >= rule.second.low2 && val <= rule.second.high2)) {
				bValid = true;
				break;
			}
		}

		if (!bValid) {
			result = val;
			break;
		}
	}

	return result;
}

void removePosition(int pos)
{
	for (auto &ruleInfo : ruleMap) {
		vector<int>& positions = ruleInfo.second.positions;
		if (positions.size() > 1) {

			positions.erase(std::remove(positions.begin(), positions.end(), pos), 
				positions.end());
		}
	}
}

void convergeFieldMatches()
{
	// if a field matches to multiple positions, need to converge to 1
	// find a field that has only one position match, remove this position from other fields
	// position vectors
	bool bFinished = false;
	while (!bFinished) {
		bFinished = true;
		for (auto &ruleInfo : ruleMap) {
			if (ruleInfo.second.positions.size() == 1 && !ruleInfo.second.bConverged) {
				removePosition(ruleInfo.second.positions[0]);
				ruleInfo.second.bConverged = true;
				bFinished = false;
			}
		}
	}
}

void findRuleMatchAllTickets(vector<vector<int>>& tickets, int pos)
{
	// find rule that matches all tickets at given position
	for (auto& rule : ruleMap) {
		bool bMatch = true;
		for (auto& ticket : tickets) {
			if (validateTicket(ticket) == 0) {
				int val = ticket[pos];
				if (!((val >= rule.second.low1 && val <= rule.second.high1) ||
					(val >= rule.second.low2 && val <= rule.second.high2))) {
					bMatch = false;
					break;
				}
			}
		}
		if (bMatch) {
			rule.second.positions.push_back(pos);
		}

	}
}



void day16()
{
	fstream myfile("day16_input.txt", ios_base::in);
	string str;
	vector<vector<int>> tickets;

	// parse rules
	while (getline(myfile, str)) {
		if (str.length() == 0) {
			break;
		}
		//cout << str << endl;

		stringstream ss(str);
		string ruleName;
		getline(ss, ruleName, ':');

		RuleInfo ruleInfo;
		string token;
		getline(ss, token, '-');
		ruleInfo.low1 = stoi(token);
		getline(ss, token, ' ');
		ruleInfo.high1 = stoi(token);
		
		getline(ss, token, ' ');	// skip "or"

		getline(ss, token, '-');
		ruleInfo.low2 = stoi(token);
		getline(ss, token, '-');
		ruleInfo.high2 = stoi(token);
		ruleMap[ruleName] = ruleInfo;
	}

	// parse my ticket
	getline(myfile, str);
	getline(myfile, str);
	stringstream ss(str);
	vector<int> myticketValues;

	string token;
	while (getline(ss, token, ',')) {
		myticketValues.push_back(stoi(token));
	}


	// parse nearby tickets
	getline(myfile, str);
	getline(myfile, str);

	int result = 0;
	while (getline(myfile, str)) {
		//cout << str << endl;

		stringstream ss(str);
		vector<int> ticketValues;

		string token;
		while (getline(ss, token, ',')) {
			ticketValues.push_back(stoi(token));
		}

		tickets.push_back(ticketValues);
	}
	
	for (auto ticket : tickets) {
		result += validateTicket(ticket);
	}

	cout << "Part 1 = " << result << endl;

	// Part 2
	cout << "Part 2" << endl;

	// for each field, find possible position matches (there may be more than one)
	for (int pos = 0; pos < tickets[0].size(); pos++) {
		// find a rule that matches all tickets at this position
		findRuleMatchAllTickets(tickets, pos);
	}
	// there may be multiple position matches for a particular field => converge to a single
	convergeFieldMatches();

	displayRulesMap();

	vector<string> fields = { 
		"departure location",
		"departure station",
		"departure platform",
		"departure track", 
		"departure date",
		"departure time"
	};
	int64_t result64 = 1;
	for (auto field : fields) {
		if (ruleMap[field].positions[0] >= 0) {
			result64 *= myticketValues[ruleMap[field].positions[0]];

		}
	}

	assert(result64 == 603409823791);
	cout << "Part 2 result = " << result64 << endl;

}

