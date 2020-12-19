#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

using namespace std;

struct bagInfo {
	int count = 0;
	string name;
};

void displayBagInfoArray(unordered_map<string, vector<bagInfo>>& map, string bagName)
{
	cout << bagName << " = ";

	vector<bagInfo> bagInfoArray = map[bagName];
	for (auto bag : bagInfoArray) {
		cout << bag.count;
		cout << bag.name << " ";
	}
	cout << endl;
}

void displayMap(unordered_map<string, vector<bagInfo>>& map)
{
	for (auto element : map) {
		string key = element.first;
		vector<bagInfo>& bagInfoArray = element.second;

		displayBagInfoArray(map, key);
	}
}

void insertMap(unordered_map<string, vector<bagInfo>> &map, vector<string>& tokenArray)
{
	string key = tokenArray[0] + " " + tokenArray[1];
	vector<bagInfo> bagInfoArray;

	for (auto i = 4; i < tokenArray.size(); i += 4) {
		string bag;
		struct bagInfo bagInfo;
		if (tokenArray[i] == "no") {
			// bag contains no other bags => skip
			continue;
		}
		bagInfo.count = stoi(tokenArray[i]);
		bagInfo.name = tokenArray[i + 1] + " " + tokenArray[i + 2];

		bagInfoArray.push_back(bagInfo);
	}

	map[key] = bagInfoArray;
}

// recursive solution (part 1)
bool queryContainsShinyGoldBag(unordered_map<string, vector<bagInfo>>& map, string key)
{
	vector<bagInfo> bagInfoArray = map[key];

	if (bagInfoArray.size() == 0) {
		return false;
	}

	bool result = false;
	for (auto bag : bagInfoArray) 
	{
		if (bag.name == "shiny gold") {
			result |= true;
		}
		else {
			result |= queryContainsShinyGoldBag(map, bag.name);
		}
	}
	return result;
}

// recursive solution (part 2)
int countBags(unordered_map<string, vector<bagInfo>>& map, string key)
{
	vector<bagInfo> bagInfoArray = map[key];

	if (bagInfoArray.size() == 0) {
		return 1;
	}

	int count = 1;
	for (auto bag : bagInfoArray)
	{
		count += bag.count * countBags(map, bag.name);
	}
	return count;
}



void day7()
{
	fstream myfile("day7_input.txt", ios_base::in);
	string str;
	unordered_map<string, vector<bagInfo>> map;

	while (getline(myfile, str)) {
		// tokenize the string
		stringstream ss(str);
		string token;
		vector<string> tokenArray;
		while (getline(ss, token, ' ')) {
			tokenArray.push_back(token);
		}

		insertMap(map, tokenArray);
	}

	//displayMap(map);

	// go through the entire map. For each bag, determine if it can hold a shiny gold bag
	int count = 0;
	for (auto element : map) {
		string key = element.first;

		if (queryContainsShinyGoldBag(map, key)) {
			count++;
		}
	}
	cout << count << endl;

	// part 2
	// this computes total number of bags.
	// the answer for the problem will be one less
	// i.e. the number of bags that the shiny bag contains = total bags - the shiny bag itself
	cout << countBags(map, "shiny gold") << endl;

}
