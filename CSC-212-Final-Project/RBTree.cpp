#include "RBTree.h"
#include "TransformHelper.h"

// -----------RBTNode-----------
RBTNode::RBTNode(Renderer* renderer, std::string in) {
	shape = sf::CircleShape(20.0f);
	target = sf::Vector2f(0.0f, 0.0f);
	text = in;
	renderer->RegisterNode(this);
}

void RBTNode::draw(sf::RenderWindow* window) {
	if (this->red) {
		shape.setFillColor(sf::Color::Red);
	}
	else {
		shape.setFillColor(sf::Color::Black);
	}


	window->draw(shape);
}

void RBTNode::update(long long int millis) {

	float factor = (millis / (1000.0f)) * /*speed*/1.0f;
	shape.setPosition((Vector2f)lerp(shape.getPosition(), target, factor));
}

void RBTNode::setTarget(sf::Vector2f t) {
	target = t;
}

// -----------RBTree------------

RBTree::RBTree(Renderer* renderer) {
	this->window = renderer->getWindow();
	this->renderer = renderer;
}

RBTree::~RBTree() {

	for (int i = nodes.size() - 1; i >= 0; i--) {
		delete nodes[i];
	}

	nodes.clear();

	if (!root) return;
	delete root;
}

void RBTree::Draw(sf::RenderWindow* window) {
	if (!root) return;

	//root->draw(window);

}

void RBTree::Update(long long int millis) {
	if (!root) return;

	//root->update(millis);
}

void RBTree::insert(std::string in) {
	if (!root) {
		root = new RBTNode(renderer, in);
		nodes.push_back(root);
		return;
	}
	nodes.push_back(new RBTNode(renderer, in));
}