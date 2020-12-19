#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdint.h>

using namespace std;

void printmap(vector<vector<int>> &map)
{
	for (size_t i = 0; i < map.size(); i++) {
		for (size_t j = 0; j < map[i].size(); j++) {
			cout << map[i][j];
		}
		cout << endl;
	}
}

int getnumtrees(int xslope, int yslope, const vector<vector<int>> &map)
{
	int x = 0;
	int y = 0;
	auto yend = map.size();
	int numtrees = 0;

	while (1) {

		// calculate new position
		x += xslope;
		x = x % map[y].size();
		y += yslope;

		if (y >= yend) {
			break;
		}
		//cout << "x = " << x << " y = " << y << endl;
		if (map[y][x] == 1) {
			numtrees++;
		}
	}

	return numtrees;
}

void day3()
{
	fstream myfile("day3_input.txt", ios_base::in);
	string str;
	vector <vector<int>> map;

	while (getline(myfile, str)) {
		stringstream s(str); // Used for breaking words 
		vector<int> line;
		for (auto ch : str) {
			int tree = (ch == '#') ? 1 : 0;
			line.push_back(tree);
		}
		map.push_back(line);
	}

	//printmap(map);

	// part1
	cout << "part1 = " << getnumtrees(3, 1, map) << endl;

	// part2
	uint64_t result = 0;
	uint64_t v1 = getnumtrees(1, 1, map);
	uint64_t v2 = getnumtrees(3, 1, map);
	uint64_t v3 = getnumtrees(5, 1, map);
	uint64_t v4 = getnumtrees(7, 1, map);
	uint64_t v5 = getnumtrees(1, 2, map);

	result = v1 * v2 * v3 * v4 * v5;
	cout << "part2" << endl;
	cout << v1 << endl;
	cout << v2 << endl;
	cout << v3 << endl;
	cout << v4 << endl;
	cout << v5 << endl;
	cout << "result = " << result << endl;
}