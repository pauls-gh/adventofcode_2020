#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <bitset>
#include <unordered_map>
#include <unordered_set>
#include <tuple>

using namespace std;

// Global Data structure
struct Tile;
unordered_map<int, Tile> tileMap;	// map of original tiles
unordered_map<int, int> countMap;

struct Tile {
	int id = 0;
	vector<vector<bool>> tileArray;
	vector<vector<bool>> origTileArray;

	// part 1
	uint32_t sides[4] = { 0 };			// sides of tile 0, 1, 2, 3 => top, left, bottom, right
	uint32_t flippedSides[4] = { 0 };	// flipped sides will be reversed values

	void display()
	{
		cout << "Tile " << id << endl;
		int length = (int)tileArray.size();
		for (auto& row : tileArray) {
			for (int i = (length - 1); i >= 0; i--) {
				if (row[i]) {
					cout << "#";
				}
				else {
					cout << ".";
				}
			}
			cout << endl;
		}
	}

	// check if the tile is a match at the given position
	bool match(int row, int col, vector<vector<int>> &tile2d) {
		auto length = tile2d.size();
		// top
		if ((row - 1) >= 0) {
			auto topTileId = tile2d[row - 1][col];
			if (topTileId) {
				auto& topTile = tileMap[topTileId];
				if (topTile.bottom() != top()) {
					return false;
				}
			}
		}
		// bottom
		if ((row + 1) < length) {
			auto bottomTileId = tile2d[row + 1][col];
			if (bottomTileId) {
				auto& bottomTile = tileMap[bottomTileId];
				if (bottomTile.top() != bottom()) {
					return false;
				}
			}
		}
		// left
		if ((col - 1) >= 0) {
			auto leftTileId = tile2d[row][col - 1];
			if (leftTileId) {
				auto& leftTile = tileMap[leftTileId];
				if (leftTile.right() != left()) {
					return false;
				}
			}
		}
		// right
		if ((col + 1) < length) {
			auto rightTileId = tile2d[row][col + 1];
			if (rightTileId) {
				auto& rightTile = tileMap[rightTileId];
				if (rightTile.left() != right()) {
					return false;
				}
			}
		}
		return true;
	}

	// bit0 is the right most bit
	int top() {
		return to_ulong(tileArray[0]);
	}

	// bit0 is the right most bit
	int bottom() {
		auto length = tileArray.size();
		return to_ulong(tileArray[length - 1]);
	}

	// bit 0 is the top row, last bit
	int left() {
		int left = 0;
		auto length = tileArray.size();
		for (int i = 0; i < length; i++) {
			auto val = to_ulong(tileArray[i]);
			left |= (((val & (1 << (length - 1))) >> (length - 1))) << i;
		}
		return left;
	}
	
	// bit 0 is the top row, first bit
	int right() {
		int right = 0;
		auto length = tileArray.size();
		for (int i = 0; i < length; i++) {
			auto val = to_ulong(tileArray[i]);
			right |= (val & 1) << i;
		}
		return right;
	}

	uint32_t to_ulong(vector<bool> array)
	{
		uint32_t res = 0;
		for (int i = 0; i < array.size(); i++) {
			if (array[i]) {
				res |= 1 << i;
			}
		}
		return res;
	}

	void reset() {
		tileArray = origTileArray;
	}

	// 16 total permutations (0-15)
	void transform(int perm) {
		reset();
		int flip = (perm % 4);
		int rot = perm / 4;

		for (int i = 0; i < rot; i++) {
			rotate();
		}

		switch (flip) {
		case 0:
			break;
		case 1:
			flipH();
			break;
		case 2:
			flipV();
			break;
		case 3:
			flipH();
			flipV();
			break;
		}
	}

	// rotate to right 90 degrees
	//
	//		0					1
	//	1		3		=>	2		0
	//		2					3
	void rotate()
	{
		auto transformed = tileArray;

		// assume square
		auto n = tileArray.size();

		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				transformed[i][j] = tileArray[n - j - 1][i];
			}
		}

		tileArray = transformed;
	}

	void flipH()
	{
		// assume square
		auto n = tileArray.size();

		for (int row = 0; row < n; ++row) {
			for (int col = 0; col < n / 2; ++col) {
				swap(tileArray[row][col], tileArray[row][n - 1 - col]);
			}
		}
	}

	void flipV()
	{
		// assume square
		auto n = tileArray.size();

		for (int col = 0; col < n; ++col) {
			for (int row = 0; row < n / 2; ++row) {
				swap(tileArray[row][col], tileArray[n - 1 - row][col]);
			}
		}
	}
};



void displayTile2dFull(vector<vector<int>>& tile2d)
{
	auto &t = tileMap[tile2d[0][0]];
	int tileSize = (int)t.tileArray.size();

	for (auto& row : tile2d) {
		for (int tileRow = 0; tileRow < tileSize; tileRow++) {
			for (auto& tileId : row) {
				if (tileId) {
					auto& tile = tileMap[tileId];
					for (int col = tileSize - 1; col >= 0; col--) {
						if (tile.tileArray[tileRow][col]) {
							cout << "#";
						}
						else {
							cout << ".";
						}
					}
					cout << " ";
				}
				else {
					for (int col = tileSize -1; col >= 0; col-- ){
						cout << ".";
					}
					cout << " ";
				}

			}
			cout << endl;
		}
		cout << endl;
	}
	cout << endl;
}

