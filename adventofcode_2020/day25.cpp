#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

uint64_t transformsSubjectNumber(uint64_t subjectNumber, uint64_t loopSize)
{
	uint64_t value = 1;

	for (auto i = 0; i < loopSize; i += 1) {
		value = value * subjectNumber;

		if (value > 20201227) {
			value = value % 20201227;
		}
	}
	//cout << subjectNumber << " " << loopSize << " " << loopSize << endl;

	return value;
}

void day25()
{
#if 1
	// puzzle input
	uint64_t cardPublicKey = 8252394;  // transformsSubjectNumber(7, cardLoopSize);
	uint64_t doorPublicKey = 6269621;  //transformsSubjectNumber(7, doorLoopSize);
#else
	// test input
	uint64_t cardPublicKey = 5764801;
	uint64_t doorPublicKey = 17807724;

#endif

	uint64_t cardLoopSize = 0;	//unknown - private key
	uint64_t doorLoopSize = 0;	//unknown - private key

	// determine card loop size
	uint64_t loopCount = 100000000;
	uint64_t value = 1;
	for (auto i = 1; i < loopCount; i++) {
		value = value * 7;
		value = value % 20201227;

		if ((cardLoopSize == 0) && (value == cardPublicKey)) {
			cardLoopSize = i;
		}
		if ((doorLoopSize == 0) && (value == doorPublicKey)) {
			doorLoopSize = i;
		}
		if (cardLoopSize && doorLoopSize) {
			break;
		}
	}

	if (cardLoopSize == 0 || doorLoopSize == 0) {
		cout << "Did not determine card/door loop size" << endl;
		return;
	}

	// encryption keys derived by card and door using each others public key
	// these should be equal
	uint64_t cardEncryptionKey = transformsSubjectNumber(doorPublicKey, cardLoopSize);
	uint64_t doorEncryptionKey = transformsSubjectNumber(cardPublicKey, doorLoopSize);

	cout << "card encryption key = " << cardEncryptionKey <<  
		" loop size = " << cardLoopSize <<endl;
	cout << "door encryption key = " << doorEncryptionKey <<
		" loop size = " << doorLoopSize << endl;




}