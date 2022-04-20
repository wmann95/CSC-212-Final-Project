#include "RBTree.h"

#include <string>
#include <iostream> 
#include <functional>
#include <ostream>
#include <sstream>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <SFML/Window/Keyboard.hpp>

std::vector<std::string> ReadFile(std::string file_name);

int main(int argc, char* argv[]) {

	std::string title = "Left-Leaning Red-Black Trees";

	int xSize = 800, ySize = 600;

	sf::RenderWindow window(sf::VideoMode(xSize, ySize),title);
	sf::View view;
	view.reset(sf::FloatRect(-400.0f, -25.0f, 800.0f, 600.0f));

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
	int textClock = 0;
	sf::Clock clock;
	sf::Vector2f target;

	long int deltaTime = 0;
	long long int millis = clock.getElapsedTime().asMilliseconds();

	std::string testWord = "supercalifragalisticexpialidocious";


	sf::Vector2f scroll(3.0f, 3.0f);
	sf::Vector2f scrollTarget(1.0f, 1.0f);

	sf::Vector2f cam(0.0f , 100.0f);
	sf::Vector2f camTarget(0, 250.0f);

	tree.insert(words[counter++ % words.size()]);
	view.setSize(xSize * scroll.x, ySize * scroll.y);
	view.setCenter(cam);

	bool pauseFlag = false;
	bool scrollLock = false;
	bool endAnimShouldPlay = false;
	bool endAnimComplete = false;

	// Run loop
	while (window.isOpen()) {
		float camSpeed = 500.0f;
		sf::Event event;

		// While there are events in the event queue.
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::MouseWheelMoved && !scrollLock) {
				std::cout << event.mouseWheel.delta << std::endl;
				float factor = 1.5f;
				scrollTarget.x *= pow(factor, event.mouseWheel.delta);
				scrollTarget.y *= pow(factor, event.mouseWheel.delta);
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Space) {
					pauseFlag = !pauseFlag;
				}
			}
			if (event.type == sf::Event::MouseButtonPressed && !scrollLock) {

				sf::Vector2i mousePos = sf::Mouse::getPosition(window);

				camTarget += window.mapPixelToCoords(mousePos) - sf::Vector2f(view.getSize().x / 2, view.getSize().y / 2);

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

		if (upsTimer >= 1000 / ups) {
			// Do things that need updating (Animation movements)

			if (!scrollLock) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
					camTarget.y += -camSpeed * deltaTime / 1000.0f;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
					camTarget.y += camSpeed * deltaTime / 1000.0f;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
					camTarget.x += -camSpeed * deltaTime / 1000.0f;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
					camTarget.x += camSpeed * deltaTime / 1000.0f;
				}
			}
			tree.Update(deltaTime);
			scroll = lerp(scroll, scrollTarget, 0.8f * deltaTime);
			camSpeed = camSpeed / (scroll.x * scroll.x);
			view.setSize(xSize * scroll.x, ySize * scroll.y);

			cam = lerp(cam, camTarget, 0.95f);

			if (endAnimShouldPlay && !endAnimComplete) {
				float dist = sqrt(pow(camTarget.x - cam.x, 2) + pow(camTarget.y - cam.y, 2));
				//std::cout << dist << std::endl;

				if (dist < 0.0025f) {
					endAnimComplete = true;
					scrollLock = false;
					std::cout << "Anim done." << std::endl;
				}
			}

			view.setCenter(cam);
		}

		if (fpsTimer >= 1000 / fps) {
			// Clear the window
			window.clear(sf::Color(171.0f, 204.0f, 214.0f, 255.0f));

			// Draw stuff here

			window.setView(view);

			renderer.Render();

			window.setView(window.getDefaultView());

			// Show drawn stuff
			window.display();


			// Reset frame timer and add a frame to count.
			fpsTimer = 0;
			frames++;
		}

		// insert a word every 2/10's of a second.
		if (textClock >= 200 && counter < words.size()) {
			if(!pauseFlag) tree.insert(words[counter++ % words.size()]);
			textClock = 0;
		}
		
		// zoom out to show whole tree
		if (counter >= words.size() && !endAnimShouldPlay) {
			scrollLock = true;
			endAnimShouldPlay = true;

			int treeHeight = tree.height();
			float width = (pow(2, treeHeight) * tree.getNodeOffset().x) / xSize;
			float height = (float)treeHeight * tree.getNodeOffset().y;
			
			width *= 1.1f;

			scrollTarget = sf::Vector2f(width, width);
			camTarget = sf::Vector2f(0, height / 2);

			std::string w = "thee";

			std::cout << "'" << w << "' count: " << tree.count(w) << std::endl;
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
	}

	file.close();

	return buffer;
}
