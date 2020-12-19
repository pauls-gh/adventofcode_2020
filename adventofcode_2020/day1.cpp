
#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>

using namespace std;

// Day 1
// Find the two entries that sum to 2020; what do you get if you multiply them together?
//
// solutions
//   - brute force using two loop through array
//   - sort array. still uses two loops but can cut the loop short
//   - unordered_map  (hash table) don't need to worry about duplicate entries
//	 - counting sort - allocate array of 2020 entries

void day1()
{
	vector<int> count(2021, 0);
	fstream myfile("day1_input.txt", ios_base::in);
	int val;

	while (myfile >> val) {
		count[val]++;
	}

	// first part - two numbers sum to 2020
	// x+y = 2020
	for (int i = 1; i < 2021; i++) {
		int val1 = i;
		int val2 = 2020 - val1;
		if (count[val1] && count[val2]) {
			// found
			cout << val1 << endl;
			cout << val2 << endl;
			cout << val1 * val2 << endl;
			break;
		}
	}

	// 2nd part - three numbers sum to 2020
	// x+y+z= 2020
	//  
	bool bfound = false;
	for (int i = 1; i < 2021; i++) {
		for (int j = 1; j < 2021; j++) {

			int val1 = i;
			int val2 = j;
			int val3 = 2020 - val1 - val2;
			if (val3 >= 0 && val3 <= 2020 &&
				count[val1] && count[val2] & count[val3]) {
				// found
				cout << val1 << endl;
				cout << val2 << endl;
				cout << val3 << endl;
				cout << val1 * val2 * val3 << endl;
				bfound = true;
				break;
			}
		}
		if (bfound) {
			break;
		}
	}

}