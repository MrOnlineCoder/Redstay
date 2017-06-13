/*
  Copyright (c) 2017 Nikita Kogut (vk.com/mronlinecoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
  All rights reserved.
  
  Only original author is allowed to edit and share this file.
  Unauthorized copying or editing this file, via any medium is strictly prohibited
  Proprietary and confidential

*/

#include "Menu.h"
#include <SFML/Graphics.hpp>
#include <random>
#include <ctime>

Menu::Menu() {

}

void Menu::draw(sf::RenderWindow& window) {
	window.clear(sf::Color::White);
	window.draw(fg);
	window.draw(logo);
	window.draw(credits);
	for (int i=0;i<lines.size();i++) {
		lines[i].move(2, 0);
		window.draw(lines[i]);
		if (lines[i].getPosition().x > 1024) {
			lines[i].move(-1040, 0);
		}
	}
}

void Menu::init() {
	srand(time(0));
	f.loadFromFile("data/fonts/Roboto-Thin.ttf");
	logoFont.loadFromFile("data/fonts/Roboto-Regular.ttf");

	fg.setSize(sf::Vector2f(1024, 768));
	fg.setFillColor(sf::Color::Black);
	lines.clear();

	logo.setFont(logoFont);
	logo.setString("Redstay");
	logo.setCharacterSize(32);
	logo.setPosition(50,50);
	logo.setFillColor(sf::Color::Black);
	logo.setStyle(sf::Text::Underlined);

	credits.setFont(f);
	credits.setString("Created by MrOnlineCoder\nMusic and SFX by Eric Matyas and Kevin MacLeod");
	credits.setFillColor(sf::Color::Black);
	credits.setPosition(50, 768-credits.getGlobalBounds().height-50);

	music.openFromFile("data/music/menu.ogg");
	music.setLoop(true);
	music.play();

	for (int i=0;i<30;i++) {
		sf::RectangleShape line;
		line.setSize(sf::Vector2f(20,1));
		if (i % 2 == 0) { 
			line.setFillColor(sf::Color::Black);
		} else {
			line.setFillColor(sf::Color::Black);
		}
		line.setPosition(rand() % 1024, rand() % 768);
		lines.push_back(line);
	}

	startClicked = false;
}

void Menu::update() {
	if (fg.getFillColor().a != 0 && c.getElapsedTime().asMilliseconds() > 40) {
		sf::Color color = fg.getFillColor();
		if (color.a != 0) color.a--;
		fg.setFillColor(color);
		c.restart();
	}
}

void Menu::handleEvent(sf::Event e) {
	if (e.type == sf::Event::KeyReleased) {
		if (e.key.code == sf::Keyboard::Return) {
			music.stop();
			startClicked = true;
		}
	}
}