#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

struct BusInfo {
	int busId;
	int offset;
};

void displayBuses(vector<BusInfo>& buses)
{
	for (auto bus : buses) {
		cout << "Bus ID = " << bus.busId << " offset = " << bus.offset << ", ";
	}
	cout << endl;
}


// Returns modulo inverse of a with respect to m using extended 
// Euclid Algorithm. Refer below post for details: 
// https://www.geeksforgeeks.org/multiplicative-inverse-under-modulo-m/ 
int64_t inv(int64_t a, int64_t m)
{
	int64_t m0 = m, t, q;
	int64_t x0 = 0, x1 = 1;

	if (m == 1)
		return 0;

	// Apply extended Euclid Algorithm 
	while (a > 1)
	{
		// q is quotient 
		q = a / m;

		t = m;

		// m is remainder now, process same as 
		// euclid's algo 
		m = a % m, a = t;

		t = x0;

		x0 = x1 - q * x0;

		x1 = t;
	}

	// Make x1 positive 
	if (x1 < 0)
		x1 += m0;

	return x1;
}

// k is size of num[] and rem[].  Returns the smallest 
// number x such that: 
//  x % num[0] = rem[0], 
//  x % num[1] = rem[1], 
//  .................. 
//  x % num[k-2] = rem[k-1] 
// Assumption: Numbers in num[] are pairwise coprime 
// (gcd for every pair is 1) 
int64_t findMinX(vector<int64_t>& num, vector<int64_t>& rem)
{
	int64_t k = num.size();

	// Compute product of all numbers 
	int64_t prod = 1;
	for (int64_t i = 0; i < k; i++)
		prod *= num[i];

	// Initialize result 
	int64_t result = 0;

	// Apply above formula 
	for (int64_t i = 0; i < k; i++)
	{
		int64_t pp = prod / num[i];
		result += rem[i] * inv(pp, num[i]) * pp;
	}

	return result % prod;
}

bool checkTimestamp(int64_t timestamp, vector<BusInfo>& buses)
{
	for (auto bus : buses) {
		int64_t mod = (timestamp + bus.offset) % bus.busId;
		cout << mod << ", ";
		if ( mod != 0) {
			return false;
		}
	}
	cout << endl << endl;
	return true;
}


void day13()
{
	fstream myfile("day13_input.txt", ios_base::in);
	string str;

	getline(myfile, str);
	int departTime = stoi(str);

	vector<BusInfo> buses;

	getline(myfile, str);
	stringstream ss(str);
	int offset = 0;
	while (getline(ss, str, ',')) {
		if (str != "x") {
			BusInfo busInfo{ stoi(str), offset };
			buses.push_back(busInfo);
		}
		offset++;
	}

	displayBuses(buses);

	// part 1 - earliest bus
	int minWaitTime = INT32_MAX;
	int minBus = 0;
	for (auto bus : buses) {
		int m = departTime / bus.busId;
		int busTime = m * bus.busId;
		if (busTime < departTime) {
			busTime += bus.busId;
		}

		int waitTime = busTime - departTime;
		if (waitTime < minWaitTime) {
			minWaitTime = waitTime;
			minBus = bus.busId;
		}
	}

	cout << "Part 1" << endl;
	cout << "Bus = " << minBus << " Wait time = " << minWaitTime << endl;
	cout << "Bus * Wait time = " << minBus * minWaitTime << endl;

	// Part 2

#if 0 // brute force - SLOW!
	int64_t timestamp = buses[0].busId;
	while (!checkTimestamp(timestamp, buses)) {
		timestamp += buses[0].busId;
	}
#endif

	// Use Chinese remainder theorem
	//
	// States that X exists, where ni are prime, and ai are remainders
	//		X mod n1 = a1
	//		X mod n2 = a2
	//		X mod n3 = a3
	//	
	vector<int64_t> num;
	vector<int64_t> rem;
	for (auto bus : buses) {
		// array of prime numbers (n1,n2...). These are the bus IDs
		num.push_back(bus.busId);
		// array of remainders (i.e. results of X mod ni)
		// bus.offset is the distance past X. The actual modulus is
		// computed at the timestamp (i.e. X).
		// ai = (ni - bus offset) % ni;
		rem.push_back((-bus.offset + bus.busId) % bus.busId);
	}
	cout << "Part 2 - timestamp is " << findMinX(num, rem);

}

