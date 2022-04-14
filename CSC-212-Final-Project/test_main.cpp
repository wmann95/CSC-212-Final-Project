#include "RBTree.h"

#include <string>
#include <iostream> 
#include <functional>
#include <ostream>


int main() {
	RBTree tree;
	tree.insert("swag");
	tree.insert("aa");
	tree.insert("a");
	tree.inorder();
}