#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <unordered_map>

using namespace std;

struct CupGame {
	list<int> cups;
	unordered_map<int, list<int>::iterator> cupMap;	// cup label => list location
	size_t n = 0;
	int moveNum = 0;
	list<int>::iterator currentIt;
	vector<int> pickupCups;

	void init(string& labels) {
		n = labels.size();
		for (int i = 0; i < n; i++) {
			int cupLabel = stoi(labels.substr(i, 1));
			cups.push_back(cupLabel);
			// save iterator to this label
			cupMap[cupLabel] = next(cups.end(), -1);
		}
		currentIt = cups.begin();

		pickupCups.resize(3, 0);
	}

	void init2(string& labels) {
		n = 1000000;
		int labelsize = (int)labels.size();
		for (int i = 0; i < labelsize; i++) {
			int cupLabel = stoi(labels.substr(i, 1));
			cups.push_back(cupLabel);
			// save iterator to this label
			cupMap[cupLabel] = next(cups.end(), -1);
		}
		int cupLabel = labelsize + 1;
		for (int i = labelsize; i < n; i++) {
			cups.push_back(cupLabel);
			// save iterator to this label
			cupMap[cupLabel] = next(cups.end(), -1);
			cupLabel++;
		}
		currentIt = cups.begin();

		pickupCups.resize(3, 0);

	}

	void displayCups() {
		cout << "cups: ";
		for (auto& cup : cups) {
			if (cup == *currentIt) {
				cout << "(";
			}
			cout << cup;
			if (cup == *currentIt) {
				cout << ") ";
			}
			else {
				cout << " ";
			}
		}
		cout << endl;
	}

	void displayPickupCups() {
		cout << "pick up: ";
		for (auto val : pickupCups) {
			cout << val << " ";
		}
		cout << endl;
	}

	bool isPickupCup(int  label) {
		for (auto val : pickupCups) {
			if (label == val) {
				return true;
			}
		}
		return false;
	}

	int chooseDestination() {
		int destLabel = *currentIt - 1;
		if (destLabel < 1) {
			destLabel = (int)n;
		}
		while (isPickupCup(destLabel)) {
			destLabel--;
			if (destLabel < 1) {
				destLabel = (int)n;
			}
		}
		return destLabel;
	}

	void insertPickupCups(int destLabel) {

		// insert pick up cups after destLabel
		auto it = cupMap[destLabel];
		incWrap(it);
		auto newit = cups.insert(it, pickupCups.begin(), pickupCups.end());

		// update pickup cup iterators
		for (int i = 0; i < 3; i++) {
			cupMap[pickupCups[i]] = newit;
			incWrap(newit);

		}
	}

	void move() {
		moveNum++;
		if ((moveNum % 100000) == 0) {
			cout << "Move " << moveNum << endl;
		}

		//displayCups();

		// pick 3 cups clockwise of current cup
		// and delete from cup list
		pickupCups.clear();
		auto it = currentIt;
		incWrap(it);
		for (int i = 0; i < 3; i++) {
			pickupCups.push_back(*it);
			it = cups.erase(it);
			if (it == cups.end()) {
				it = cups.begin();
			}
		}

		//displayPickupCups();

		int destLabel = chooseDestination();
		//cout << "Destination: " << destLabel << endl;

		insertPickupCups(destLabel);

		incWrap(currentIt);
	}

	void incWrap(list<int>::iterator& it) {
		it++;
		if (it == cups.end()) {
			it = cups.begin();
		}
	}

	uint64_t getPart2Result() {
		auto it = cupMap[1];
		incWrap(it);
		uint64_t result = *it;
		incWrap(it);
		result *= *it;
		return result;

	}

};

void part1()
{
	//string labels =   "389125467";		// test input
	string labels = "123487596";		// puzzle input

	CupGame game;
	game.init(labels);

	for (int i = 0; i < 100; i++) {
		game.move();
	}
	game.displayCups();
}

void part2()
{
	//string labels =   "389125467";		// test input
	string labels = "123487596";		// puzzle input

	CupGame game;
	game.init2(labels);

	for (int i = 0; i < 10000000; i++) {
		game.move();
	}

	cout << game.getPart2Result() << endl;
	//game.displayCups();
}

void day23()
{
	part1();

	part2();
}