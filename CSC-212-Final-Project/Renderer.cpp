#include <iostream>
#include "Renderer.h"


Renderer::Renderer(sf::RenderWindow* window){
	this->window = window;
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
	std::cout << node->text << std::endl;
}