void displayTile2d(vector<vector<int>>& tile2d)
{
	for (auto& row : tile2d) {
		for (auto& tileId : row) {
			cout << tileId << " ";
		}
		cout << endl;
	}
	cout << endl;

	displayTile2dFull(tile2d);
}

void displayTiles()
{
	for (auto &pair : tileMap) {
		auto &tile = pair.second;
		tile.display();

		cout << endl;

		cout << "Tile sides: ";
		for (auto val : tile.sides) {
			cout << hex << val << " ";
		}
		cout << endl;

		cout << "Flipped Tile sides: ";
		for (auto val : tile.flippedSides) {
			cout << val << " ";
		}
		cout << dec << endl;
	}
}

unsigned int reverseBits(unsigned int num)
{
	auto  NO_OF_BITS = sizeof(num) * 8;
	unsigned int reverse_num = 0;
	for (auto i = 0; i < NO_OF_BITS; i++)
	{
		if ((num & (1 << i)))
			reverse_num |= 1 << ((NO_OF_BITS - 1) - i);
	}
	return reverse_num;
}

void countNumbers()
{
	for (auto& pair : tileMap) {
		auto& tile = pair.second;
		for (auto val : tile.sides) {
			countMap[val]++;
		}
		for (auto val : tile.flippedSides) {
			countMap[val]++;
		}

	}
}

int countUniqueSides(Tile& tile)
{
	int count = 0;

	for (int i = 0; i < 4; i++) {
		if (countMap[tile.sides[i]] == 1 || countMap[tile.flippedSides[i]] == 1) {
			count++;
		}
#if 0
		if (countMap[tile.sides[i]] == 1) {
			cout << "Tile " << tile.id << " unique side = " << i << endl;
		}
		if (countMap[tile.flippedSides[i]] == 1) {
			cout << "Tile " << tile.id << " unique side (flipped) = " << i << endl;
		}
#endif
	}

	return count;
}

void computeTileSideValues()
{
	for (auto &pair : tileMap) {
		auto& tile = pair.second;
		auto length = tile.tileArray.size();

		// sides of tile 0, 1, 2, 3 => top, left, bottom, right
		tile.sides[0] = tile.top();
		tile.sides[1] = tile.left();
		tile.sides[2] = tile.bottom();
		tile.sides[3] = tile.right();

		for (int i = 0; i < 4; i++) {
			tile.flippedSides[i] = reverseBits(tile.sides[i]) >> (32 - length);
		}
	}
}


// compute tile layout in 2d grid
// input - tile at row 0 col 0
//		 - set of remaining tiles
// output - tile2d
//
//  0 1 2 
//  3 4 5
//  6 7 8
tuple<bool, vector<vector<int>>> matchTileRecursive( int row, int col, vector<vector<int>> tile2d,
						unordered_set<int> remainingTiles)
{
#if 1
	cout << "matchTileRecursive: row = " << row << " col = " << col << " remainingTiles = ";
	for (auto val : remainingTiles) {
		cout << val << " ";
	}
	cout << endl;
#endif

	auto n = tile2d.size();

	if (remainingTiles.empty()) {
		return make_tuple(true, tile2d);
	}

	for (auto tileId : remainingTiles) {
		auto& tile = tileMap[tileId];
		for (int perm = 0; perm < 16; perm++) {
			tile.transform(perm);
			if (tile.match(row, col, tile2d)) {

				// found match
				tile2d[row][col] = tileId;

#if 0
				cout << "Found Match" << endl;
				displayTile2d(tile2d);
#endif

				auto newTile2d = tile2d;
				auto newRemainingTiles = remainingTiles;
				newRemainingTiles.extract(tileId);

				if (newRemainingTiles.empty()) {
					return make_tuple(true, newTile2d);
				}

				col++;
				if (col >= n) {
					col = 0;
					row++;
				}
				if (row >= n) {
					cout << "Error - row exceeds length" << endl;
				}
				auto [res, resTile2d] = matchTileRecursive(row, col, newTile2d, newRemainingTiles);

				if (res) {
					return make_tuple(res, resTile2d);
				}
			}
		}
	}

	cout << "No match" << endl;
	return make_tuple(false, tile2d);
}

auto computeTileLayout2(int tileId)
{
	cout << "computeTileLayout2" << endl;

	unordered_set<int> remainingTilesSet;

	for (auto& pair : tileMap) {
		auto tid = pair.first;
		remainingTilesSet.insert(tid);
	}
	remainingTilesSet.erase(tileId);

	int n = (int) sqrt(tileMap.size());

	vector<vector<int>> tile2d;
	tile2d.resize(n, vector<int>(n, 0));
	for (auto &row : tile2d) {
		for (auto &val: row) {
			val = 0;
		}
	}

	// set starting corner tile
	tile2d[0][0] = tileId;

	auto [res, resTile2d] = matchTileRecursive(0, 1, tile2d, remainingTilesSet);

	return make_tuple(res, resTile2d);
}

