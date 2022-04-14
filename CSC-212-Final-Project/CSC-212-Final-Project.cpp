#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include "RBTree.h"
#include "Renderer.h"

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
				tree.debugSetTarget(target);
			}
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Space) {
					tree.insert("Test");
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

	return 0;

}
