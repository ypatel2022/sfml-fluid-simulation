#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "Fluid.h"

#define IX(x, y, N) ((x) + (y) * N)


int WIDTH = 1280;
int HEIGHT = 720;
sf::Color BACKGROUND_COLOR(30, 30, 30);

int main()
{
	// create a window & settings
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT, 32), "Fluid Simulation");
	sf::View view = window.getDefaultView();
	window.setFramerateLimit(144);


	// for zooming in/out & panning view
	sf::Vector2f old_pos;
	bool moving = false;
	float zoom = 1;

	// while the window is kept open
	while (window.isOpen())
	{
		// store and loop through each event
		sf::Event event;
		while (window.pollEvent(event))
		{

			// if closing window
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::KeyReleased:
				if (event.key.code == sf::Keyboard::Escape)
				{
					window.close();
				}

				if (event.key.code == sf::Keyboard::Space) {}

				break;

			case sf::Event::MouseButtonPressed:
				// Mouse button is pressed, get the position and set moving as active
				if (event.mouseButton.button == 0)
				{
					moving = true;
					old_pos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
				}
				break;

			case  sf::Event::MouseButtonReleased:
				// Mouse button is released, no longer move
				if (event.mouseButton.button == 0)
				{
					moving = false;
				}
				break;

			case sf::Event::MouseMoved:
			{
				// Ignore mouse movement unless a button is pressed (see above)
				if (!moving)
					break;
				// Determine the new position in world coordinates
				const sf::Vector2f newPos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
				// Determine how the cursor has moved
				// Swap these to invert the movement direction
				const sf::Vector2f deltaPos = old_pos - newPos;

				// Move our view accordingly and update the window
				view.setCenter(view.getCenter() + deltaPos);
				window.setView(view);

				// Save the new position as the old one
				// We're recalculating this, since we've changed the view
				old_pos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
				break;

			}
			case sf::Event::MouseWheelScrolled:
				// Ignore the mouse wheel unless we're not moving
				if (moving)
					break;

				// Determine the scroll direction and adjust the zoom level
				// Again, you can swap these to invert the direction
				if (event.mouseWheelScroll.delta <= -1)
					zoom = std::min(2.f, zoom + .1f);
				else if (event.mouseWheelScroll.delta >= 1)
					zoom = std::max(.5f, zoom - .1f);

				// Update our view
				view.setSize(window.getDefaultView().getSize()); // Reset the size
				view.zoom(zoom); // Apply the zoom level (this transforms the view)
				window.setView(view);
				break;
			}
		}


		// clear frame
		window.clear(BACKGROUND_COLOR);
		
		


		// show frame / draw calls
		window.display();
	}

	return 0;
}