#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

bool validPassword(int mincount, int maxcount, char ch, const string &password)
{
	int count = 0;

	for (auto testch : password) {
		if (testch == ch) {
			count++;
		}
	}

	if (count >= mincount && count <= maxcount) {
		return true;
	}
	else {
		return false;
	}
}

bool validPassword2(int pos1, int pos2, char ch, const string& password)
{
	int count = 0;

	// account for non-zero index
	pos1--;
	pos2--;

	if (password[pos1] == ch) {
		count++;
	}
	if (password[pos2] == ch) {
		count++;
	}

	if (count == 1) {
		return true;
	}
	else {
		return false;
	}
}


void day2()
{
	fstream myfile("day2_input.txt", ios_base::in);
	string str;
	int validCount = 0;

	while (getline(myfile, str)) {
		stringstream s(str); // Used for breaking words 
		int mincount;
		char dummy;
		int maxcount;
		char ch;
		string password;
		s >> mincount;
		s >> dummy;
		s >> maxcount;
		s >> ch;
		s >> dummy;
		s >> password;

		//cout << mincount << " " << maxcount << " " << ch << " " << password << endl;
		if (validPassword2(mincount, maxcount, ch, password)) {
			validCount++;
		}
	}

	cout << validCount << endl;
}