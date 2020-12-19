#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdint.h>

using namespace std;

class Passport {
public:
	Passport(string keyvalues) {
		stringstream ss(keyvalues); // Used for breaking words 

		cout << keyvalues << endl;
		string key;
		string value;

		while (getline(ss, key, ':')) {
			getline(ss, value, ' ');
			//cout << "key = " << key << " value = " << value << endl;
			set_validation(key, value);
		}
	};
	~Passport() {};
	bool isValid() {
		if (byr.length() == 0 || iyr.length() == 0 || eyr.length() == 0) {
			return false;
		}
		if (hgt.length() == 0 || hclr.length() == 0 || eclr.length() == 0) {
			return false;
		}
		if (pid.length() == 0) {
			return false;
		}
		return true;
	}

	bool isValidPid(string value) {
		auto len = value.length();
		if (len != 9) {
			return false;
		}
		for (int i = 0; i < len; i++) {
			if (!(value[i] >= '0' && value[i] <= '9')) {
				return false;
			}
		}
		return true;
	}

	bool isValidColor(string value) {
		bool bValid = false;
		if (value.length() != 7) {
			return false;
		}
		if (value[0] != '#') {
			return false;
		}
		for (int i = 1; i <= 6; i++) {
			char ch = value[i];
			if (!((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f'))){
				return false;
			}

		}
		return true;
	}

	void set_validation(string key, string value) {
		if (key == "byr") {
			int v = stoi(value);
			if (v >= 1920 && v <= 2002) {
				byr = value;
			}
			return;
		}
		if (key == "iyr") {
			int v = stoi(value);
			if (v >= 2010 && v <= 2022) {
				iyr = value;
			}
			return;
		}
		if (key == "eyr") {
			int v = stoi(value);
			if (v >= 2020 && v <= 2030) {
				eyr = value;
			}
			return;
		}
		if (key == "hgt") {
			stringstream ss(value);
			int num;
			string units;
			ss >> num >> units;

			bool bValid = false;
			if (units == "cm" && (num >= 150 && num <= 193)) {
				bValid = true;
			} 
			if (units == "in" && (num >= 59 && num <= 76)) {
				bValid = true;
			}
			if (bValid) {
				hgt = value;
			}
			return;
		}
		if (key == "hcl") {
			if (isValidColor(value)) {
				hclr = value;
			}
			return;
		}
		if (key == "ecl") {
			if (value == "amb" ||
				value == "blu" ||
				value == "brn" ||
				value == "gry" ||
				value == "grn" ||
				value == "hzl" ||
				value == "oth"
				) {
				eclr = value;
			}
			return;
		}
		if (key == "pid") {
			if (isValidPid(value)) {
				pid = value;
			}
			return;
		}
		if (key == "cid") {
			cid = value;
			return;
		}
	}

	void set_novalidation(string key, string value) {
		if (key == "byr") {
			byr = value;
			return;
		}
		if (key == "iyr") {
			iyr = value;
			return;
		}
		if (key == "eyr") {
			eyr = value;
			return;
		}
		if (key == "hgt") {
			hgt = value;
			return;
		}
		if (key == "hcl") {
			hclr = value;
			return;
		}
		if (key == "ecl") {
			eclr = value;
			return;
		}
		if (key == "pid") {
			pid = value;
			return;
		}
		if (key == "cid") {
			cid = value;
			return;
		}
	}

private:
	string byr;
	string iyr;
	string eyr;
	string hgt;
	string hclr;
	string eclr;
	string pid;
	string cid;

};
void day4()
{
	fstream myfile("day4_input.txt", ios_base::in);
	string str;
	string keyvaluesStr;
	int validCount = 0;

	while (getline(myfile, str)) {
		if (str.length() == 0) {
			// found all key/values, create new object
			Passport p(keyvaluesStr);
			keyvaluesStr.clear();

			if (p.isValid()) {
				cout << "Valid" << endl;
				validCount++;
			}
			else {
				cout << "Invalid" << endl;
			}
		} else {
			if (keyvaluesStr.length() != 0) {
				keyvaluesStr += " ";
			}
			keyvaluesStr += str;
		}
	}

	if (keyvaluesStr.length() != 0) {
		Passport p(keyvaluesStr);
		keyvaluesStr.clear();

		if (p.isValid()) {
			cout << "Valid" << endl;
			validCount++;
		}
		else {
			cout << "Invalid" << endl;
		}
	}
	cout << validCount;

}