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


Game::Game(void)
{

}


Game::~Game(void)
{

}


void Game::init(void)
{
	srand(time(0));
	currentLevel = 10;
	flashing = false;
	debug = false;
	animState = 0;
	force = 1;
	finished = false;
	pl.init();
	pl.tex.loadFromFile("data/images/player.png");
	timescale = 1.0f;

	tips.clear();
	tracks.clear();



	tracks.push_back("data/music/game.ogg");
	tracks.push_back("data/music/game2.ogg");
	tracks.push_back("data/music/game3.ogg");
	tracks.push_back("data/music/game4.ogg");

	tips.push_back("Use A and D to move, W to jump");
	tips.push_back("Try to avoid Red Blocks");
	tips.push_back("Be patient.");
	tips.push_back("Sneaky Peaky!");
	tips.push_back("Sometimes you need a key to continue");
	tips.push_back("Teleports are real!");
	tips.push_back("Seems to be easy.");
	tips.push_back("Oh, crap! Too many teleports!");
	tips.push_back("Use Time Ray to slow down the time!");
	tips.push_back("Stuck in space!");
	tips.push_back("Press E to use a magnet");
	tips.push_back("Swap and stop!");
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

	dmgBuff.loadFromFile("data/sfx/damage.wav");
	dmgSnd.setBuffer(dmgBuff);

	deathBuff.loadFromFile("data/sfx/death.ogg");
	deathSnd.setBuffer(deathBuff);

	okBuff.loadFromFile("data/sfx/complete.ogg");
	okSnd.setBuffer(okBuff);

	pickupBuff.loadFromFile("data/sfx/pickup.wav");
	pickupSnd.setBuffer(pickupBuff);

	tpBuff.loadFromFile("data/sfx/teleport.ogg");
	tpSnd.setBuffer(tpBuff);

	rewBuff.loadFromFile("data/sfx/rewind.ogg");
	rewSnd.setBuffer(rewBuff);
	
	magBuff.loadFromFile("data/sfx/magnet.ogg");
	magSnd.setBuffer(magBuff);

	flash.setFillColor(sf::Color::White);
	flash.setSize(sf::Vector2f(1024,768));
	flash.setPosition(sf::Vector2f(0,0));

	keySpr.setTexture(lvl.tilesetImage);
	keySpr.setTextureRect(sf::IntRect(4 * Constants::TILESIZE, 0, Constants::TILESIZE, Constants::TILESIZE));

	tpSpr.setTexture(lvl.tilesetImage);
	tpSpr.setTextureRect(sf::IntRect(5 * Constants::TILESIZE, 0, Constants::TILESIZE, Constants::TILESIZE));

	magSpr.setTexture(lvl.tilesetImage);
	magSpr.setTextureRect(sf::IntRect(7 * Constants::TILESIZE, 0, Constants::TILESIZE, Constants::TILESIZE));

	tpSpr.setOrigin(sf::Vector2f(tpSpr.getLocalBounds().width/2, tpSpr.getLocalBounds().height/2 ));

	debugTxt.setFont(f);
	debugTxt.setPosition(36,68);
	debugTxt.setFillColor(sf::Color::White);
	debugTxt.setCharacterSize(16);

	deathView.setSize(sf::Vector2f(1024,768));

	magSpr.setPosition(sf::Vector2f(-1000,-1000));

	loadLevel();
}

