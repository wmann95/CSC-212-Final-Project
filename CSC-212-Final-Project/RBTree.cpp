#include "RBTree.h"

RBTNode::RBTNode(Renderer* renderer, RBTNode* p) {
	this->counter = 0;
	this->left = nullptr;
	this->right = nullptr;
	red = true;
	shape = sf::CircleShape(shapeSize);

	renderer->RegisterNode(this);
	parent = p;

	// select the font
	text.setFont(*(renderer->getFont())); // font is a sf::Font

	// set the string to display
	text.setString("EMPTY WTF???");

	// set the character size
	text.setCharacterSize(24); // in pixels, not points!

	// set the color
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);

	// set the text style
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);
}

// Creates a Node
RBTNode::RBTNode(Renderer* renderer, RBTNode* p, std::string word, bool color) {
	
	this->word = word;
	this->counter = 1;
	this->left = nullptr;
	this->right = nullptr;
	red = color;
	shape = sf::CircleShape(shapeSize);
	renderer->RegisterNode(this);
	parent = p;
	// select the font
	text.setFont(*(renderer->getFont())); // font is a sf::Font

	// set the string to display
	text.setString(word);

	// set the character size
	text.setCharacterSize(24); // in pixels, not points!

	// set the color
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(2.0f);

	// set the text style
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);
}

// returns true if word1 comes before word2. 
bool areWordsInOrder(std::string word1, std::string word2) {;
	return word1 < word2;
}

// Inserts a Node
RBTNode* RBTree::insert(std::string word, RBTNode* root, RBTNode* prev) {
	
	// checks if a root is found in the tree
	if (!root) {
		RBTNode* node = new RBTNode(renderer, prev, word, true);
		nodes.push_back(node);

		return node;
	}
	
	// checks if word that is trying to be inserted is found in tree
	if (word == root->word) {
		// if found then add to counter and do not make new node
		root->counter++;
		return root;
	}
	
	// checks if word is less than the comparison node
	else if (word < root->word) {
		// goes to the left
		root->left = insert(word, root->left, root);
	}
	else {
		// goes to the right
		root->right = insert(word, root->right, root);
	}
	
	// checks if left rotation is necessary
	if (isRed(root->right) && !isRed(root->left)) {
		root = rotateLeft(root);
	}
	
	// checks if right rotation is necessary
	if (isRed(root->left) && isRed(root->left->left)) {
		root = rotateRight(root);
	}
	
	// checks if color flip is necessary
	if (isRed(root->left) && isRed(root->right)) {
		root->red = true;
		root->left->red = false;
		root->right->red = false;
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

void RBTree::updateTargets(RBTNode* root, RBTNode* prev, int baseWidth) {

	if (!root) {
		return;
	}

	if (!prev) {
		// ensure that the root is at the start and has no parent. The parent is for the line rendering.
		root->target = sf::Vector2f(xStart, yStart);
		root->parent = nullptr;
	}
	else {
		root->parent = prev;
	}

	float xOff = (float)(baseWidth) / 2;
	float yOff = yOffset * log2f(baseWidth + 1);

	if (root->left) {
		root->left->target = root->target + sf::Vector2f(-xOff * xOffset, yOff);
	}

	if (root->right) {
		root->right->target = root->target + sf::Vector2f(xOff * xOffset, yOff);
	}

	updateTargets(root->left, root, xOff);
	updateTargets(root->right, root, xOff);

}

void RBTree::Update(long long int millis) {
	UpdateNodeTargets();
	for (RBTNode* node : nodes) {
		node->update(millis);
	}
}

void RBTree::UpdateNodeTargets() {
	updateTargets(root, nullptr, pow(2, height() - 1));
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

void RBTree::getWordCounts(RBTNode* root, std::vector<std::pair<std::string, int>>* in) {
	if (!root) {
		return;
	}

	getWordCounts(root->left, in);
	in->push_back(std::pair<std::string, int>(root->word, root->counter));
	getWordCounts(root->right, in);
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

// finds the count of how many times a word appears
int RBTree::count(RBTNode* root, std::string key) {
	
	// checks if there is a root node
	if (!root) {
		return 0;
	}
	
	// once the key is equal to a word, returns the count
	if (key == root->word){
		return root->counter;
	}
  
	// goes to the left
	if (areWordsInOrder(key, root->word)) {
		return count(root->left, key);
	}
	
	//goes to the right
	else {
		return count(root->right, key);
	}
}

// returns count with the root and key
int RBTree::count(std::string key) {
	return count(root, key);
}

// destroys the tree from the leaves up
void RBTree::destroy(RBTNode* root) {
	// stops once there is no root
	if (!root) {
		return;
	}
	 // deletes nodes
	this->destroy(root->left);
	this->destroy(root->right);
	delete root->left;
	delete root->right;
}

// searches tree for specific word
bool RBTree::search(std::string word, RBTNode* root) {
	
	// checks of there is a root
	if (!root) {
		return false;
	}
	
	// checks if words are the same
	if (word == root->word) {
		return true;
	}
	
	// checks child node to the left
	if (word < root->word) {
		return this->search(word, root->left);
	}
	
	// checks child node to the right
	else {
		return this->search(word, root->right);
	}
}

// performs a left loation
RBTNode* RBTree::rotateLeft(RBTNode* root) {
	
	// changes directions of nodes
	RBTNode* p = root->right;
	root->right = root->right->left;
	p->left = root;
	
	// color flips
	p->red = p->left->red;
	p->left->red = true;

	return p;
}

// performs a right rotation
RBTNode* RBTree::rotateRight(RBTNode* root) {
	
	//changes direction of nodes
	RBTNode* p = root->left;
	root->left = root->left->right;
	p->right = root;
	
	//color flips
	p->red = p->right->red;
	p->right->red = true;
	return p;
}

// checks the color of the node
bool RBTree::isRed(RBTNode* node) {
	if (!node) {
		return false;
	}

	return node->red;
}

void RBTNode::update(long long int millis) {
	float factor = 1/((millis) + 10.0f);
	position = lerp(position, target, factor);
}

/*
 * Public Functions
*/

void RBTNode::draw(sf::RenderWindow* window) {

	shape.setPosition(position - sf::Vector2f(shapeSize, shapeSize));
	text.setPosition(position - sf::Vector2f(shapeSize, shapeSize));

	if (this->red) {
		shape.setFillColor(sf::Color::Red);
	}
	else {
		shape.setFillColor(sf::Color::Black);
	}


	window->draw(shape);
	window->draw(text);
}

RBTree::RBTree(Renderer* renderer) {
	this->root = nullptr;
	this->renderer = renderer;
}

RBTree::~RBTree() {
	this->destroy(this->root);
}

void RBTree::insert(std::string word) {
	this->root = this->insert(word, this->root, nullptr);
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

void RBTree::getWordCounts(std::vector<std::pair<std::string, int>>* in) {
	getWordCounts(root, in);
}

void RBTree::postorder(std::ostream& os) {
	this->postorder(this->root, os);
	os << "\n";
}

bool RBTree::search(std::string word) {
	std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c) { return tolower(c); });
	return this->search(word, this->root);
}

void RBTree::clear() {
	this->destroy(this->root);
	this->root = nullptr;
}
