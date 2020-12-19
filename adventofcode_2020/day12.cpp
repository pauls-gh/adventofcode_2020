#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

struct Command {
	char command;
	int value;
};

struct Position {
	double x = 0.0;
	double y = 0.0;
};

struct Direction {
	double x = 1.0;		// initial east
	double y = 0.0;
};

void displayInfo(Position &pos, Direction &dir)
{
	cout << "Position = " << pos.x << " " << pos.y << endl;
	cout << "Direction = " << dir.x << " " << dir.y << endl;
	cout << "Manhatten distance = " << abs(pos.x) + abs(pos.y) << endl;
}

void displayInfoPart2(Position& pos, Position& wpPos)
{
	cout << "Ship Position = " << pos.x << " " << pos.y << endl;
	cout << "Waypoint Position = " << wpPos.x << " " << wpPos.y << endl;
	cout << "Manhatten distance = " << abs(pos.x) + abs(pos.y) << endl;
}

void rotate(Direction& dir, int val)
{
	double rad = val * M_PI / 180.0;
	double x2 = cos(rad) * dir.x - sin(rad) * dir.y;
	double y2 = sin(rad ) * dir.x + cos(rad ) * dir.y;

	//cout << "Rotate angle = " << val << " x = " << dir.x << " y = " << dir.y;
	//cout << " new x = " << x2 << " new y = " << y2 << endl;

	dir.x = x2;
	dir.y = y2;

}

void processCommand(char command, int val, Position &pos, Direction &dir)
{
	switch (command) {
	case 'N':
		pos.y += val;
		break;
	case 'S':
		pos.y -= val;
		break;
	case 'E':
		pos.x += val;
		break;
	case 'W':
		pos.x -= val;
		break;
	case 'L':
		rotate(dir, val);		// anti clockwise (positive angle)
		break;
	case 'R':
		rotate(dir, -val);		// clockwise (negative angle)
		break;
	case 'F':
		pos.x += dir.x * val;
		pos.y += dir.y * val;
		break;
	}

	//cout << "Command = " << command << " " << val << endl;
}

void processCommandPart2(char command, int val, Position& pos, Position& wpPos)
{
	Direction dir;
	switch (command) {
	case 'N':
		wpPos.y += val;
		break;
	case 'S':
		wpPos.y -= val;
		break;
	case 'E':
		wpPos.x += val;
		break;
	case 'W':
		wpPos.x -= val;
		break;
	case 'L':
		// rotate wp around the ship position (pos)
		// note way point pos is already relative to ship position. No need to subtract ship pos.
		dir = { wpPos.x, wpPos.y};
		rotate(dir, val);		// anti clockwise (positive angle)
		wpPos.x = dir.x;
		wpPos.y = dir.y;
		break;
	case 'R':
		// rotate wp around the ship position (pos)
		// note way point pos is already relative to ship position. No need to subtract ship pos.
		dir = { wpPos.x, wpPos.y };
		rotate(dir, -val);		// clockwise (negative angle)
		wpPos.x = dir.x;
		wpPos.y = dir.y;
		break;
	case 'F':
		// wpPos is relative to ship pos
		pos.x += wpPos.x * val;
		pos.y += wpPos.y * val;
		break;
	}

	//cout << "Command = " << command << " " << val << endl;
}

void day12()
{
	fstream myfile("day12_input.txt", ios_base::in);
	string str;
	vector<Command> cmds;

	Position pos;	// default 0,0
	Direction dir{ 1.0, 0.0 };	// default facing east

	while (getline(myfile, str)) {
		//cout << str << endl;
		stringstream ss(str);

		char command;
		int val;
		ss >> command >> val;

		cmds.push_back(Command{ command, val });
	}

	// part 1
	for (auto cmd : cmds) {
		processCommand(cmd.command, cmd.value, pos, dir);
	}

	displayInfo(pos, dir);

	// part 2

	// ship.  
	// no direction is used for part 2.
	// the direction is waypoint pos - ship pos
	pos.x = 0;	// default 0,0
	pos.y = 0;
	// waypoint  (relative to ship)
	Position wpPos{ 10, 1 };	// relative to ship position

	for (auto cmd : cmds) {
		processCommandPart2(cmd.command, cmd.value, pos, wpPos);

		//cout << "Command = " << cmd.command << " " << cmd.value << endl;
		//displayInfoPart2(pos, wpPos);
	}

	displayInfoPart2(pos, wpPos);
}

