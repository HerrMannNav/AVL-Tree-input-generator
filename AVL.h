#pragma once

#include <iostream>
#include <algorithm>
#include <ctime>
#include <string>
#include <vector>
using namespace std;

class AVL {

	private:

		// Instrumentation variables
		int instrumentationActive;
		clock_t beginTime, endTime;
		string instrumentationTag;
		vector<int> elapsedTimes;

		bool debugPrints;

		// AVL tree variables
		struct Node {
			int value;
			Node* leftChild;
			Node* rightChild;
			int height;
		};

		Node* root;
		int numNodes;

		Node* insert(int value, Node* node);
		Node* remove(int value, Node* node);

		Node* rightRotate(Node* node);
		Node* rightRotateAfterLeftRotateToLeftChild(Node* node);
		Node* leftRotate(Node* node);
		Node* leftRotateAfterRightRotateToRightChild(Node* node);

		int balanceFactor(Node* node);
		int nodeHeight(Node* node);
		Node* treeMinValue(Node* node);
		void inorder(Node* node);

	public:

		AVL();
		~AVL();

		void insertValue(int value);
		void removeValue(int value);

		void print();
		void printElapsedTimes();

		void setInstrumentation(bool active, int numberOfOperations);
		void checkInstrumentation();
		void updateInstrumentation();

		void setDebugPrints(bool debugPrints);
};

