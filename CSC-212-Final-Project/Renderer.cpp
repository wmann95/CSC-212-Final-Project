#include <iostream>
#include "Renderer.h"


// ------------ PRIVATE -----------------


// ------------ PUBLIC ------------------


Renderer::Renderer(sf::RenderWindow* window) {
	this->window = window;
	if (!font.loadFromFile("Arial.ttf")) {

		std::cout << "ERROR LOADING FONT" << std::endl;
	}
	std::cout << "Font loaded..." << std::endl;

	camPos = sf::Vector2f(window->getSize().x / 2.0f, window->getSize().y / 2.0f);
}

Renderer::~Renderer() {

}

void Renderer::Render() {

	for (RBTNode* node : nodes) {
		node->draw(window);
	}
}

void Renderer::RegisterNode(RBTNode* node) {
	node->renderer = this;
	nodes.push_back(node);
	//std::cout << node->word << std::endl;
}

sf::Vector2f Renderer::getCameraPos() {
	return camPos;
}

void Renderer::moveCam(float x, float y) {
	camPos += sf::Vector2f(x, y);
}