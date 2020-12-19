#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdint.h>
#include <algorithm>

using namespace std;

int getSeatId(string str)
{
	int row = 0;
	int col = 0;

	// first 7 letters are the row
	for (int i = 0; i < 7; i++) {
		if (str[i] == 'B') {
			row |= (1 << (7 - (i + 1)));
		}
	}

	//  next 3 letters are the column
	for (int i = 7; i < 10; i++) {
		if (str[i] == 'R') {
			col |= (1 << (10 - (i + 1)));
		}
	}

	cout << "row = " << row << " col = " << col << endl;
	return row * 8 + col;
}

void day5()
{
	fstream myfile("day5_input.txt", ios_base::in);
	string str;
	int maxid = 0;
	vector <int> idArray;

	while (getline(myfile, str)) {
		cout << str << endl;

		int id = getSeatId(str);
		cout << id << endl;

		idArray.push_back(id);

		maxid = max(maxid, id);
	}

	cout << maxid << endl;

	// sort array
	sort(idArray.begin(), idArray.end());
	for (auto id : idArray) {
		cout << id << " ";
	}

	// find missing id
	int currid = idArray[0];
	int index = 0;
	while (currid == idArray[index]) {
		index++;
		currid++;
	}
	cout <<  "Missing = " << currid << endl;
	cout << endl;
}