/*
  Copyright (c) 2017 Nikita Kogut (vk.com/mronlinecoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
  All rights reserved.
  
  Only original author is allowed to edit and share this file.
  Unauthorized copying or editing this file, via any medium is strictly prohibited
  Proprietary and confidential

*/

#include "Game.h"
#include "level.h"
#include <random>
#include <ctime>
#include "Constants.h"

#include <iostream>
#include <string>
#include <sstream>



std::string itos(int i) // convert int to string
{
    std::stringstream s;
    s << i;
    return s.str();
}

std::string ftos(float i) // convert int to string
{
    std::stringstream s;
    s << i;
    return s.str();
}


Game::Game(void)
{

}


Game::~Game(void)
{

}


void Game::init(void)
{
	srand(time(0));
	currentLevel = 0;
	flashing = false;
	debug = false;


	tracks.push_back("data/music/game.ogg");
	tracks.push_back("data/music/game2.ogg");
	tracks.push_back("data/music/game3.ogg");
	
	tips.push_back("Use A and D to move, W to jump");
	tips.push_back("Try to avoid Red Blocks");
	tips.push_back("Be patient.");
	tips.push_back("Sneaky Peaky!");
	tips.push_back("Sometimes you need a key to continue");
	tips.push_back("Teleports are real!");
	tips.push_back("Seems to be easy.");
	tips.push_back("Oh, crap! Too many teleports!");
	tips.push_back("Good Luck.\n-Mr.Gray");

	f.loadFromFile("data/fonts/Roboto-Regular.ttf");
	tip.setString(tips[currentLevel]);
	tip.setCharacterSize(18);
	tip.setPosition(32,3);
	tip.setFillColor(sf::Color::White);
	tip.setFont(f);


	newMusic();

	bgTex.loadFromFile("data/images/bg2.png");
	bg1.setTexture(bgTex);
	bg2.setTexture(bgTex);
	
	bg1.setPosition(0,-768);
	bg2.setPosition(0,0);

	deathBuff.loadFromFile("data/sfx/death.wav");
	deathSnd.setBuffer(deathBuff);

	okBuff.loadFromFile("data/sfx/complete.ogg");
	okSnd.setBuffer(okBuff);

	pickupBuff.loadFromFile("data/sfx/pickup.wav");
	pickupSnd.setBuffer(pickupBuff);

	tpBuff.loadFromFile("data/sfx/teleport.ogg");
	tpSnd.setBuffer(tpBuff);

	flash.setFillColor(sf::Color::White);
	flash.setSize(sf::Vector2f(1024,768));
	flash.setPosition(sf::Vector2f(0,0));

	keySpr.setTexture(lvl.tilesetImage);
	keySpr.setTextureRect(sf::IntRect(4 * Constants::TILESIZE, 0, Constants::TILESIZE, Constants::TILESIZE));

	tpSpr.setTexture(lvl.tilesetImage);
	tpSpr.setTextureRect(sf::IntRect(5 * Constants::TILESIZE, 0, Constants::TILESIZE, Constants::TILESIZE));

	tpSpr.setOrigin(sf::Vector2f(tpSpr.getLocalBounds().width/2, tpSpr.getLocalBounds().height/2 ));

	debugTxt.setFont(f);
	debugTxt.setPosition(36,36);
	debugTxt.setFillColor(sf::Color::White);
	debugTxt.setCharacterSize(16);


	loadLevel();
}

void Game::loadLevel() {

	teleports.clear();
	teleportsEnds.clear();

	std::string path = "data/levels/";
	path = path + itos(currentLevel);
	path = path + ".tmx";
	lvl.LoadFromFile(path);
	lvl.SetDrawingBounds(sf::FloatRect(0,0,1024+32, 768+32));

	Object o = lvl.GetObject("player");
	pl.spr.setPosition(o.rect.left, o.rect.top);

	Object endObj = lvl.GetObject("end");
	endPos.x = endObj.rect.left;
	endPos.y = endObj.rect.top;

	if (lvl.HasObject("key")) {
		isKey = true;
		key = false;
		Object keyObj = lvl.GetObject("key");
		keySpr.setPosition(keyObj.rect.left, keyObj.rect.top);
	} else {
		isKey = false;
		key = false;
	}

	std::vector<Object> objs = lvl.GetAllObjects();
	for(int i=0;i<objs.size();i++) {
		if (objs[i].type == "tpStart") {
			teleports.push_back(sf::Vector2f(objs[i].rect.left, objs[i].rect.top));
		}

		if (objs[i].type == "tpEnd") {
			sf::Vector2f pos(objs[i].rect.left, objs[i].rect.top);
			teleportsEnds.insert(std::pair<std::string,sf::Vector2f>(objs[i].name, pos));
		}
	}



	/*std::vector<Object> objs = lvl.GetAllObjects();
	for (int i=0;i<objs.size();i++) {
		if (objs[i].name == "damage") {
			blacks.push_back(objs[i].rect);
		}
	}*/

	tip.setString(tips[currentLevel]);
}

