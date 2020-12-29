#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <unordered_map>

using namespace std;

// hex grid
// https://math.stackexchange.com/questions/2254655/hexagon-grid-coordinate-system
// axial coordinate system
//	(x,y)
//	west =  -1, 0
//  NW   =   0, -1
//  NE   =   1, -1
//  east =   1, 0
//  SE   =   0, 1
//  SW   =   -1, 1

vector<string> abbrDirections = { "w", "nw", "ne", "e", "se", "sw" };
vector<int> xoffset = { -1, 0, 1, 1, 0, -1 };
vector<int> yoffset = { 0, -1, -1, 0, 1, 1 };

string xytostring(int x, int y) {
	return to_string(x) + "," + to_string(y);
}

struct HexTile;
unordered_map<string, HexTile> mapHexTiles;
void insertHexTile(int x, int y);


struct HexTile {
	int x = 0;
	int y = 0;
	bool bWhite = true;

	int countBlackNeighbors(bool bCreateNewNeighbors = true) {
		int countBlack = 0;
		for (int i = 0; i < xoffset.size(); i++) {
			int nx = x + xoffset[i];
			int ny = y + yoffset[i];

			string tileid = xytostring(nx, ny);
			if (mapHexTiles.find(tileid) != mapHexTiles.end()) {
				if (mapHexTiles[tileid].bWhite == false) {
					countBlack++;
				}
			}
			else {
				// create white tile. we may need to flip this.
				if (bCreateNewNeighbors) {
					insertHexTile(nx, ny);
				}
			}
		}
		return countBlack;
	}

};

void displayHexTiles()
{
	for (auto& pair : mapHexTiles) {
		auto& key = pair.first;
		auto& tile = pair.second;
		cout << "(" << tile.x << "," << tile.y << ") white = " << tile.bWhite << endl;
	}
}

void insertHexTile(int x, int y)
{
	HexTile tile;
	tile.x = x;
	tile.y = y;
	mapHexTiles[xytostring(x, y)] = tile;
}

void processDirection(string& direction)
{
	int x = 0, y = 0;

	auto pos = 0;
	while (pos < direction.length()) {
		auto index = -1;
		for (auto i = 0; i < abbrDirections.size(); i++) {
			auto& abbr = abbrDirections[i];
			if (abbr == direction.substr(pos, abbr.length())) {
				index = i;
				pos += (int)abbr.length();
				//cout << abbr << endl;
				break;
			}
		}

		x += xoffset[index];
		y += yoffset[index];
	}

	string tileid = xytostring(x, y);
	if (mapHexTiles.find(tileid) == mapHexTiles.end()) {
		// insert new tile
		insertHexTile(x, y);
	}

	// flip color
	auto& tile = mapHexTiles[tileid];
	tile.bWhite = !tile.bWhite;
}

void processDay()
{
	vector<string> tilesToFlip;

	// process black tiles first as this may introduce new white tiles (new neighbors)
	// which will then need to be processed.
	for (auto& pair : mapHexTiles) {
		auto& tileid = pair.first;
		auto& tile = pair.second;
		
		if (tile.bWhite == false) {
			int count = tile.countBlackNeighbors();
			if (count == 0 || count > 2) {
				tilesToFlip.push_back(tileid);
			}
		}
	}

	// process white
	for (auto& pair : mapHexTiles) {
		auto& tileid = pair.first;
		auto& tile = pair.second;

		if (tile.bWhite == true) {
			int count = tile.countBlackNeighbors();
			if (count == 2) {
				tilesToFlip.push_back(tileid);
			}
		}
	}

	// flip
	for (auto& tileid : tilesToFlip) {
		HexTile& tile = mapHexTiles[tileid];
		tile.bWhite = !tile.bWhite;
	}
}

void prune()
{
	vector<string> toerase;
	for (auto& pair : mapHexTiles) {
		auto& tileid = pair.first;
		auto& tile = pair.second;
		if (tile.bWhite && (tile.countBlackNeighbors(false) == 0)) {
			toerase.push_back(tileid);
		}
	}
	for (auto& tileid : toerase) {
		mapHexTiles.erase(tileid);
	}

}

int countBlackTiles()
{
	int countBlack = 0;
	for (auto& pair : mapHexTiles) {
		auto& tileid = pair.first;
		auto& tile = pair.second;
		if (tile.bWhite == false) {
			countBlack++;
		}
	}

	return countBlack;
}

void day24()
{
	fstream myfile("day24_input.txt", ios_base::in);
	string str;
	vector<string> directions;

	while (getline(myfile, str)) {
		directions.push_back(str);
	}
	
	insertHexTile(0, 0);

	for (auto& direction : directions) {
		processDirection(direction);
	}

	displayHexTiles();

	cout << "Part 1: Number of black tiles = " << countBlackTiles() << endl;

	for (int i = 0; i < 100; i++) {
		processDay();
		prune();
		cout << "Day " << i << " " << countBlackTiles() << endl;
	}

	cout << "Part 2: Number of black tiles = " << countBlackTiles() << endl;

}