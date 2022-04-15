#include "RBTree.h"


RBTNode::RBTNode(Renderer* renderer) {
	this->counter = 0;
	this->left = nullptr;
	this->right = nullptr;
	red = false;
	shape = sf::CircleShape(shapeSize);
	renderer->RegisterNode(this);
}

RBTNode::RBTNode(Renderer* renderer, std::string word) {
	this->word = word;
	this->counter = 1;
	this->left = nullptr;
	this->right = nullptr;
	red = false;
	shape = sf::CircleShape(shapeSize);
	renderer->RegisterNode(this);
}

RBTNode::~RBTNode() {}

int RBTree::value(std::string word) {
	int sum = 0;
	for (unsigned int i = 0; i < word.size(); i++) {
		char x = tolower(word[i]);
		sum += int(x);
	}
	return sum;
}

bool areWordsInOrder(std::string word1, std::string word2) {
	return word1 < word2;
}

RBTNode* RBTree::insert(std::string word, RBTNode* root, float x = 400.0f, float y = 25.0f) {

	if (!root) {
		RBTNode* node = new RBTNode(renderer, word);
		node->red = true;
		nodes.push_back(node);
		node->target = sf::Vector2f(x, y);
		//std::cout << node->word << " : " << x << ", " << y << std::endl;

		return node;
	}

	if (word == root->word) {
		root->counter++;
		//ADD MORE
		return root;
	}


	if(areWordsInOrder(word, root->word)){
		root->left = insert(word, root->left, x - 50.0f, y + 30.0f);
		root->left->red = true;

		//nodes.push_back(root->left);


		//std::cout << "l" << std::endl;
	}
	else {
		root->right = insert(word, root->right, x + 50.0f, y + 30.0f);
		root->right->red = true;
		//nodes.push_back(root->right);
		//std::cout << "r" << std::endl;
	}

	if (isRed(root->right) && !isRed(root->left)) {
		root = rotateLeft(root);
	}

	if (isRed(root->left) && isRed(root->left->left)) {
		root = rotateRight(root);
	}

	if (isRed(root->left) && isRed(root->right)) {
		root->red = true;
		root->left->red = false;
		root->right->red = false;
	}

	if (root->red == true) {
		root->red = false;
	}

	return root;
}

int RBTree::height(RBTNode* root) {
	if (!root) {
		return -1;
	}
	int left = height(root->left);
	int right = height(root->right);

	return (left > right ? left + 1 : right + 1);
}

void RBTree::preorder(RBTNode* root, std::ostream& os) {
	if (!root) {
		return;
	}

	os << root->word << ":" << root->red << ":" << root->counter << ", ";
	this->preorder(root->left, os);
	this->preorder(root->right, os);

	return;
}

void RBTree::inorder(RBTNode* root, std::ostream& os) {
	if (!root) {
		return;
	}

	this->inorder(root->left, os);
	os << root->word << ":" << root->red << ":" << root->counter << ", ";
	this->inorder(root->right, os);

	return;
}

void RBTree::postorder(RBTNode* root, std::ostream& os) {
	if (!root) {
		return;
	}

	this->postorder(root->left, os);
	this->postorder(root->right, os);
	os << root->word << ":" << root->red << ":" << root->counter << ", ";

	return;
}

void RBTree::destroy(RBTNode* root) {
	if (!root) {
		return;
	}

	this->destroy(root->left);
	this->destroy(root->right);
	delete root->left;
	delete root->right;
}

bool RBTree::search(std::string word, RBTNode* root) {
	if (!root) {
		return false;
	}

	if ((value(word) == value(root->word)) && (word == root->word)) {
		return true;
	}

	if (word < root->word) {
		return this->search(word, root->left);
	}
	else {
		return this->search(word, root->right);
	}
}

RBTNode* RBTree::rotateLeft(RBTNode* root) {
	RBTNode* p = root->right;

	if (root->right->left) {
		root->right->target = root->right->left->position;
	}
	else {
		root->right->target = root->right->position + sf::Vector2f(-50.0f, 0.0f);
	}

	root->right = root->right->left;

	if (p->left) {
		p->left->target = root->position;
	}

	p->left = root;

	p->red = p->left->red;
	p->left->red = true;

	root->target = root->position + sf::Vector2f(-50.0f, -30.0f);

	return p;
}

RBTNode* RBTree::rotateRight(RBTNode* root) {
	RBTNode* p = root->left;
	root->left = root->left->right;
	p->right = root;

	p->red = p->right->red;
	p->right->red = true;
	return p;
}

bool RBTree::isRed(RBTNode* node) {
	if (!node) {
		return false;
	}

	return node->red;
}

void RBTNode::update(long long int millis) {
	float factor = (millis) * 75.0f;
	position = lerp(position, target, factor);
}

/*
 * Public Functions
*/


void RBTNode::draw(sf::RenderWindow* window) {

	if (this->red) {
		shape.setFillColor(sf::Color::Red);
	}
	else {
		shape.setFillColor(sf::Color::Black);
	}

	shape.setPosition(position);

	window->draw(shape);
}

RBTree::RBTree(Renderer* renderer) {
	this->root = nullptr;
	this->renderer = renderer;
}

RBTree::~RBTree() {
	this->destroy(this->root);
}

void RBTree::insert(std::string word) {
	this->root = this->insert(word, this->root);
	this->root->red = false;
}

int RBTree::height() {
	return this->height(this->root);
}

void RBTree::preorder(std::ostream& os) {
	this->preorder(this->root, os);
	os << "\n";
}

void RBTree::inorder(std::ostream& os) {
	this->inorder(this->root, os);
	os << "\n";
}

void RBTree::postorder(std::ostream& os) {
	this->postorder(this->root, os);
	os << "\n";
}

bool RBTree::search(std::string word) {
	return this->search(word, this->root);
}

void RBTree::clear() {
	this->destroy(this->root);
	this->root = nullptr;
}

void RBTree::Update(long long int millis) {
	for (RBTNode* node : nodes) {
		node->update(millis);
	}
}

void RBTree::Draw(sf::RenderWindow* window) {

}

/*
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

	float factor = (millis / (1000.0f)) * 1.0f;
	shape.setPosition((Vector2f)lerp(shape.getPosition(), target, factor));
}

void RBTNode::setTarget(sf::Vector2f t) {
	target = t;
}

*/
// -----------RBTree------------

/*
RBTree::RBTree(Renderer* renderer) {
	this->window = renderer->getWindow();
	this->renderer = renderer;
}

RBTree::~RBTree() {

	for (int i = nodes.size() - 1; i >= 0; i--) {
		delete nodes[i];
	}


	if (!root) return;
	//delete root;
}

void RBTree::Draw(sf::RenderWindow* window) {
	if (!root) return;

	//root->draw(window);

}

void RBTree::Update(long long int millis) {
	if (!root) return;

	for (RBTNode* node : nodes) {
		node->update(millis);
	}

}

void RBTree::insert(std::string in) {
	if (!root) {
		root = new RBTNode(renderer, in);
		nodes.push_back(root);
		return;
	}
	nodes.push_back(new RBTNode(renderer, in));
}
*/