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

	int xSize = window->getSize().x;
	int ySize = window->getSize().y;


	scroll = sf::Vector2f(30.0f, 30.0f); // This is where the camera scroll is handled. Having this set to 3.0 in both x and y with different scrollTarget will cause a small animation on start.
	scrollTarget = sf::Vector2f(1.0f, 1.0f); // This is where the scroll is trying to get to.

	camPos = sf::Vector2f(0.0f, 100.0f); // This is the current position of the view. Similar to the scroll, having a different camTarget will cause the cam to move towards that target.
	camTarget = sf::Vector2f(0, 250.0f); // Where the cam is trying to get.

	view.reset(sf::FloatRect(-400.0f, -25.0f, 800.0f, 600.0f)); // Resetting the view means that it will be the same size as the window, so the "scale" is one-to-one.
																// This is also offsetting the camera by half the x-width and up a little bit.

	view.setSize(xSize * scroll.x, ySize * scroll.y); // Ensure the camera scroll is at the right scroll before doing any rendering.
	view.setCenter(camPos); // Ensure the camera is in the correct place before rendering.
}

Renderer::~Renderer() {

}

void Renderer::Update(long long int deltaTime) {

	scroll = lerp(scroll, scrollTarget, 0.8f * deltaTime / 20.0f); // Interpolate the scroll towards the scrollTarget by a factor of 0.8f * the deltaTime. One would assume that this is the same as setting the factor to 1,
														   // but that causes the scroll to be very janky. Using the deltatime smooths it out.

	view.setSize(window->getSize().x * scroll.x, window->getSize().y * scroll.y); // Change the view to be scaled according to the scroll.

	camPos = lerp(camPos, camTarget, 0.95f * deltaTime / 10.0f); // Move the camera closer to the camTarget by a factor of 0.95f.

	view.setCenter(camPos); // sets the views position to where the cam is. Using move would cause the view to constantly move, not track the cam position.

}

// The main render function. Everything here should be about drawing to the window.
void Renderer::Render() {

	window->setView(view); // Ensures that the window is based around our current view.

	// Draw all the nodes
	for (RBTNode* node : nodes) {
		node->draw(window);
	}

	window->setView(window->getDefaultView()); // Reset the view back to the default. Keeps the window sane.

	// Do this after the reset to make sure it's static

	sf::RectangleShape box(sf::Vector2f(250, 30));
	box.setFillColor(sf::Color::Black);

	sf::Text wordText(this->currWord, font, 24);
	wordText.setFillColor(sf::Color::White);

	window->draw(box);
	window->draw(wordText);
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
	camTarget += sf::Vector2f(x, y);
}

void Renderer::moveCam(sf::Vector2f off) {
	camTarget += off;
}

void Renderer::increaseScroll() {
	scrollTarget.x *= scrollFactor;
	scrollTarget.y *= scrollFactor;
}

void Renderer::decreaseScroll() {
	scrollTarget.x /= scrollFactor;
	scrollTarget.y /= scrollFactor;
}

void Renderer::PlayEnd(RBTree* tree) {

	int treeHeight = tree->height();
	float width = (pow(2, treeHeight) * tree->getNodeOffset().x) / window->getSize().x;
	float height = (float)treeHeight * tree->getNodeOffset().y;

	width *= 1.1f;

	scrollTarget = sf::Vector2f(width, width);
	camTarget = sf::Vector2f(0, height / 2);
}

void Renderer::PlayStart() {
	scrollTarget = sf::Vector2f(1.0f, 1.0f);
	camTarget = sf::Vector2f(0, 0);
}