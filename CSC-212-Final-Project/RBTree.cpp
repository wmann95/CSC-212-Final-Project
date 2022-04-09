#include "RBTree.h"
#include "TransformHelper.h"

// -----------RBTNode-----------

void RBTree::RBTNode::draw(sf::RenderWindow* window) {
	if (red) {
		shape.setFillColor(sf::Color::Red);
	}
	else {
		shape.setFillColor(sf::Color::Black);
	}


	window->draw(shape);
}

void RBTree::RBTNode::update(long long int millis) {

	float factor = (millis / (1000.0f)) * /*speed*/1.0f;
	shape.setPosition((Vector2f)lerp(shape.getPosition(), target, factor));
}

void RBTree::RBTNode::setTarget(sf::Vector2f t) {
	target = t;
}

// -----------RBTree------------

void RBTree::Draw(sf::RenderWindow* window) {
	root->draw(window);
}

void RBTree::Update(long long int millis) {
	root->update(millis);
}