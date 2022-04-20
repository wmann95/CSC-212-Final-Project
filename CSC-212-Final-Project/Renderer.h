#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>

#include <vector>
#include "RBTree.h"

class RBTNode;

class Renderer
{
private:
	sf::RenderWindow* window;
	bool closeFlag = false;
	std::vector<RBTNode*> nodes;
	sf::Font font;
	sf::Vector2f camPos;

public:
	Renderer(sf::RenderWindow* window);
	~Renderer();

	bool shouldWindowClose() { return closeFlag; }
	void Close() { closeFlag = true; }
	sf::RenderWindow* getWindow() { return window; }

	void Render();
	void RegisterNode(RBTNode* node);

	sf::Vector2f getCameraPos();
	void moveCam(float x, float y);
	
	sf::Font* getFont() {
		return &font;
	}
};

