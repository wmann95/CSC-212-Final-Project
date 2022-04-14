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