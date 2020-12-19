#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>

using namespace std;

struct Instruction {
	string operation;
	int argument = 0;
	int count = 0;
};

void displayProgram(vector<Instruction>& program)
{
	for (auto instr : program) {
		cout << instr.operation << " " << instr.argument << " " << instr.count << endl;
	}
}

auto runProgram(vector<Instruction>& program)
{
	int acc = 0;
	int pc = 0;
	bool bInfiteLoopDetected = false;

	// reset counts
	for (auto &instr : program) {
		instr.count = 0;
	}

	while (1) {
		Instruction &instr = program[pc];

		if (instr.count == 1) {
			bInfiteLoopDetected = true;
			break;
		}

		instr.count++;
		
		if (instr.operation == "nop") {
			pc++;
		}
		if (instr.operation == "acc") {
			acc += instr.argument;
			pc++;
		}
		if (instr.operation == "jmp") {
			pc += instr.argument;
		}
		
		if (pc >= program.size()) {
			break;
		}
	}

	return make_tuple(bInfiteLoopDetected, acc);
}

void swapOperations(Instruction& instr)
{
	if (instr.operation == "nop") {
		instr.operation = "jmp";
		return;
	}
	if (instr.operation == "jmp") {
		instr.operation = "nop";
	}
}

void day8()
{
	fstream myfile("day8_input.txt", ios_base::in);
	string str;
	vector<Instruction> program;

	while (getline(myfile, str)) {
		stringstream ss(str);
		Instruction instr;
		getline(ss, instr.operation, ' ');
		string arg;
		getline(ss, arg);
		instr.argument = stoi(arg);

		program.push_back(instr);
	}

	//displayProgram(program);

	auto [bInfiteLoopDetected, acc] = runProgram(program);
	cout << "Infinite loop = " << bInfiteLoopDetected << " acc = " << acc << endl;

	// part 2
	// switch nop and jump instructions until infinte loop is not detected
	for (int i = 0; i < program.size(); i++) {
		auto& instr = program[i];
		swapOperations(instr);
		auto [bInfiteLoopDetected, acc] = runProgram(program);
		if (!bInfiteLoopDetected) {
			cout << "No infinite loop acc = " << acc << endl;
			break;
		}
		swapOperations(instr);
	}
}
