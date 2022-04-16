#include <iostream>
#include "Renderer.h"


Renderer::Renderer(sf::RenderWindow* window){
	this->window = window;
	if (!font.loadFromFile("Arial.ttf")) {

		std::cout << "ERROR LOADING FONT" << std::endl;
	}
	std::cout << "Font loaded..." << std::endl;


}

Renderer::~Renderer() {

}

// ------------ PRIVATE -----------------


// ------------ PUBLIC ------------------

void Renderer::Render() {
	for (RBTNode* node : nodes) {
		node->draw(window);
	}
}

void Renderer::RegisterNode(RBTNode* node) {
	nodes.push_back(node);
	//std::cout << node->word << std::endl;
}