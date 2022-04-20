#include "RBTree.h"

#include <string>
#include <iostream> 
#include <functional>
#include <ostream>


RBTNode::RBTNode() {
	this->counter = 0;
	this->left = nullptr;
	this->right = nullptr;
	red = true;
}

RBTNode::RBTNode(std::string word) {
	this->word = word;
	this->counter = 1;
	this->left = nullptr;
	this->right = nullptr;
	red = true;
}

RBTNode::~RBTNode() {

}

int RBTree::value(std::string word){
	int sum = 0;
	for (int i = 1; i <= word.size(); i++){
		char x = std::tolower(word[i - 1]);
		sum += int(x) * i;
	}
	return sum;
}

int RBTree::recalculate_value(std::string word){
	int sum = 0;
	for (int i = 1; i <= word.size(); i++){
		char x = std::tolower(word[word.size() - i]);
		sum += int(x) * i;
	}
	return sum;
}

RBTNode* RBTree::insert(std::string word, RBTNode* root) {
	if (!root) {
		return new RBTNode(word);
	}
	int word_val = value(word);
	int root_val = value(root->word);
	if (word_val == root_val){
		if (word == root->word){
			root->counter++;
		}else{
			std::cout << "duplicate" << std::endl;
		}
	}
	if (word_val < root_val) {
		root->left = insert(word, root->left);
		root->left->red = true;
	}
	else if (word_val > root_val){
		root->right = insert(word, root->right);
		root->right->red = true;
	}
	bool good = false;
	while (!good){
		bool change = false;

	if (isRed(root->right) && !isRed(root->left)) {
		root = rotateLeft(root);
		std::cout << "Left" << std::endl;
		change = true;
	}

	if (isRed(root->left) && isRed(root->left->left)) {
		root = rotateRight(root);
		std::cout << "Right" << std::endl;
		change = true;
	}
	
	if (isRed(root->left) && isRed(root->right)) {
		root->red = true;
		root->left->red = false;
		root->right->red = false;
		change = true;
		std::cout << "Swap" << std::endl;
	}

	if (!change){
		good = true;
	}
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

RBTNode* RBTree::top(){
	return root;
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
	root->right = p->left;
	p->left = root;

	p->red = root->red;
	root->red = true;
	return p;
}

RBTNode* RBTree::rotateRight(RBTNode* root) {
	RBTNode* p = root->left;
	root->left = p->right;
	p->right = root;

	p->red = root->red;
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