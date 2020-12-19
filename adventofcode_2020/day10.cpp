#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

const int JOLTDIFF = 3;

int countDifferences(vector<int> &outputs)
{
	int diffs[4] = { 0 };
	int currjolt = outputs[0];

	for (int i = 1; i < outputs.size(); i++) {
		int diff = outputs[i] - currjolt;
		if (diff > JOLTDIFF) {
			cout << "ERROR - jolt difference greater than 3" << endl;
			return 0;
		}
		diffs[diff]++;

		currjolt = outputs[i];
	}

	for (auto diff : diffs) {
		cout << diff << " ";
	}
	cout << endl;

	return diffs[1] * diffs[3];
}

// brute force
int countPermutationsBF(vector<int>& outputs, int index)
{
	if (index == (outputs.size() - 1)) {
		return 1;
	}
	if (index >= (outputs.size())) {
		cout << "ERROR = exceed array bounds" << endl;
	}

	int curr = outputs[index];
	int sum = 0;
	for (int i = 1; i <= JOLTDIFF; i++) {
		int j = index + i;
		if (j < outputs.size() && (outputs[j] - curr) <= JOLTDIFF) {
			sum += countPermutationsBF(outputs, j);
		}
	}

	return sum;
}


// optimized
int64_t countPermutationsOpt(vector<int>& outputs, int index, vector<int64_t>& cache)
{
	if (index == (outputs.size() - 1)) {
		return 1;
	}
	if (index >= (outputs.size())) {
		cout << "ERROR = exceed array bounds" << endl;
	}

	if (cache[index]) {
		return cache[index];
	}

	int curr = outputs[index];
	int64_t sum = 0;
	for (int i = 1; i <= JOLTDIFF; i++) {
		int j = index + i;
		if (j < outputs.size() && (outputs[j] - curr) <= JOLTDIFF) {
			sum += countPermutationsOpt(outputs, j, cache);
		}
	}

	cache[index] = sum;

	return sum;
}

void day10()
{
	fstream myfile("day10_input.txt", ios_base::in);
	string str;
	vector<int> outputs;

	outputs.push_back(0);	// needed for 2nd part

	while (getline(myfile, str)) {
		//cout << str << endl;
		outputs.push_back(stoi(str));
	}

	sort(outputs.begin(), outputs.end());

	int maxOutput = outputs[outputs.size() - 1] + JOLTDIFF;
	cout << "Highest adapter joltage rating = " << maxOutput << endl;

	outputs.push_back(maxOutput);

	// part 1
	int count = countDifferences(outputs);
	cout << "diff 1 * diff 3 = " << count << endl;

	// part 2 - brute force
	int64_t countPerm = 0;
	//countPerm = countPermutationsBF(outputs, 0);
	cout << "Number of permutations (brute force) = " << countPerm << endl;

	vector<int64_t> cache(outputs.size(), 0);
	countPerm = countPermutationsOpt(outputs, 0, cache);
	cout << "Number of permutations (optimized) = " << countPerm << endl;

}

