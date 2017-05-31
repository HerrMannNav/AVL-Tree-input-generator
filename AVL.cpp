#include "stdafx.h"

#include "AVL.h"

AVL::AVL() {
	this->root = NULL;
	this->numNodes = 0;

	this->instrumentationActive = 0;
	this->beginTime = this->endTime = -1;

	this->debugPrints = false;
}

AVL::~AVL() {}

AVL::Node* AVL::insert(int value, Node* node) {
	// Insert the new value in the corresponding place
	if (node == NULL) {                                                                                  // Base case: we have finally found a not-initialised leaf, so we initialise it with the new data
		node = new Node;
		node->value = value;
		node->leftChild = NULL;
		node->rightChild = NULL;
		node->height = 0;
	}
	else if (value < node->value) node->leftChild = this->insert(value, node->leftChild);                // If the value is smaller than 'node' -> we know the value must be inserted somewhere in the right child
	else if (value > node->value) node->rightChild = this->insert(value, node->rightChild);              // If the value is bigger than 'node' -> we know the value must be inserted somewhere in the right child
	                                                                                                     // No else defined, we assume no repeated values ; if an existing value is inserted it does nothing.
	// Check for the unbalancing of the tree
	int balanceFactor = this->balanceFactor(node);
	if (balanceFactor > 1) {                                                                             // If it becomes unbalanced
		if (value < node->leftChild->value) node = this->rightRotate(node);                              // If we are unbalanced left-left -> apply a single right rotation (enough: it will result in balanceFactor of 1)
		else                                node = this->rightRotateAfterLeftRotateToLeftChild(node);    // If we are unbalanced left-right -> first apply a left rotation to left child and then it is the same situation as above
	}
	else if (balanceFactor < -1) {
		if (value > node->rightChild->value) node = this->leftRotate(node);                              // If we are unbalanced right-right -> apply a single left rotation (enough: it will result in balanceFactor of 1)
		else                                 node = this->leftRotateAfterRightRotateToRightChild(node);  // If we are unbalanced right-left -> first apply a right rotation to right child and then it is the same situation as above
	}

	node->height = max(this->nodeHeight(node->leftChild), this->nodeHeight(node->rightChild)) + 1;       // At this point we have inserted and balanced all the nodes of the 'node' subtree, so we need to update their height
	return node;
}

AVL::Node* AVL::remove(int value, Node* node) {
	// Search the node to be deleted and fix the pointers of the tree
	if (node == NULL) return node;                                                                  // If we finally are in a NULL leaf, then stop searching

	else if (value < node->value) node->leftChild = this->remove(value, node->leftChild);           // If the value is smaller -> continue searching in the left child
	else if (value > node->value) node->rightChild = this->remove(value, node->rightChild);         // If the value is bigger -> Continue searching in the right child
	else {                                                                                          // If 'node' is the node we need to remove
		if (node->leftChild != NULL && node->rightChild != NULL) {                                  // If it has both children -> need to reconstruct the whole subtree
			node->value = this->treeMinValue(node->rightChild)->value;                              // Replace 'node's value with the next value (in terms of increeasing order), which is the minimum of the right subtree
			node->rightChild = this->remove(node->value, node->rightChild);                         // And delete the moved value from the right subtree
		}
		else {                                                                                      // Otherwise (1 or 0 children) it is much more simple
			Node* toRemoveNode = node;                                                              // Save the reference to the condemned node
			if (node->leftChild == NULL) node = node->rightChild;                                   // Change the reference: now 'node' points out to its right child
			else if (node->rightChild == NULL) node = node->leftChild;                              // (the same situation but with the other child)
			delete toRemoveNode;                                                                    // The old node reference (saved in 'toRemoveNode) is now pointing out to a node we are not interested in anymore.
		}
	}
	if (node == NULL) return node;                                                                  // Need to check again whether 'node' is NULL, mainly because of the 0-child case

	node->height = max(this->nodeHeight(node->leftChild), this->nodeHeight(node->rightChild)) + 1;  // Update their height (same as in the insert operation)

	// As in the insert operation, check for the unbalancing of the tree
	int balanceFactor = this->balanceFactor(node);
	if (balanceFactor > 1) {
		int leftChildBF = this->balanceFactor(node->leftChild);
		if (leftChildBF >= 1) node = this->rightRotate(node);                                        // Left left unbalancing: a single right rotation is needed
		else if (leftChildBF <= -1) node = this->rightRotateAfterLeftRotateToLeftChild(node);        // Left right unbalancing: a left rotation of the left child and then it is the same as above
	}
	else if (balanceFactor < -1) {
		int rightChildBF = this->balanceFactor(node->rightChild);
		if (rightChildBF <= -1) node = this->leftRotate(node);                                       // Right right unbalancing: a single left rotation is needed
		else if (rightChildBF >= 1) node = this->leftRotateAfterRightRotateToRightChild(node);       // Right left unbalancing: a right rotation of the right child and then it is the same as above
	}

	return node;	
}

