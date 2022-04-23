#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>

#include <vector>
#include "RBTree.h"

class RBTNode;
class RBTree;

class Renderer
{
private:
	sf::RenderWindow* window;
	bool closeFlag = false;
	std::vector<RBTNode*> nodes;
	sf::Font font;
	sf::Vector2f camPos;
	sf::Vector2f camTarget;
	std::string currWord;
	sf::View view; // Create a view. This is essentially a camera in the world.
	sf::Vector2f scroll;
	sf::Vector2f scrollTarget;
	float scrollFactor = 1.5f;

public:
	Renderer(sf::RenderWindow* window);
	~Renderer();

	bool shouldWindowClose() { return closeFlag; }
	void Close() { closeFlag = true; }
	sf::RenderWindow* getWindow() { return window; }

	void Update(long long int millis);
	void Render();
	void RegisterNode(RBTNode* node);

	sf::Vector2f getCameraPos();
	void moveCam(float x, float y);
	void moveCam(sf::Vector2f off);
	
	sf::Font* getFont() {
		return &font;
	}

	void setCurrWord(std::string word) {
		currWord = word;
	}

	void increaseScroll();
	void decreaseScroll();

	void PlayEnd(RBTree* tree);
	void PlayStart();
};

