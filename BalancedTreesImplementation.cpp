#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "AVL.h"
using namespace std;

int main() {
	bool debugPrints = false;

	AVL balancedTree;
	balancedTree.setDebugPrints(debugPrints);

	string operation, value, numberOfElements;
	//ifstream inputFile("data/inputExample.txt");
	ifstream inputFile("data/inputExperiment-Insertion.txt");
	//ifstream inputFile("data/inputExperiment-Deletion.txt");

	cout << "READING FILE..." << endl;
	vector<pair<string, int>> operations;
	try {
		if (inputFile.is_open()) {
			getline(inputFile, numberOfElements);
			operations.reserve(stoi(numberOfElements));
			while (getline(inputFile, operation, ' ')) {
				if (operations.size() % 100000 == 0) cout << "OPS READ:" << operations.size() << endl;  // Uncomment this when input is huge in order to see the input reading progression
				getline(inputFile, value);
				operations.push_back(make_pair(operation, stoi(value)));
			}
			inputFile.close();
		}
		else {
			if (debugPrints) cout << "Unable to open file" << endl;
		}
	}
	catch (exception& e) {
		cout << "Error!! Message: " << e.what() << endl;
	}
	cout << "TOTAL NUMBER OF OPERATIONS: " << operations.size() << endl;
	
	cout << "APPLYING OPERATIONS..." << endl;
	for (int i = 0; i < operations.size(); i++) {
		string operation = operations[i].first;
		int value = operations[i].second;

		if (operation == "IN") balancedTree.insertValue(value);
		else if (operation == "RE") balancedTree.removeValue(value);
		else if (operation == "TI") balancedTree.setInstrumentation(true, value);
		else if (operation == "PR" && debugPrints) balancedTree.print();
	}
	balancedTree.printElapsedTimes();
}

