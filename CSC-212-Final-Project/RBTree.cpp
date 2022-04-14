#include "RBTree.h"

#include <string>
#include <iostream> 
#include <functional>
#include <ostream>


RBTNode::RBTNode() {
	this->counter = 0;
	this->left = nullptr;
	this->right = nullptr;
	red = false;
}

RBTNode::RBTNode(std::string word) {
	this->word = word;
	this->counter = 1;
	this->left = nullptr;
	this->right = nullptr;
	red = false;
}

RBTNode::~RBTNode() {

}

int RBTree::value(std::string word){
	int sum = 0;
	for (int i = 0; i < word.size(); i++){
		char x = std::tolower(word[i]);
		sum += int(x);
	}
	return sum;
}

RBTNode* RBTree::insert(std::string word, RBTNode* root) {
	if (!root) {
		return new RBTNode(word);
	}

	if (value(word) == value(root->word)){
		if (word == root->word){
			root->counter++;
			//ADD MORE
		}
	}

	else if (value(word) < value(root->word)) {
		root->left = insert(word, root->left);
		root->left->red = true;
		std::cout << "l" << std::endl;
	}
	else{
		root->right = insert(word, root->right);
		root->right->red = true;
		std::cout << "r" << std::endl;
	}
	
	if (isRed(root->right)) {
		root = rotateLeft(root);
	}

	if (isRed(root->left) && isRed(root->left->left)) {
		root = rotateRight(root);
	}

	if (isRed(root->left) && isRed(root->right)) {
		root->red = true;
		root->left->red = false;
		root->right->red = false;
	}

	if (root->red == true){
		root->red = false;
	}

	return root;
}

int RBTree::height(RBTNode* root) {
	if (!root) {
		return -1;
	}
	int left = height(root->left);
	int right = height(root->right);

	return (left > right ? left + 1 : right + 1);
}

void RBTree::preorder(RBTNode* root, std::ostream& os) {
	if (!root) {
		return;
	}

	os << root->word << ":" << root->red << ":" << root->counter << ", ";
	this->preorder(root->left, os);
	this->preorder(root->right, os);

	return;
}

void RBTree::inorder(RBTNode* root, std::ostream& os) {
	if (!root) {
		return;
	}

	this->inorder(root->left, os);
	os << root->word << ":" << root->red << ":" << root->counter << ", ";
	this->inorder(root->right, os);

	return;
}

void RBTree::postorder(RBTNode* root, std::ostream& os) {
	if (!root) {
		return;
	}

	this->postorder(root->left, os);
	this->postorder(root->right, os);
	os << root->word << ":" << root->red << ":" << root->counter << ", ";

	return;
}

void RBTree::destroy(RBTNode* root) {
	if (!root) {
		return;
	}

	this->destroy(root->left);
	this->destroy(root->right);
	delete root->left;
	delete root->right;
}

bool RBTree::search(std::string word, RBTNode* root) {
	if (!root) {
		return false;
	}

	if ((value(word) == value(root->word)) && (word == root->word)) {
		return true;
	}

	if (word < root->word) {
		return this->search(word, root->left);
	}
	else {
		return this->search(word, root->right);
	}
}

RBTNode* RBTree::rotateLeft(RBTNode* root) {
	RBTNode* p = root->right;
	root->right = root->right->left;
	p->left = root;

	p->red = p->left->red;
	p->left->red = true;
	return p;
}

RBTNode* RBTree::rotateRight(RBTNode* root) {
	RBTNode* p = root->left;
	root->left = root->left->right;
	p->right = root;

	p->red = p->right->red;
	p->right->red = true;
	return p;
}

bool RBTree::isRed(RBTNode* node) {
	if (!node) {
		return false;
	}

	return node->red;
}

/*
 * Public Functions
*/

RBTree::RBTree() {
	this->root = nullptr;
}

RBTree::~RBTree() {
	this->destroy(this->root);
}

void RBTree::insert(std::string word) {
	this->root = this->insert(word, this->root);
	this->root->red = false;
}

int RBTree::height() {
	return this->height(this->root);
}

void RBTree::preorder(std::ostream& os) {
	this->preorder(this->root, os);
	os << "\n";
}

void RBTree::inorder(std::ostream& os) {
	this->inorder(this->root, os);
	os << "\n";
}

void RBTree::postorder(std::ostream& os) {
	this->postorder(this->root, os);
	os << "\n";
}

bool RBTree::search(std::string word) {
	return this->search(word, this->root);
}

void RBTree::clear() {
	this->destroy(this->root);
	this->root = nullptr;
}