#ifndef __RBTREE_H__
#define __RBTREE_H__

#pragma once

#include <string>
#include <iostream> 
#include <functional>
#include <ostream>


class RBTNode {
private:
	std::string word;
	int counter;

public:
	RBTNode();
	RBTNode(std::string word);
	~RBTNode();
	RBTNode* left;
	RBTNode* right;
	bool red;


	friend class RBTree;

};

class RBTree {
private:
	RBTNode* root;

	RBTNode* insert(std::string word, RBTNode* root);

	void preorder(RBTNode* root, std::ostream& os);
	void inorder(RBTNode* root, std::ostream& os);
	void postorder(RBTNode* root, std::ostream& os);

	void destroy(RBTNode* root);
	bool search(std::string word, RBTNode* root);

	RBTNode* rotateLeft(RBTNode* root);
	RBTNode* rotateRight(RBTNode* root);

	bool isRed(RBTNode* node);

	
public:
	RBTree();
	~RBTree();

	int height(RBTNode* root);

	RBTNode* top();
	int value(std::string word);
	int recalculate_value(std::string word);

	void insert(std::string word);
	int height();

	void preorder(std::ostream& os = std::cout);
	void inorder(std::ostream& os = std::cout);
	void postorder(std::ostream& os = std::cout);

	bool search(std::string word);
	void clear();
};

#endif