void Game::loadLevel() {

	teleports.clear();
	teleportsEnds.clear();
	timescale = 1.0f;
	music.setPitch(1.0f);
	magnet = false;
	force = 1;

	std::string path = "data/levels/";
	path = path + itos(currentLevel);
	path = path + ".tmx";
	lvl.LoadFromFile(path);
	lvl.SetDrawingBounds(sf::FloatRect(0,0,1024+32, 768+32));

	Object o = lvl.GetObject("player");
	pl.spr.setPosition(o.rect.left + Constants::TILESIZE/2, o.rect.top + Constants::TILESIZE/2);

	Object endObj = lvl.GetObject("end");
	endPos.x = endObj.rect.left + Constants::TILESIZE/2;
	endPos.y = endObj.rect.top + Constants::TILESIZE/2;

	if (lvl.HasObject("key")) {
		isKey = true;
		key = false;
		Object keyObj = lvl.GetObject("key");
		keySpr.setPosition(keyObj.rect.left, keyObj.rect.top);
	} else {
		isKey = false;
		key = false;
	}

	if (lvl.HasObject("magnet")) {
		Object mobj = lvl.GetObject("magnet");
		magSpr.setPosition(sf::Vector2f(mobj.rect.left, mobj.rect.top));
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
	if (pl.hp == 0) return;
	Object o = lvl.GetObject("player");
	pl.spr.setPosition(o.rect.left + Constants::TILESIZE/2, o.rect.top + Constants::TILESIZE/2);
	key = false;
	force = 1;
}

void Game::newMusic() {
	music.openFromFile(tracks[rand() % tracks.size()]);
	music.setVolume(50);
	music.play();
}

void Game::update()
{
	if (animState == 0) pl.update(timescale, force);

	/* pretty background animation*/
	bg1.move(0, Constants::BG_SPEED*timescale*force);
	bg2.move(0, Constants::BG_SPEED*timescale*force);

	if (force == 1) {
		if (bg1.getPosition().y >= 768) {
			bg1.setPosition(0,-768);
		}

		if (bg2.getPosition().y >= 768) {
			bg2.setPosition(0,-767);
		}
	} else {
		if (bg1.getPosition().y < 0) {
			bg1.setPosition(0,768);
		}

		if (bg2.getPosition().y < 0) {
			bg2.setPosition(0,768);
		}
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
		tpSpr.rotate(5.0f*timescale);
	}

	/* x collision check*/
	std::vector<Object> obj = lvl.GetAllObjects();
	sf::FloatRect hr = pl.spr.getGlobalBounds();
	sf::FloatRect vr = pl.spr.getGlobalBounds();
	hr.left += pl.vel.x;
	for (int i=0;i<obj.size();i++) {
		if (hr.intersects(obj[i].rect)) {
			if (obj[i].name=="solid") {
				pl.vel.x = 0;
				break;
			} 
		}
	}

	pl.spr.move(pl.vel.x, 0);

	/* y collision check */
	vr.top += pl.vel.y;
	if (pl.hp > 0) for (int i=0;i<obj.size();i++) {

		if (vr.intersects(obj[i].rect)) {
			if (obj[i].name=="solid") {
				if (pl.vel.y*force > 0) { pl.vel.y = 0; pl.onGround = true;}
				if (pl.vel.y*force < 0) { pl.vel.y = 0;}
				continue;
			} 

			/* If we touched damage blocks*/
			if (obj[i].name == "damage") {
				dmgSnd.play();
				pl.vel.x = 0;
				pl.vel.y = 0;

				/* if we are dead (hp == 0) */
				if (pl.damage(-1)) {
					timescale = 0.0f;
					music.stop();

					/* play animation (?) */
					deadClock.restart();
					animState = 1;
					deathSnd.play();
				} else {
					restartLevel();
				}
				continue;
			}

			/* If we picked up a key*/
			if (obj[i].name == "key") {
				if (!key) {
					key = true;
					pickupSnd.play();
				}
				continue;
			}

			/* If we touched a teleport */
			if (obj[i].type == "tpStart") {
				sf::Vector2f pos = teleportsEnds.at(obj[i].name);
				pl.spr.setPosition(pos);
				pl.vel.x = 0;
				pl.vel.y = 0;
				tpSnd.play();
				continue;
			}

			if (obj[i].name == "rewind") {
				if (timescale == 1.0f) {
					rewSnd.play();
					timescale = 0.4f;
					music.setPitch(0.5f);
				}
			}

			if (obj[i].name == "magnet") {
				if (magnet) continue;
				magnet = true;
				pickupSnd.play();
			}
		}
	}
	pl.spr.move(0,pl.vel.y);


	if (pl.spr.getGlobalBounds().contains(endPos)) {
		if (isKey && !key) return;
		currentLevel++;
		okSnd.play();
		flash.setFillColor(sf::Color(255,255,255,255));
		flashing = true;

		loadLevel();
		pl.damage(1);
	}

	if (debug) {
		std::stringstream ss;
		ss << "pos: " << pl.spr.getPosition().x << " : " << pl.spr.getPosition().y << "\n" << "onGround: " << pl.onGround << "\nkey: " << key << "\ntimescale: "<< timescale << "\nforce: " << force;
		debugTxt.setString(ss.str());
	}


	if (animState == 1) {
		if (deathView.getSize().x > Constants::DEATHVIEW_WIDTH) {
			sf::Vector2f s = deathView.getSize();
			s.x -= 10;
			s.y -= 7.6f;
			deathView.setCenter(pl.spr.getPosition());
			deathView.setSize(s);
		}

		if (deadClock.getElapsedTime().asMilliseconds() >= 1750 ) {
			animState = 2;
			deadClock.restart();
			pl.tex.loadFromFile("data/images/dead.png");
			flash.setFillColor(sf::Color::Red);
			flashing = true;
			okSnd.play();
			deathView.setCenter(pl.spr.getPosition());
		}
	}

	if (animState == 2) {
		if (deadClock.getElapsedTime().asSeconds() > 2) {
			finished = true;
		}
	}
}

void Game::handleEvent(sf::Event event)
{
	if (event.type == sf::Event::KeyReleased) {
		if (event.key.code == sf::Keyboard::F3) {
			debug = !debug;
		}

		if (event.key.code == sf::Keyboard::E && magnet) {
			force = -force;
			pl.vel.y = 0;
			pl.spr.setScale(pl.spr.getScale().x, -pl.spr.getScale().y);
			magSnd.play();
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
	if (animState == 0) window.draw(tip);
	if (animState > 0) window.setView(deathView);
	if (isKey && !key) window.draw(keySpr);
	if (flashing) window.draw(flash);
	if (debug) window.draw(debugTxt);
	for (int i=0;i<teleports.size();i++) {
		tpSpr.setPosition(teleports[i]);
		window.draw(tpSpr);
	}
	if (!magnet) window.draw(magSpr);
}
