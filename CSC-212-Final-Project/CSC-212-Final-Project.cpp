#include "RBTree.h"

#include <string>
#include <iostream> 
#include <functional>
#include <ostream>
#include <sstream>
#include <fstream>
#include <cctype>
#include <algorithm>

void ReadFile(std::string file_name, RBTree* tree);
std::vector<std::string> split(std::string line, std::string delimiters);

int main(int argc, char* argv[]) {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Left-Leaning Red-Black Trees");
	Renderer renderer(&window);
	RBTree tree(&renderer);

	ReadFile(argv[1], &tree);
	/*tree.insert("a");
	tree.insert("b");
	tree.insert("c");
	tree.insert("d");
	tree.insert("e");
	tree.inorder();*/
	//std::cout << tree.height() << std::endl;

	tree.inorder();
}

std::vector<std::string> split(std::string line, std::string delimiters) {

	std::stringstream stream(line);
	std::string buffer;

	std::vector<std::string> words;

	while (stream >> buffer) {

		//std::string b = buffer;

		std::transform(buffer.begin(), buffer.end(), buffer.begin(), [delimiters](char c) {
			// check if the character is a delimiter, if it is, return null character, otherwise return the character.
			if (c == '!' || c == ',' || c == ' ' || c == '\n' || c == '\t' || c == '?' || c == '\v' || c == '.' || c == '\r') {
				return '\0';
			}
			return c;
		});


		if (buffer == "") {
			continue;
		}


		words.push_back(buffer);
	}
	
	return words;
}

void ReadFile(std::string file_name, RBTree* tree) {
	// Create the input filestream - opens the file & prepares it for reading
	std::ifstream file(file_name);

	// Temporary string to hold a single line of the file
	std::string str;

	std::string const delimiters = " ,'\"-?!%&*^()@_;:./\\";

	// Reads all lines in the file, 1 at at time
	while (std::getline(file, str)) {

		std::stringstream stream(str);
		std::string word;

		while(stream >> word) {

			//std::cout << word << std::endl;


			while (delimiters.find(word[0]) != std::string::npos) {
				word = word.substr(1, word.size() - 1);
			}
			if (word == "") continue;
			while (delimiters.find(word[word.size() - 1]) != std::string::npos) {
				word = word.substr(0, word.size() - 1);
			}

			// em dash, not a single character delimiter, needs to be handled separately
			size_t em = word.find("--");
			if (em != std::string::npos) {
				std::string word1 = word.substr(0, em);
				std::string word2 = word.substr(em, word.size() - 1);
				stream << word1 << word2;
				continue;
			}

			// This line uses the transform method in the standard library algorithm to go through each character
			// in a string and convert it to a lowercase using a lambda function that takes the character in, and
			// returns (to the transform method, and thereby that specific character) a lowercase character.
			std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c) { return std::tolower(c); });

			tree->insert(word);
		}

		/* // Converts our string into a stringstream
		std::istringstream ss(str);
		 // Temp double to store a converted value from a line
		std::string token;
		  // Reads all values from the stringstream (current row), converts to double
		  while(getline(ss, token)){
			  for (int i = 0; i < token; i++){

			  }
			  // Adds the converted value to the row
			  (*tree).insert(token);
		  }
	  }*/
	}
}

/*
void ReadFile(std::string file_name, RBTree* tree) {
	// Create the input filestream - opens the file & prepares it for reading
	std::ifstream file(file_name);

	

	// Temporary string to hold a single line of the file
	std::string str;

	std::string const delimiters = "" + ',' + '\'' + '"' + '-' + '?' + '\!' + '%' + '&' + '*' + '^' + '(' + ')' + '@' + '_' + ';' + ':' + '.' + '/' + '\\' + '\n';

	// Reads all lines in the file, 1 at at time
	while (file >> str) {

		std::vector<std::string> words = split(str, delimiters);

		for (int i = 0; i < words.size(); i++) {
			// This line uses the transform method in the standard library algorithm to go through each character
			// in a string and convert it to a lowercase using a lambda function that takes the character in, and
			// returns (to the transform method, and thereby that specific character) a lowercase character.
			std::transform(words[i].begin(), words[i].end(), words[i].begin(), [](unsigned char c) { return std::tolower(c); });

			tree->insert(words[i]);
		}

		/* // Converts our string into a stringstream
		std::istringstream ss(str);
		 // Temp double to store a converted value from a line
		std::string token;
		  // Reads all values from the stringstream (current row), converts to double
		  while(getline(ss, token)){
			  for (int i = 0; i < token; i++){

			  }
			  // Adds the converted value to the row
			  (*tree).insert(token);
		  }
	  }
	}
}
*/

/*
#include "RBTree.h"
#include "Renderer.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>

int main()
{

	sf::RenderWindow window(sf::VideoMode(800, 600), "Left-Leaning Red-Black Trees");
	Renderer renderer(&window);
	RBTree tree(&renderer);

	// How many frames per second and updates per second that should be done.
	int fps = 60;
	int ups = 30;

	// How many frames/updates have happened since last frame/update
	int updates = 0;
	int frames = 0;

	// timers that have deltaTime added to them
	int fpsTimer = 0;
	int upsTimer = 0;
	int statClock = 0;

	int testCounter = 0;

	sf::Clock clock;
	sf::Vector2f target;

	long int deltaTime = 0;
	long long int millis = clock.getElapsedTime().asMilliseconds();

	// Run loop
	while (window.isOpen()) {
		sf::Event event;

		// While there are events in the event queue.
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed) {
				target = (sf::Vector2f)sf::Mouse::getPosition(window);
				//tree.debugSetTarget(target);
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Space) {
					std::string s(1, char(97 + testCounter++));
					tree.insert(s);
				}
			}
		}

		// find the time since the last loop, if applicable, then reset timer to this loop time.
		deltaTime = clock.getElapsedTime().asMilliseconds() - millis;
		millis = clock.getElapsedTime().asMilliseconds();

		fpsTimer += deltaTime;
		upsTimer += deltaTime;
		statClock += deltaTime;

		if (fpsTimer >= 1000 / fps) {
			// Clear the window
			window.clear(sf::Color(171.0f, 204.0f, 214.0f, 255.0f));

			// Draw stuff here
			tree.Draw(&window);

			renderer.Render();

			// Show drawn stuff
			window.display();


			// Reset frame timer and add a frame to count.
			fpsTimer = 0;
			frames++;
		}

		if (upsTimer >= 1000 / ups) {
			// Do things that need updating (Animation movements)
			tree.Update(deltaTime);
		}

		// Shows current frames per second and updates per second.
		if (statClock >= 1000) {
			std::cout << "FPS: " << frames << ", UPS: " << updates << std::endl;
			statClock = 0;
			frames = 0;
			updates = 0;

		}

	}

	std::cout << "Preorder" << std::endl;
	tree.preorder();

	std::cout << "Inorder" << std::endl;
	tree.inorder();

	std::cout << "Postorder" << std::endl;
	tree.postorder();

	return 0;
}
*/