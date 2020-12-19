#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int getNumYes(vector<string> groupArray)
{
	int count[26] = { 0 };
	for (auto str : groupArray) {
		for (auto ch : str) {
			count[ch - 'a']++;
		}
	}

	int total = 0;
	for (int i = 0; i < 26; i++) {
		if (count[i]) {
			total++;
		}
	}

	return total;
}

int getNumYesAll(vector<string> groupArray)
{
	unsigned int allYes = 0xFFFFFFFF;
	for (auto str : groupArray) {
		int mask = 0;
		for (auto ch : str) {
			mask |= 1 << (ch - 'a');
		}
		allYes = allYes & mask;
	}

	int total = 0;
	for (int i = 0; i < 26; i++) {
		if (allYes & (1 << i)) {
			total++;
		}
	}

	//cout << "Yes All = " << total << endl;
	return total;
}

void day6()
{
	fstream myfile("day6_input.txt", ios_base::in);
	string str;
	vector<string> groupArray;
	int sum = 0;
	int sumYesAll = 0;

	while (getline(myfile, str)) {
		cout << str << endl;
		if (str.length() == 0) {
			// end of group
			sum += getNumYes(groupArray);
			sumYesAll += getNumYesAll(groupArray);

			groupArray.clear();
		}
		else {
			groupArray.push_back(str);
		}
	}

	if (groupArray.size() != 0) {
		sum += getNumYes(groupArray);
		sumYesAll += getNumYesAll(groupArray);
	}

	cout << "Sum = " << sum << endl;
	cout << "sumYesAll = " << sumYesAll << endl;

}
