#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

using namespace std;

struct Operation {
	int opcode = 0;		// 0 = mask, 1 = mem write
	uint64_t address = 0;
	uint64_t value = 0;
	string mask;
};

void displayOperations(vector<Operation> &operations)
{
	for (auto op : operations) {
		cout << "opcode = " << op.opcode << " address = " << op.address << " value = " << op.value;
		cout << " mask = " << op.mask << endl;
	}
}

uint64_t processMask(string& mask, uint64_t value)
{
	auto i = mask.length() - 1;
	uint64_t result = 0;
	for (auto ch : mask) {
		if (ch == '0') {
			// nothing
		}
		else if (ch == '1') {
			result |= 1ULL << i;

		}
		else if (ch == 'X') {
			// propagate original value
			result |= value & (1ULL << i);

		}
		i--;
	}

	return result;
}

void processMask2(Operation& op, string &mask, unordered_map<uint64_t,
	uint64_t>& addressSpace)
{
	uint64_t addr = op.address;
	int countX = 0;

	// first process 0/1
	auto i = mask.length() - 1;
	for (auto ch : mask) {
		if (ch == '0') {
			// nothing
		}
		else if (ch == '1') {
			addr |= 1ULL << i;
		}
		else if (ch == 'X') {
			countX++;
		}
		i--;
	}

	// now process 'X' and produce the addresses
	if (countX == 0) {
		addressSpace[addr] = op.value;
	}
	else {
		int xValue = 1 << countX;
		//cout << "countX = " << countX << " xValue = " << xValue << endl;
		for (int x = 0; x < xValue; x++) {
			auto newaddr = addr;

			i = mask.length() - 1;
			int xpos = 0;

			for (auto ch : mask) {
				if (ch == 'X') {
					newaddr &= ~(1ULL << i);
					uint64_t modifiedbit = (x & (1 << xpos)) >> xpos;
					newaddr |= modifiedbit << i;
					xpos++;
				}
				i--;
			}
			addressSpace[newaddr] = op.value;
			//cout << "addr = " << newaddr << endl;
		}
	}

}


void processOperations(vector<Operation> &operations, 
	unordered_map<uint64_t, uint64_t> &addressSpace)
{
	string mask;
	for (auto op : operations) {
		if (op.opcode == 0) {
			// mask
			mask = op.mask;
		}
		else {
			// mem write
			addressSpace[op.address] = processMask(mask, op.value);
		}
	}
}

void processOperations2(vector<Operation>& operations,
	unordered_map<uint64_t, uint64_t>& addressSpace)
{
	string mask;
	for (auto op : operations) {
		if (op.opcode == 0) {
			// mask
			mask = op.mask;
		}
		else {
			// mem write
			processMask2(op, mask, addressSpace);
		}
	}
}


uint64_t sumMemory(unordered_map<uint64_t, uint64_t> &addressSpace)
{
	uint64_t sum = 0;

	for (auto pair : addressSpace) {
		sum += pair.second;
	}

	return sum;
}

void day14()
{
	fstream myfile("day14_input.txt", ios_base::in);
	string str;
	unordered_map<uint64_t, uint64_t> addressSpace;
	vector<Operation> operations;


	while (getline(myfile, str)) {
		//cout << str << endl;
		Operation op;
		if (str.substr(0, 4) == "mask") {
			stringstream ss(str);

			string mask;
			getline(ss, mask, '=');
			ss >> mask;

			op.opcode = 0;	// mask
			op.mask = mask;
		}
		else {
			stringstream ss(str);
			string dummy;
			getline(ss, dummy, '[');
			ss >> op.address;
			getline(ss, dummy, '=');
			ss >> op.value;

			op.opcode = 1;	// mem write
		}

		operations.push_back(op);
	}

	//displayOperations(operations);

	// part1
	processOperations(operations, addressSpace);
	cout << "Sum = " << sumMemory(addressSpace) << endl;


	// part2
	addressSpace.clear();
	processOperations2(operations, addressSpace);
	cout << "Sum = " << sumMemory(addressSpace) << endl;

}

