/*
  Copyright (c) 2017 Nikita Kogut (vk.com/mronlinecoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
  All rights reserved.
  
  Only original author is allowed to edit and share this file.
  Unauthorized copying or editing this file, via any medium is strictly prohibited
  Proprietary and confidential

*/

#include <SFML/Graphics.hpp>
#include "level.h"
#include "Menu.h"
#include "Game.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Redstay");
	window.setFramerateLimit(60);
	sf::Texture tex;

	bool playing = false;

	Menu menu;
	menu.init();

	Game game;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

			if (event.type == sf::Event::EventType::KeyReleased) {
				if (event.key.code == sf::Keyboard::F10) 
					window.close();
			}

			if (!playing) {
				menu.handleEvent(event);
			} else {
				game.handleEvent(event);
			}
        }
		/*
			Update
		*/

		if (!playing) {
			menu.update();
			if (menu.startClicked) {
				game.init();
				playing = true;
			}
		} else {
			game.update();
		}

		/*
			Draw
		*/
        window.clear();
		if (!playing) {
			menu.draw(window);
		} else {
			game.draw(window);
		}
        window.display();
    }

    return 0;
}