// input - 4 corner tile ids
// output - tile2d
auto computeTileLayout(vector<int> cornerTiles)
{
	cout << "computeTileLayout" << endl;
	// for each corner tile and for each transformation of the corner tile
	for (auto tileId : cornerTiles) {
		auto& tile = tileMap[tileId];
		for (int i = 0; i < 16; i++) {
			tile.transform(i);
			auto [res, tile2d] = computeTileLayout2(tileId);
			//displayTile2d(tile2d);
			if (res) {
				return make_tuple(res, tile2d);
			}
		}
	}
	bool res = false;
	vector<vector<int>> tile2d;
	return make_tuple(res, tile2d);
}

void day20()
{
	fstream myfile("day20_input.txt", ios_base::in);
	string str;
	while (getline(myfile, str)) {
		//cout << str << endl;
		Tile tile;
		stringstream ss(str);
		string token;
		getline(ss, token, ' ');
		ss >> tile.id;

		while (getline(myfile, str)) {
			if (str.length() == 0) {
				break;
			}
			vector<bool> row(10, false);
			for (int i = 0; i < 10; i++) {
				if (str[i] == '#') {
					row[9 - i] = 1;
				}
			}
			tile.tileArray.push_back(row);
		}
		tile.origTileArray = tile.tileArray;
		tileMap[tile.id] = tile;
	}

	computeTileSideValues();
	countNumbers();
	//displayTiles();

	cout << "Count unique tiles" << endl;

	vector<int> cornerTiles;
	uint64_t result = 1;

	for (auto &pair : tileMap) {
		auto& tile = pair.second;
		int count = countUniqueSides(tile);
		cout << "Tile " << tile.id << " count of unique sides = " << count << endl;

		// corner tiles will have 2 unique sides
		if (count == 2) {
			result *= tile.id;
			cornerTiles.push_back(tile.id);
		}
	}

	cout << "Part 1 = " << result << endl;

	// Part 2
	cout << "Part 2" << endl;

	auto [res, tile2d] = computeTileLayout(cornerTiles);
	if (!res) {
		cout << "computeTileLayout failed" << endl;
		return;
	}

	displayTile2d(tile2d);

	// Convert Tile2d to single tile while removing borders
	Tile photo;
	auto tile2dN = tile2d.size();
	auto tileWidth = tileMap[tile2d[0][0]].tileArray.size();
	auto photoN = tile2dN * (tileWidth - 2);	// remove borders on each side
	photo.tileArray.resize(photoN, vector<bool>(photoN, false));

	for (int row = 0; row < tile2dN; row++) {
		for (int col = 0; col < tile2dN; col++) {

			auto& tile = tileMap[tile2d[row][col]];

			for (int i = 1; i < (tileWidth - 1); i++) {
				for (int j = 1; j < (tileWidth - 1); j++) {
					auto photorow = row * (tileWidth - 2) + i - 1;
					auto photocol = (tile2dN - 1 - col) * (tileWidth - 2) + j - 1;

					photo.tileArray[photorow][photocol] = tile.tileArray[i][j];
				}
			}
		}
	}
	photo.origTileArray = photo.tileArray;

	cout << "Photo" << endl;
	photo.display();

	// Check for monster
	vector<vector<bool>> monster = {
		{false , false, false, false, false, false, false, false, false, false, false , false, false, false, false, false, false, false, true, false,},
		{true , false, false, false, false, true, true, false, false, false, false , true, true, false, false, false, false, true, true, true,},
		{false , true, false, false, true, false, false, true, false, false, true , false, false, true, false, false, true, false, false, false,},
	};

	auto monsterHeight = monster.size();
	auto monsterWidth = monster[0].size();

	for (int perm = 0; perm < 16; perm++) {
		cout << "Perm = " << perm << endl;
		photo.transform(perm);

		int monsterCount = 0;

		//search for monster
		for (int row = 0; row < (photoN - monsterHeight); row++) {
			for (int col = 0; col < (photoN - monsterWidth); col++) {
				bool bFindMonster = true;

				for (int i = 0; i < monsterHeight; i++) {
					for (int j = 0; j < monsterWidth; j++) {
						if (monster[i][j] && !photo.tileArray[row + i][col + j]) {
							bFindMonster = false;
							break;
						}
					}
					if (!bFindMonster) {
						break;
					}
				}
				if (bFindMonster) {
					monsterCount++;
					//cout << "Find monster" << endl;
				}
			}
		}

		if (monsterCount) {
			cout << "Found " << monsterCount << " monsters" << endl;
			int totalOnes = 0;
			for (int i = 0; i < photoN; i++) {
				for (int j = 0; j < photoN; j++) {
					if (photo.tileArray[i][j]) {
						totalOnes++;
					}
				}
			}

			cout << "Water Roughness = " << totalOnes - (15 * monsterCount) << endl;
		}
	}

}