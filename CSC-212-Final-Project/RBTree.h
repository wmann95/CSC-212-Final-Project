#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>


class RBTree {


private:

	class RBTNode {
	private:
		bool red;
		int x, y;
		sf::CircleShape shape;
		sf::Vector2f target;
	public:
		RBTNode() {
			shape = sf::CircleShape(20.0f);
			target = sf::Vector2f(0.0f, 0.0f);
		}

		void draw(sf::RenderWindow* window);
		void update(long long int millis);
		void setTarget(sf::Vector2f target);

		friend class RBTree;
	};



	RBTNode* root;
	int x, y;

public:
	RBTree() {
		root = new RBTNode;


	}
	~RBTree() {
		//destroy();
		delete root;
	}

	void Draw(sf::RenderWindow* window);
	void Update(long long int millis);

	void debugSetTarget(sf::Vector2f target) {
		root->setTarget(target);
	}
};