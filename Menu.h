/*
  Copyright (c) 2017 Nikita Kogut (vk.com/mronlinecoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
  All rights reserved.
  
  Only original author is allowed to edit and share this file.
  Unauthorized copying or editing this file, via any medium is strictly prohibited
  Proprietary and confidential

*/

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

class Menu {
	public:
		void draw(sf::RenderWindow& window);
		void update();
		void init();
		void handleEvent(sf::Event e);

		bool exitClicked;
		bool startClicked;

		Menu();
	
	private:
		sf::Font f;
		sf::Font logoFont;
		sf::Text logo;
		sf::Text credits;
		sf::Text info;
		sf::RectangleShape fg;
		sf::Clock c;
		sf::Music music;
		std::vector<sf::RectangleShape> lines;
};