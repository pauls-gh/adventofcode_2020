#include <iostream>
#include <vector>
#include <unordered_map>
#include <assert.h>

using namespace std;

#if 0
In this game, the players take turns saying numbers.They begin by taking turns reading 
from a list of starting numbers(your puzzle input).
Then, each turn consists of considering the most recently spoken number :

If that was the first time the number has been spoken, the current player says 0.
Otherwise, the number had been spoken before;
the current player announces how many turns apart the number is
from when it was previously spoken.

#endif

vector<int> startingNumbers = { 14,1,17,0,3,20 };
//vector<int> startingNumbers = { 0, 3, 6 };

struct SpokenInfo {
	int count = 0;
	int index = 1;
	int prevIndex = 1;
};
// map number => last turn spoken
unordered_map<int, SpokenInfo> spokenMap;

void updateSpoken(int spoken, int turnNumber)
{
	SpokenInfo info = spokenMap[spoken];
	info.count++;
	info.prevIndex = info.index;
	info.index = turnNumber;
	spokenMap[spoken] = info;
}

int takeTurn(int turnNumber, int previousSpoken)
{
	int speak = -1;
	if (turnNumber <= startingNumbers.size()) {
		speak = startingNumbers[turnNumber - 1];
	}
	else {
		// previously spoken
		// Note - if the accessor [] create a new entry (never seen before)
		//		  it will be initialized with a count of 1 which will force
		//		  speak = 0
		SpokenInfo spokenInfo = spokenMap[previousSpoken];

		if (spokenInfo.count == 1) {
			speak = 0;
		}
		else {
			speak = turnNumber - 1 - spokenInfo.prevIndex;
		}
	}

	updateSpoken(speak, turnNumber);

	return speak;
}

int run(int maxTurns)
{
	int turnNumber = 1;
	int spoken = 0;
	int previousSpoken = -1;

	spokenMap.clear();

	while (turnNumber <= maxTurns) {
		spoken = takeTurn(turnNumber, previousSpoken);
		//cout << "Turn = " << turnNumber;
		//cout << " spoken = " << spoken << " previous spoken = " << previousSpoken << endl;

		previousSpoken = spoken;
		turnNumber++;
	}

	return spoken;
}

void day15()
{
	int spoken = run(2020);
	cout << "Part 1 - spoken = " << spoken << endl;
	assert(spoken == 387);

	spoken = run(30000000);
	cout << "Part 2 - spoken = " << spoken << endl;
	assert(spoken == 6428);

}