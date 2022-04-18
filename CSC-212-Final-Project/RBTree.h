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
	RBTNode(Renderer* renderer, RBTNode* p);
	RBTNode(Renderer* renderer, RBTNode* p, std::string word, bool color);
	Renderer* renderer;
	int counter;
	RBTNode* left;
	RBTNode* right;
	RBTNode* parent;
	sf::CircleShape shape;
	float shapeSize = 25.0f;
	sf::Vector2f position;
	sf::Vector2f target;
	bool red;
	sf::Text text;

	int xPos = 0, yPos = 0;

	void update(long long int);

public:
	~RBTNode();

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

	void updateTargets(RBTNode* root, RBTNode* prev, int baseWidth);

	void destroy(RBTNode* root);
	bool search(std::string word, RBTNode* root);

	RBTNode* rotateLeft(RBTNode* root);
	RBTNode* rotateRight(RBTNode* root);

	bool isRed(RBTNode* node);

	float xStart = 0.0f;
	float yStart = 0.0f;
	float xOffset = 60.0f;
	float yOffset = 60.0f;


public:
	RBTree(Renderer* renderer);
	~RBTree();

	int value(std::string word);

	void insert(std::string word);
	int height();

	void preorder(std::ostream& os = std::cout);
	void inorder(std::ostream& os = std::cout);
	void postorder(std::ostream& os = std::cout);

	bool search(std::string word);
	void clear();

	void Update(long long int millis);
	void UpdateNodeTargets();

	RBTNode* top() {
		return root;
	}

};

/*
#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include "Renderer.h"

class Renderer;


class RBTNode {
private:
	bool red = false;
	int x, y;
	sf::CircleShape shape;
	sf::Vector2f target;
	RBTNode(Renderer* renderer, std::string in);

public:
	std::string text;

	void draw(sf::RenderWindow* window);
	void update(long long int millis);
	void setTarget(sf::Vector2f target);

	friend class RBTree;
};

class RBTree {


private:

	sf::RenderWindow* window;
	Renderer* renderer;
	RBTNode* root;
	std::vector<RBTNode*> nodes;
	int x, y;
	sf::Vector2f target;
public:
	RBTree(Renderer* renderer);
	~RBTree();

	void Draw(sf::RenderWindow* window);
	void Update(long long int millis);

	void insert(std::string in);

	void debugSetTarget(sf::Vector2f target) {
		this->target = target;

		for (RBTNode* node : nodes) {
			node->setTarget(target);
		}
	}


};

*/