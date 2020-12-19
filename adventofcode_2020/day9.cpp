#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

bool isSum(vector<int64_t>& numbers, const int windowSize, int index)
{
	// this number has to the be sum of 2 numbers in the previous window
	int64_t val = numbers[index]; 

	// test all pairs.  TODO - there may be a more efficient way to do this
	for (int i = index - windowSize; i < index; i++) {
		for (int j = index - windowSize; j < index; j++) {
			if ((numbers[i] + numbers[j]) == val) {
				return true;
			}
		}
	}

	return false;
}

int64_t findConsecutive(vector<int64_t>& numbers, int64_t part1Result)
{
	for (int i = 0; i < numbers.size(); i++) {
		int64_t sum = 0;
		int64_t minNum = INT64_MAX;
		int64_t maxNum = INT64_MIN;
		for (int j = i; j < numbers.size(); j++) {
			minNum = min(minNum, numbers[j]);
			maxNum = max(maxNum, numbers[j]);

			sum += numbers[j];

			if (sum == part1Result) {
				return minNum + maxNum;
			}
			if (sum > part1Result) {
				break;
			}
		}
	}

	return 0;
}


void day9()
{
	fstream myfile("day9_input.txt", ios_base::in);
	string str;
	vector<int64_t> numbers;

	while (getline(myfile, str)) {
		//cout << str << endl;
		numbers.push_back(stoll(str));
	}

	const int windowSize = 25;

	int64_t part1Result = 0;

	for (int i = windowSize; i < numbers.size(); i++) {
		if (!isSum(numbers, windowSize, i)) {
			cout << "Part 1 = " <<  numbers[i] << endl;
			part1Result = numbers[i];
			break;
		}
	}

	// part2 - find consecutive numbers that add to part1Result
	int64_t part2Result = findConsecutive(numbers, part1Result);
	cout << "Part 2 = " << part2Result << endl;
}

