#include "RBTree.h"

#include <string>
#include <iostream> 
#include <functional>
#include <ostream>
#include <sstream>
#include <fstream>
#include <cctype>
#include <cmath>
#include <algorithm>
#include <SFML/Window/Keyboard.hpp>

std::vector<std::string> ReadFile(std::string file_name);
double zeta(double n, double s);

int main(int argc, char* argv[]) {
	
	std::vector<std::string> words;

	if (argc == 1) {

		// Request the user input a book to be read in.
		std::string filename;

		std::cout << "Default available books: " << std::endl;
		std::cout << "greatgatsby.txt" << std::endl;
		std::cout << "junglebook.txt" << std::endl;
		std::cout << "samiam.txt" << std::endl;
		std::cout << std::endl;
		std::cout << "Please choose a book, or type the name of the file you added with the .exe: " << std::endl;

		std::cin >> filename;
		std::cout << std::endl;

		filename = "books/" + filename;

		words = ReadFile(filename); // get the wordlist.
	}
	else {
		std::string filename = "/books/" + std::string(argv[1]);
		words = ReadFile(filename); // get the wordlist.
	}

	// Title the window should take. This will be used to easily add a FPS and UPS counter to the window title.
	std::string title = "Left-Leaning Red-Black Trees";

	// Window size in pixels.
	int xSize = 800, ySize = 600;

	sf::RenderWindow window(sf::VideoMode(xSize, ySize), title); // Open up a window of size xSize by ySize, and set the handle of the window to title.

	Renderer renderer(&window); // Create the renderer and pass through the window pointer. This will be where the actual rendering is handled.

	RBTree tree(&renderer); // Create the tree, and pass in the renderer pointer. This is required because as the nodes are created, they need to register themselves
							// to the renderer.

	int counter = 0; // used to increment through the word list.

	// How many frames per second and updates per second that should be done.
	int fps = 60;
	int ups = 60;

	// How many frames/updates have happened since last frame/update.
	int updates = 0;
	int frames = 0;
	
	// timers that have deltaTime added to them.
	int fpsTimer = 0;
	int upsTimer = 0;
	int statClock = 0;
	int textClock = 0;
	int endAnimLimit = 0;

	sf::Clock clock; // Create a clock that will be used to time how long each run loop takes.

	long int deltaTime = 0; // Counts how many milliseconds have passed.
	long long int millis = clock.getElapsedTime().asMilliseconds(); // Gets the current time in milliseconds since the last restart.

	tree.insert(words[counter++ % words.size()]); // Insert a single word to ensure there is one node already on screen to show the animation.

	// Flags used to manage the visualization.
	bool pauseFlag = false; // Should words be getting put into the tree?
	bool scrollLock = false; // Should the scroll (and movement) be locked? Used in the final animation.
	bool endAnimShouldPlay = false; // Should the end animation play?
	bool endAnimComplete = false; // Is the end animation done?


	// Run loop
	while (window.isOpen()) {
		float camSpeed = 500.0f;
		sf::Event event;

		// Events are things that happen to the window. Keystrokes, mouse clicks, window closings, window focusing, etc.
		while (window.pollEvent(event)) {
			// Clicking the 'x' button on the window causes this event.
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			// Check if the mouse wheel was moved.
			if (event.type == sf::Event::MouseWheelMoved && !scrollLock) {
				if (event.mouseWheel.delta < 0) {
					renderer.increaseScroll();
				}
				else {
					renderer.decreaseScroll();
				}
			}
			// Check if any keys have been pressed.
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Space) {
					pauseFlag = !pauseFlag; // Space pressed, pause the visualization.
				}
				if (event.key.code == sf::Keyboard::Escape) {
					window.close(); // Escape pressed, close the window and end.
				}
			}
		}

		// find the time since the last loop, if applicable, then reset timer to this loop time.
		deltaTime = clock.getElapsedTime().asMilliseconds() - millis;
		millis = clock.getElapsedTime().asMilliseconds();

		// increment timers and clocks by deltatime.
		fpsTimer += deltaTime;
		upsTimer += deltaTime;
		statClock += deltaTime;
		textClock += deltaTime;

		if (endAnimShouldPlay) {
			endAnimLimit += deltaTime;
		}

		// Limits the updates to being the required updates per second.
		if (upsTimer >= 1000 / ups) {
			// Do things that need updating (Animation movements)

			// So long as the user is allowed to move, change the camera target based on what keys are pressed.
			// Doing the keyhandling here allows us to skip over some operating system backend stuff that makes
			// keystrokes a separate thing than keys being pressed. For instance, open a text file and try holding
			// a key down. The key will type once, then pause, then start typing over and over. That pause is unwanted.
			if (!scrollLock) {

				float camMove = camSpeed * deltaTime / 1000.0f; // take the base cam speed, multiply it by the change in time.
				sf::Vector2f camTargetOffset;

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
					camTargetOffset.y += -camMove;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
					camTargetOffset.y += camMove;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
					camTargetOffset.x += -camMove;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
					camTargetOffset.x += camMove;
				}

				renderer.moveCam(camTargetOffset);
			}
			tree.Update(deltaTime); // Updates the tree nodes. This is where the nodes figure out where they ought to be.

			
			// The end animation is comprised of a scroll out and a camera movement back to the center of the tree.
			if (endAnimShouldPlay && !endAnimComplete) { // While the end animation should play and hasn't been complete.
				
				renderer.setCurrWord("Tree Completed");

				if (endAnimLimit >= 2500) { // Wait 2.5 seconds then finish anim.
					endAnimComplete = true;
					scrollLock = false;
					std::cout << "Anim done." << std::endl;
				}
			}

			renderer.Update(deltaTime); // move camera and such

			updates++;
			upsTimer = 0;
		}

		if (fpsTimer >= 1000 / fps) {
			// Clear the window
			window.clear(sf::Color(171.0f, 204.0f, 214.0f, 255.0f));

			renderer.Render(); // Let the renderer do its thing

			// Show drawn stuff
			window.display();

			// Reset frame timer and add a frame to count.
			fpsTimer = 0;
			frames++;
		}

		// insert a word every 2/10's of a second.
		if (textClock >= 10 && counter < words.size()) {
			if (!pauseFlag){
				std::string tmp = words[counter++ % words.size()];
				tree.insert(tmp);
				renderer.setCurrWord(tmp);
			}
			textClock = 0;
		}
		
		// zoom out to show whole tree
		if (counter >= words.size() && !endAnimShouldPlay) {
			scrollLock = true;
			endAnimShouldPlay = true;

			renderer.PlayEnd(&tree);

		}

		// Shows current frames per second and updates per second.
		if (statClock >= 1000) {
			//std::cout << "FPS: " << frames << ", UPS: " << updates << std::endl;

			window.setTitle(title + " || FPS: " + std::to_string(frames) + ", UPS: " + std::to_string(updates));


			statClock = 0;
			frames = 0;
			updates = 0;

		}

	}

	// Zipfian analysis
	std::cout << "Zipf" << std::endl;

	std::vector<std::pair<std::string, int>> wordCounts;
	std::vector<int> zipfPredict;

	tree.getWordCounts(&wordCounts);

	// sort the wordList using a lambda comparator comparing the counts.
	std::sort(wordCounts.begin(), wordCounts.end(), [](std::pair<std::string, int> p1, std::pair<std::string, int> p2) {return p1.second > p2.second; });

	zipfPredict.push_back(wordCounts[0].second);

	// Describes the 'order' of the elements in place. This value changes from book to book, but 1.1 was found to provide more accurate predictive capability.
	double s = 1.1;

	// Make predictions based on index in the list and the count of the first word
	for (int i = 1; i < wordCounts.size(); i++) {
		zipfPredict.push_back(std::ceil(((1.0 / pow(i + 1.0, s)) / (zeta(words.size(), s))) * wordCounts[0].second));
	}

	std::ofstream out;

	out.open("output.txt");

	for (int i = 0; i < wordCounts.size(); i++) {
		std::pair<std::string, int> p = wordCounts[i];
		out << "'" << p.first << "' count: " << p.second << " || Zipfian prediction: " << zipfPredict[i] << " || Zipfian accuracy: " << (zipfPredict[i] > p.second ? (double)p.second / (double)zipfPredict[i] : (double)zipfPredict[i] / (double)p.second) << std::endl;
	}

	out.close();

	return 0;
}

//This is not a true Reimann Zeta summation, as it doesn't go to infinity. Instead, it goes to the number of elements in the set.
double zeta(double n, double s)
{
	double output = 0;

	for (int i = 1; i < n; i++) {
		output += 1 / pow(n, s);
	}

	return output;
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
	}

	file.close();

	return buffer;
}
