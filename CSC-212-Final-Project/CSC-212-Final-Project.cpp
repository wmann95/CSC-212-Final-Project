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

	// Title the window should take. This will be used to easily add a FPS and UPS counter to the window title.
	std::string title = "Left-Leaning Red-Black Trees";

	// Window size in pixels.
	int xSize = 800, ySize = 600;

	sf::RenderWindow window(sf::VideoMode(xSize, ySize), title); // Open up a window of size xSize by ySize, and set the handle of the window to title.
	sf::View view; // Create a view. This is essentially a camera in the world.
	view.reset(sf::FloatRect(-400.0f, -25.0f, 800.0f, 600.0f)); // Resetting the view means that it will be the same size as the window, so the "scale" is one-to-one.
																// This is also offsetting the camera by half the x-width and up a little bit.

	Renderer renderer(&window); // Create the renderer and pass through the window pointer. This will be where the actual rendering is handled.

	RBTree tree(&renderer); // Create the tree, and pass in the renderer pointer. This is required because as the nodes are created, they need to register themselves
							// to the renderer.

	std::vector<std::string> words = ReadFile(argv[1]); // get the wordlist.
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


	sf::Vector2f scroll(3.0f, 3.0f); // This is where the camera scroll is handled. Having this set to 3.0 in both x and y with different scrollTarget will cause a small animation on start.
	sf::Vector2f scrollTarget(1.0f, 1.0f); // This is where the scroll is trying to get to.

	sf::Vector2f cam(0.0f , 100.0f); // This is the current position of the view. Similar to the scroll, having a different camTarget will cause the cam to move towards that target.
	sf::Vector2f camTarget(0, 250.0f); // Where the cam is trying to get.

	tree.insert(words[counter++ % words.size()]); // Insert a single word to ensure there is one node already on screen to show the animation.
	view.setSize(xSize * scroll.x, ySize * scroll.y); // Ensure the camera scroll is at the right scroll before doing any rendering.
	view.setCenter(cam); // Ensure the camera is in the correct place before rendering.

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
				// how much should each wheel move increase/decrease the zoom?
				float factor = 1.5f;
				
				// The mouse wheel delta will always be either -1 or 1, so raising the factor to this delta
				// will either scale the scrollTarget up, or shrink it down.
				scrollTarget.x *= pow(factor, -event.mouseWheel.delta);
				scrollTarget.y *= pow(factor, -event.mouseWheel.delta);
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

		// Limits the updates to being the required updates per second.
		if (upsTimer >= 1000 / ups) {
			// Do things that need updating (Animation movements)

			// So long as the user is allowed to move, change the camera target based on what keys are pressed.
			// Doing the keyhandling here allows us to skip over some operating system backend stuff that makes
			// keystrokes a separate thing than keys being pressed. For instance, open a text file and try holding
			// a key down. The key will type once, then pause, then start typing over and over. That pause is unwanted.
			if (!scrollLock) {

				camSpeed = camSpeed * scroll.x * 10.0f; // allows the camSpeed to increase based on the scroll level.

				float camMove = camSpeed * deltaTime / 1000.0f; // take the base cam speed, multiply it by the change in time.

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
					camTarget.y += -camMove;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
					camTarget.y += camMove;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
					camTarget.x += -camMove;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
					camTarget.x += camMove;
				}
			}
			tree.Update(deltaTime); // Updates the tree nodes. This is where the nodes figure out where they ought to be.

			scroll = lerp(scroll, scrollTarget, 0.8f * deltaTime * (endAnimShouldPlay ? 0.5f : 1.0f)); // Interpolate the scroll towards the scrollTarget by a factor of 0.8f * the deltaTime. One would assume that this is the same as setting the factor to 1,
																   // but that causes the scroll to be very janky. Using the deltatime smooths it out.

			view.setSize(xSize * scroll.x, ySize * scroll.y); // Change the view to be scaled according to the scroll.

			cam = lerp(cam, camTarget, 0.95f); // Move the camera closer to the camTarget by a factor of 0.95f.

			// The end animation is comprised of a scroll out and a camera movement back to the center of the tree.
			if (endAnimShouldPlay && !endAnimComplete) { // While the end animation should play and hasn't been complete.
				double dist = sqrt(pow(camTarget.x - cam.x, 2) + pow(camTarget.y - cam.y, 2));
				
				endAnimLimit += deltaTime;

				if (endAnimLimit >= 2500) { // Wait 2.5 seconds then finish anim.
					endAnimComplete = true;
					scrollLock = false;
					std::cout << "Anim done." << std::endl;
				}
			}

			view.setCenter(cam); // sets the views position to where the cam is. Using move would cause the view to constantly move, not track the cam position.

			updates++;
			upsTimer = 0;
		}

		if (fpsTimer >= 1000 / fps) {
			// Clear the window
			window.clear(sf::Color(171.0f, 204.0f, 214.0f, 255.0f));

			// Draw stuff here

			window.setView(view); // Ensures that the window is based around our current view.

			renderer.Render(); // Goes through all of the nodes and draws their shape, words, and lines to their parents.

			window.setView(window.getDefaultView()); // Reset the view back to the default. Keeps the window sane.

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