void Game::restartLevel() {
	Object o = lvl.GetObject("player");
	pl.spr.setPosition(o.rect.left, o.rect.top);
	key = false;
}

void Game::newMusic() {
	music.openFromFile(tracks[rand() % tracks.size()]);
	music.setVolume(50);
	music.play();
}

void Game::update()
{
	pl.update();

	/* pretty background animation*/
	bg1.move(0, Constants::BG_SPEED);
	bg2.move(0, Constants::BG_SPEED);

	if (bg1.getPosition().y >= 768) {
		bg1.setPosition(0,-768);
	}

	if (bg2.getPosition().y >= 768) {
		bg2.setPosition(0,-767);
	}

	if (music.getPlayingOffset() == music.getDuration()) {
		music.stop();
		newMusic();
	}

	if (flashing) {
		sf::Color fc = flash.getFillColor();
		fc.a -= 15;
		if (fc.a <= 0) {
			flashing = false;
		} else {
			flash.setFillColor(fc);
		}
	}

	if (hasTp) {
		tpSpr.rotate(5.0f);
	}

	/* x collision check*/
	pl.spr.move(pl.vel.x, 0);
	std::vector<Object> obj = lvl.GetAllObjects();
	for (int i=0;i<obj.size();i++) {
		if (pl.spr.getGlobalBounds().intersects(obj[i].rect)) {
			if (obj[i].name=="solid") {

				if (pl.vel.x > 0 ) { pl.spr.move(-pl.vel.x, 0); pl.vel.x = 0; }
				if (pl.vel.x < 0 ) { pl.spr.move(-pl.vel.x, 0); pl.vel.x = 0; }

			} 
		}
	}

	/* y collision check */
	pl.spr.move(0, pl.vel.y);
	for (int i=0;i<obj.size();i++) {
		if (pl.spr.getGlobalBounds().intersects(obj[i].rect)) {
			if (obj[i].name=="solid") {
				if (pl.vel.y > 0) { pl.spr.setPosition(sf::Vector2f(pl.spr.getPosition().x, obj[i].rect.top - obj[i].rect.height)); pl.vel.y = 0; pl.onGround = true;}
				if (pl.vel.y < 0) { pl.spr.move(0, -pl.vel.y); pl.vel.y = 0;}
			} 

			/* If we touched damage blocks*/
			if (obj[i].name == "damage") {
				deathSnd.play();
				pl.vel.x = 0;
				pl.vel.y = 0;
				restartLevel();	
			}

			/* If we picked up a key*/
			if (obj[i].name == "key") {
				if (!key) {
					key = true;
					pickupSnd.play();
				}
			}

			/* If we touched a teleport */
			if (obj[i].type == "tpStart") {
				sf::Vector2f pos = teleportsEnds.at(obj[i].name);
				pl.spr.setPosition(pos);
				pl.vel.x = 0;
				pl.vel.y = 0;
				tpSnd.play();
			}
		}
	}


	if (pl.spr.getGlobalBounds().contains(endPos)) {
		if (isKey && !key) return;
		currentLevel++;
		okSnd.play();
		flash.setFillColor(sf::Color(255,255,255,255));
		flashing = true;

		loadLevel();
	}

	if (debug) {
		std::stringstream ss;
		ss << "vel: " << pl.vel.x << " : " << pl.vel.y << "\n" << "onGround: " << pl.onGround << "\nkey: " << key;
		debugTxt.setString(ss.str());
	}
					
}

void Game::handleEvent(sf::Event event)
{
	if (event.type == sf::Event::KeyReleased) {
		if (event.key.code == sf::Keyboard::F3) {
			debug = !debug;
		}
	}
}



void Game::draw(sf::RenderWindow &window)
{
	window.clear(sf::Color::Black);
	window.draw(bg1);
	window.draw(bg2);
	lvl.Draw(window);
	pl.draw(window);
	window.draw(tip);
	if (isKey && !key) window.draw(keySpr);
	if (flashing) window.draw(flash);
	if (debug) window.draw(debugTxt);
	for (int i=0;i<teleports.size();i++) {
		tpSpr.setPosition(teleports[i]);
		window.draw(tpSpr);
	}
}
