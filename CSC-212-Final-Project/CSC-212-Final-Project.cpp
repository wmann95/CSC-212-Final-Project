#include "RBTree.h"

#include <string>
#include <iostream> 
#include <functional>
#include <ostream>
#include <sstream>
#include <fstream>
#include <cctype>
#include <algorithm>

std::vector<std::string> ReadFile(std::string file_name);

int main(int argc, char* argv[]) {

	std::string title = "Left-Leaning Red-Black Trees";

	sf::RenderWindow window(sf::VideoMode(800, 600),title);
	Renderer renderer(&window);
	RBTree tree(&renderer);

	// get the wordlist
	std::vector<std::string> words = ReadFile(argv[1]);
	int counter = 0;

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
	sf::Clock clock;
	sf::Vector2f target;

	long int deltaTime = 0;
	long long int millis = clock.getElapsedTime().asMilliseconds();

	std::string testWord = "supercalifragalisticexpialidocious";

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
				if (event.key.code == sf::Keyboard::Space && counter < words.size()) {
					std::string st = "";
					//st.push_back(testWord[(counter++) % testWord.size()]);
					//tree.insert(st);

					tree.insert(words[counter++ % words.size()]);
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
			tree.UpdateNodeTargets();
		}

		// Shows current frames per second and updates per second.
		if (statClock >= 1000) {
			//std::cout << "FPS: " << frames << ", UPS: " << updates << std::endl;

			window.setTitle(title + " || FPS: " + std::to_string(frames) + ", UPS: " + std::to_string(updates));
			tree.insert(words[counter++ % words.size()]);


			statClock = 0;
			frames = 0;
			updates = 0;

		}

	}

	tree.inorder();
	tree.preorder();
	tree.postorder();

	return 0;
}

std::vector<std::string> ReadFile(std::string file_name) {
	// Create the input filestream - opens the file & prepares it for reading
	std::ifstream file(file_name);

	// place to store the words of the text
	std::vector<std::string> buffer;

	// Temporary string to hold a single line of the file
	std::string str;

	std::string const delimiters = " ,'\"-?!%&*^()[]@_;:./\\";

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

			buffer.push_back(word);
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

	file.close();

	return buffer;
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