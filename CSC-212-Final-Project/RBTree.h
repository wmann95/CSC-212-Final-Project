#pragma once

#include <string>
#include <iostream> 
#include <functional>
#include <ostream>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include "Renderer.h"
#include "TransformHelper.h"

class Renderer;

class RBTNode {
private:
	// Constructors should be private to stop anything else from creating the nodes other
	// than the tree.
	RBTNode(Renderer* renderer, RBTNode* p);
	RBTNode(Renderer* renderer, RBTNode* p, std::string word, bool color);

	Renderer* renderer; // access to the renderer in order to draw and register.
	int counter; // word count
	bool red;
	float shapeSize = 25.0f;
	RBTNode* left;
	RBTNode* right;
	RBTNode* parent;
	sf::CircleShape shape;
	sf::Vector2f position;
	sf::Vector2f target;
	sf::Text text;


	void update(long long int);

public:

	std::string word;
	void draw(sf::RenderWindow*);

	friend class RBTree;
	friend class Renderer;
};

class RBTree {
private:
	Renderer* renderer;
	std::vector<RBTNode*> nodes;

	RBTNode* root;

	RBTNode* insert(std::string word, RBTNode* root, RBTNode* prev);
	int height(RBTNode* root);

	void preorder(RBTNode* root, std::ostream& os);
	void inorder(RBTNode* root, std::ostream& os);
	void postorder(RBTNode* root, std::ostream& os);
	void getWordCounts(RBTNode*, std::vector<std::pair<std::string, int>>*);

	void updateTargets(RBTNode* root, RBTNode* prev, int baseWidth);

	void destroy(RBTNode* root);
	bool search(std::string word, RBTNode* root);

	RBTNode* rotateLeft(RBTNode* root);
	RBTNode* rotateRight(RBTNode* root);

	bool isRed(RBTNode* node);

	int count(RBTNode* node, std::string);

	float xStart = 0.0f;
	float yStart = 0.0f;
	float xOffset = 60.0f;
	float yOffset = 40.0f;


public:
	RBTree(Renderer* renderer);
	~RBTree();

	void insert(std::string word);
	int height();

	void preorder(std::ostream& os = std::cout);
	void inorder(std::ostream& os = std::cout);
	void postorder(std::ostream& os = std::cout);
	void getWordCounts(std::vector<std::pair<std::string, int>>*);

	bool search(std::string word);
	void clear();
	int count(std::string);

	void Update(long long int millis);
	void UpdateNodeTargets();

	RBTNode* top() {
		return root;
	}

	sf::Vector2f getNodeOffset() {
		return sf::Vector2f(xOffset, yOffset);
	}
};
