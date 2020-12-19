#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

struct SeatState {
	bool bSeat = false;
	bool bOccupied = false;
};

void displaySeats(vector<vector<SeatState>>& seats)
{
	for (int i = 0; i < seats.size(); i++) {
		for (int j = 0; j < seats[i].size(); j++) {
			if (seats[i][j].bSeat) {
				if (seats[i][j].bOccupied) {
					cout << '#';
				}
				else {
					cout << 'L';
				}
			}
			else {
				cout << '.';
			}

		}
		cout << endl;
	}
	cout << endl << endl;
}


int countOccupiedSeats(vector<vector<SeatState>>& seats)
{
	int count = 0;
	for (int i = 0; i < seats.size(); i++) {
		for (int j = 0; j < seats[i].size(); j++) {
			if (seats[i][j].bOccupied) {
				count++;
			}
		}		
	}

	return count;
}

int countAdjacentOccupiedSeats(vector<vector<SeatState>>& seats, int row, int col)
{
	vector<int> offsetRow = {-1, -1, -1, 0, 0, 1, 1, 1};
	vector<int> offsetCol = { -1, 0, 1, -1, 1, -1, 0, 1 };
	int count = 0;

	for (int i = 0; i < offsetRow.size(); i++) {
		int adjRow = row + offsetRow[i];
		int adjCol = col + offsetCol[i];

		// validate 
		if (adjRow < 0 || adjRow >= seats.size()) {
			continue;
		}

		if (adjCol < 0 || adjCol >= seats[adjRow].size()) {
			continue;
		}

		if (seats[adjRow][adjCol].bOccupied) {
			count++;
		}
	}

	return count;
}

int countAdjacentOccupiedSeatsDirection(vector<vector<SeatState>>& seats, int row, int col)
{
	vector<int> offsetRow = { -1, -1, -1, 0, 0, 1, 1, 1 };
	vector<int> offsetCol = { -1, 0, 1, -1, 1, -1, 0, 1 };
	int count = 0;

	for (int i = 0; i < offsetRow.size(); i++) {

		int adjRow = row;
		int adjCol = col;

		// keep going in the same direction until a seat is found
		while (1) {

			adjRow += offsetRow[i];
			adjCol += offsetCol[i];

			// validate 
			if (adjRow < 0 || adjRow >= seats.size()) {
				break;
			}

			if (adjCol < 0 || adjCol >= seats[adjRow].size()) {
				break;
			}

			if (seats[adjRow][adjCol].bSeat) {
				if (seats[adjRow][adjCol].bOccupied) {
					count++;
				}
				break;
			}
		}
	}

	return count;
}

void run(vector<vector<SeatState>>& seats)
{
	// make copy of the seats state array
	// the rules apply simulataneously to all chairs so we can't update
	// the occupied state until all seats are updated.

	auto copySeats = seats;

	for (int i = 0; i < seats.size(); i++) {
		for (int j = 0; j < seats[i].size(); j++) {
			// rules
			if (seats[i][j].bSeat && !seats[i][j].bOccupied &&
				countAdjacentOccupiedSeats(seats, i, j) == 0) {
				copySeats[i][j].bOccupied = true;
			}
			else if (seats[i][j].bSeat && seats[i][j].bOccupied &&
				countAdjacentOccupiedSeats(seats, i, j) >= 4) {
				copySeats[i][j].bOccupied = false;
			}
		}
	}

	seats = copySeats;
}

void run2(vector<vector<SeatState>>& seats)
{
	// make copy of the seats state array
	// the rules apply simulataneously to all chairs so we can't update
	// the occupied state until all seats are updated.

	auto copySeats = seats;

	for (int i = 0; i < seats.size(); i++) {
		for (int j = 0; j < seats[i].size(); j++) {
			// rules
			if (seats[i][j].bSeat && !seats[i][j].bOccupied &&
				countAdjacentOccupiedSeatsDirection(seats, i, j) == 0) {
				copySeats[i][j].bOccupied = true;
			}
			else if (seats[i][j].bSeat && seats[i][j].bOccupied &&
				countAdjacentOccupiedSeatsDirection(seats, i, j) >= 5) {
				copySeats[i][j].bOccupied = false;
			}
		}
	}

	seats = copySeats;
}

void day11()
{
	fstream myfile("day11_input.txt", ios_base::in);
	string str;
	vector<vector<SeatState>> seats;

	while (getline(myfile, str)) {
		//cout << str << endl;
		vector<SeatState> row;
		for (auto ch : str) {
			SeatState state;
			if (ch == 'L') {
				state.bSeat = true;
			}
			row.push_back(state);
		}
		seats.push_back(row);
	}

	// part 1
	int prevCount = 0;
	int delta = 0;
	auto seats1 = seats;
	do {
		run(seats1);
		int count = countOccupiedSeats(seats1);
		delta = count - prevCount;
		prevCount = count;
	} while (delta != 0);

	cout << "Part 1 = " << countOccupiedSeats(seats1) << endl;

	// part 2
	prevCount = 0;
	delta = 0;
	auto seats2 = seats;	// initial state

	//displaySeats(seats2);
	do {
		run2(seats2);
		int count = countOccupiedSeats(seats2);
		//cout << count << endl;
		//displaySeats(seats2);

		delta = count - prevCount;
		prevCount = count;
	} while (delta != 0);

	cout << "Part 2 = " << countOccupiedSeats(seats2) << endl;
}