AVL::Node* AVL::rightRotate(Node* node) {
	Node* newSubtreeRoot = node->leftChild;
	node->leftChild = newSubtreeRoot->rightChild;
	newSubtreeRoot->rightChild = node;
	node->height = max(nodeHeight(node->leftChild), nodeHeight(node->rightChild) ) + 1;
	newSubtreeRoot->height = max(nodeHeight(newSubtreeRoot->leftChild), node->height) + 1;
	return newSubtreeRoot;
}

AVL::Node* AVL::rightRotateAfterLeftRotateToLeftChild(Node* node) {
	node->leftChild = this->leftRotate(node->leftChild);
	return this->rightRotate(node);
}

AVL::Node* AVL::leftRotate(Node* node) {
	Node* newSubtreeRoot = node->rightChild;
	node->rightChild = newSubtreeRoot->leftChild;
	newSubtreeRoot->leftChild = node;
	node->height = max( nodeHeight(node->leftChild), nodeHeight(node->rightChild) ) + 1;
	newSubtreeRoot->height = max(nodeHeight(node->rightChild), node->height) + 1;
	return newSubtreeRoot;
}

AVL::Node* AVL::leftRotateAfterRightRotateToRightChild(Node* node) {
	node->rightChild = this->rightRotate(node->rightChild);
	return this->leftRotate(node);
}

int AVL::balanceFactor(Node* node) {
	if (node == NULL) return 0;
	return nodeHeight(node->leftChild) - nodeHeight(node->rightChild);
}

int AVL::nodeHeight(Node* node) {
	if (node == NULL) return -1;
	return node->height;
}

AVL::Node* AVL::treeMinValue(Node * node) {
	if (node == NULL || node->leftChild == NULL) return node;
	return treeMinValue(node->leftChild);
}

void AVL::inorder(Node* node) {
	if (node != NULL) {
		inorder(node->leftChild);
		cout << "(" << node->value << " H:" << node->height << ") ";
		inorder(node->rightChild);
	}
	else cout << "(_ H:-1) ";
}

void AVL::setInstrumentation(bool active, int numberOfOperations) {
	if (active) {
		this->instrumentationActive = numberOfOperations;
		this->instrumentationTag = "INSTRUMENTED_OPERATIONS=" + to_string(numberOfOperations) + "|TREENODES=" + to_string(this->numNodes) + "|TREEHEIGHT=" + to_string(this->nodeHeight(this->root));
	}
	else this->instrumentationActive = 0;
}

void AVL::checkInstrumentation() {
	if (this->instrumentationActive) {                       // If instrumentation is active (> 0)
		if (this->beginTime < 0) {
			this->beginTime = clock();                       // And we have not already started to record, we start it
		}
	}
	else {                                                   // In case instrumentation is not active
		if (this->beginTime > 0) {                           // But we know it has been active until the previous operation
			this->endTime = clock();                         // We record final time
			double elapsedTime = double(this->endTime - this->beginTime);
			elapsedTimes.push_back(elapsedTime);
			if (this->debugPrints) cout << this->instrumentationTag << " | ELAPSED TIME: " << elapsedTime << endl;
			this->beginTime = this->endTime = -1;            // And reset everything
		}
	}
}

void AVL::updateInstrumentation() {
	if (this->instrumentationActive > 0) this->instrumentationActive--;
	this->checkInstrumentation();
}

void AVL::setDebugPrints(bool debugPrints) {
	this->debugPrints = debugPrints;
}

void AVL::insertValue(int value) {
	if (this->debugPrints) cout << "INSERTING VALUE " << value << "...\tCURRENT HEIGHT: " << to_string(this->root != NULL ? this->root->height : 0) << endl;

	this->checkInstrumentation();
	this->root = this->insert(value, this->root);
	this->updateInstrumentation();

	this->numNodes++;
}

void AVL::removeValue(int value) {
	if (this->debugPrints) cout << "DELETING VALUE " << value << "...\tCURRENT HEIGHT: " << to_string(this->root != NULL ? this->root->height : 0) << endl;

	this->checkInstrumentation();
	this->root = this->remove(value, this->root);
	this->updateInstrumentation();

	this->numNodes--;
}

void AVL::print() {
	if (this->debugPrints) {
		cout << "PRINTING TREE IN INORDER (SHOULD BE AN INCREASING LIST) ; HEIGHT = " << this->root->height << " | NUMBER OF NODES = " << this->numNodes << endl;
		this->inorder(this->root);
		cout << endl;
	}
}

void AVL::printElapsedTimes() {
	cout << "RESULTING TIMES: " << endl;
	for (int i = 0; i < this->elapsedTimes.size(); i++) cout << this->elapsedTimes[i] << " ";
	cout << endl;
}
