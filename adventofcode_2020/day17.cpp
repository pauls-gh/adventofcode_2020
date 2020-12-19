#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;


#if 0 

// PART 1 - 3d vectors
struct Vector3d {
	int x = 0;
	int y = 0;
	int z = 0;

	bool operator==(const Vector3d& other) const
	{
		return (x == other.x
			&& y == other.y
			&& z == other.z);
	}
};

namespace std {

	template <>
	struct hash<Vector3d>
	{
		std::size_t operator()(const Vector3d& k) const
		{
			using std::size_t;
			using std::hash;
			using std::string;

			// Compute individual hash values for first,
			// second and third and combine them using XOR
			// and bit shifting:

			return ((hash<int>()(k.x)
				^ (hash<int>()(k.y) << 1)) >> 1)
				^ (hash<int>()(k.z) << 1);
		}
	};

}

unordered_map<Vector3d, bool> map3d;

struct StateChange {
	Vector3d v;
	bool bActive = false;
};

int countActive()
{
	int active = 0;
	for (auto pair : map3d) {
		if (pair.second) {
			active++;
		}
	}
	return active;
}

// check how many neighbors are active
int checkNeighbors(const Vector3d& v)
{
	int active = 0;

	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			for (int z = -1; z <= 1; z++) {
				if (x == 0 && y == 0 && z == 0) {
					continue;
				}
				Vector3d n = { v.x + x, v.y + y, v.z + z };
				if (map3d[n]) {
					active++;
				}
			}
		}
	}
	return active;
}

vector<StateChange> checkInactiveNeighbors(const Vector3d& v)
{
	vector<StateChange> changeState;

	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			for (int z = -1; z <= 1; z++) {
				if (x == 0 && y == 0 && z == 0) {
					continue;
				}

				Vector3d n = { v.x + x, v.y + y, v.z + z };
				if (!map3d[n]) {
					// inactive neighbor
					if (checkNeighbors(n) == 3) {
						changeState.push_back({ n, true });
					}

				}
			}
		}
	}
	return changeState;
}

void run()
{
	vector<StateChange> changeState;

	for (auto m : map3d) {
		bool bActive = m.second;
		const Vector3d& v = m.first;
		if (bActive) {
			int activeCount = checkNeighbors(v);
			if (activeCount == 2 || activeCount == 3) {
				// remain active
			}
			else {
				// change from active to inactive
				changeState.push_back({ v, false });
			}

			// check its inactive neighbors
			auto changeStateInactive = checkInactiveNeighbors(v);
			for (auto sc : changeStateInactive) {
				changeState.push_back(sc);
			}
		}
	}

	// change states
	for (auto sc : changeState) {
		bool bActive = map3d[sc.v];
		map3d[sc.v] = sc.bActive;
	}
}

void day17()
{
	fstream myfile("day17_input.txt", ios_base::in);
	string str;

	// parse rules
	int y = 0;
	int z = 0;
	while (getline(myfile, str)) {
		//cout << str << endl;
		int x = 0;
		for (auto ch : str) {
			bool active = false;
			if (ch == '#') {
				active = true;
			}
			Vector3d v{ x, y, z };
			map3d[v] = active;
			x++;
		}
		y--;
	}

	cout << countActive() << endl;

	for (int cycle = 1; cycle <= 6; cycle++) {
		run();
		cout << countActive() << endl;
	}
}

#else


// PART 2 - 4d vectors
struct Vector4d {
	int x = 0;
	int y = 0;
	int z = 0;
	int w = 0;

	bool operator==(const Vector4d& other) const
	{
		return (x == other.x
			&& y == other.y
			&& z == other.z
			&& w == other.w
			);
	}
};

namespace std {

	template <>
	struct hash<Vector4d>
	{
		std::size_t operator()(const Vector4d& k) const
		{
			using std::size_t;
			using std::hash;
			using std::string;

			// Compute individual hash values for first,
			// second and third and combine them using XOR
			// and bit shifting:

			return ((hash<int>()(k.x)
				^ (hash<int>()(k.y) << 1)
				^ (hash<int>()(k.z) << 2)
				^ (hash<int>()(k.w) << 3)));

		}
	};

}

unordered_map<Vector4d, bool> map4d;

struct StateChange {
	Vector4d v;
	bool bActive = false;
};

int countActive()
{
	int active = 0;
	for (auto const &pair : map4d) {
		if (pair.second) {
			active++;
		}
	}
	return active;
}

// check how many neighbors are active
int checkNeighbors(const Vector4d& v)
{
	int active = 0;

	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			for (int z = -1; z <= 1; z++) {
				for (int w = -1; w <= 1; w++) {

					if (x == 0 && y == 0 && z == 0 && w == 0) {
						continue;
					}
					Vector4d n = { v.x + x, v.y + y, v.z + z, v.w + w };
					if (map4d[n]) {
						active++;
					}
				}
			}
		}
	}
	return active;
}

vector<StateChange> checkInactiveNeighbors(const Vector4d& v)
{
	vector<StateChange> changeState;

	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			for (int z = -1; z <= 1; z++) {
				for (int w = -1; w <= 1; w++) {

					if (x == 0 && y == 0 && z == 0 && w == 0) {
						continue;
					}

					Vector4d n = { v.x + x, v.y + y, v.z + z, v.w + w };
					if (!map4d[n]) {
						// inactive neighbor
						if (checkNeighbors(n) == 3) {
							changeState.push_back({ n, true });
						}

					}
				}
			}
		}
	}
	return changeState;
}

void run()
{
	vector<StateChange> changeState;

	for (auto &m : map4d) {
		bool bActive = m.second;
		const Vector4d& v = m.first;
		if (bActive) {
			int activeCount = checkNeighbors(v);
			if (activeCount == 2 || activeCount == 3) {
				// remain active
			}
			else {
				// change from active to inactive
				changeState.push_back({ v, false });
			}

			// check its inactive neighbors
			auto changeStateInactive = checkInactiveNeighbors(v);
			for (auto &sc : changeStateInactive) {
				changeState.push_back(sc);
			}
		}
	}

	// change states
	for (auto sc : changeState) {
		bool bActive = map4d[sc.v];
		map4d[sc.v] = sc.bActive;
	}
}

void day17()
{
	fstream myfile("day17_input.txt", ios_base::in);
	string str;

	// parse rules
	int y = 0;
	int z = 0;
	int w = 0;
	while (getline(myfile, str)) {
		//cout << str << endl;
		int x = 0;
		for (auto ch : str) {
			bool active = false;
			if (ch == '#') {
				active = true;
			}
			Vector4d v{ x, y, z, w };
			map4d[v] = active;
			x++;
		}
		y--;
	}

	cout << countActive() << endl;

	for (int cycle = 1; cycle <= 6; cycle++) {
		run();
		cout << countActive() << endl;
	}
}

#endif
