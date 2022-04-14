#ifndef __RBTREE_H__
#define __RBTREE_H__

#pragma once

#include <string>
#include <iostream> 


class RBTNode {
private:
	std::string word;
	int counter;
	RBTNode* left;
	RBTNode* right;
	bool red = false;

	void swapColor() { red != red; }

public:
	RBTNode();
	RBTNode(std::string word);
	~RBTNode();

	friend class RBTree;

};

class RBTree {
private:
	RBTNode* root;

	RBTNode* insert(int word, RBTNode* root);
	int height(RBTNode* root);

	void preorder(RBTNode* root, std::ostream& os);
	void inorder(RBTNode* root, std::ostream& os);
	void postorder(RBTNode* root, std::ostream& os);

	void destroy(RBTNode* root);
	bool search(int word, RBTNode* root);

	RBTNode* rotateLeft(RBTNode* root);
	RBTNode* rotateRight(RBTNode* root);

	bool isRed(RBTNode* node);

	
public:
	RBTree();
	~RBTree();

	void insert(int word);
	int height();

	void preorder(std::ostream& os = std::cout);
	void inorder(std::ostream& os = std::cout);
	void postorder(std::ostream& os = std::cout);

	bool search(int word);
	void clear();
};

#endif