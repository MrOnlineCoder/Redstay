/*
  Copyright (c) 2017 Nikita Kogut (vk.com/mronlinecoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
  All rights reserved.
  
  Only original author is allowed to edit and share this file.
  Unauthorized copying or editing this file, via any medium is strictly prohibited
  Proprietary and confidential

*/

#include <SFML/Graphics.hpp>
#include "level.h"
#include "Player.h"
#include "Boss.h"
#include <SFML/Audio.hpp>

#pragma once
class Game
{
public:
	void init();
	void draw(sf::RenderWindow &window);
	void update();
	void handleEvent(sf::Event event);
	void newMusic();
	void loadLevel();
	void restartLevel();

	Level lvl;
	Player pl;
	sf::Music music;
	std::vector<std::string> tracks;
	std::vector<std::string> tips;
	std::vector<sf::Rect<float>> blacks;

	sf::Sprite bg1, bg2;
	sf::Texture bgTex;

	sf::Text tip;
	sf::Font f;

	sf::Vector2f endPos;

	sf::Sound deathSnd;
	sf::SoundBuffer deathBuff;

	sf::Sound dmgSnd;
	sf::SoundBuffer dmgBuff;

	sf::Sound okSnd;
	sf::SoundBuffer okBuff;

	sf::Sound pickupSnd;
	sf::SoundBuffer pickupBuff;

	sf::Sound tpSnd;
	sf::SoundBuffer tpBuff;

	sf::Sound rewSnd;
	sf::SoundBuffer rewBuff;

	sf::Sound magSnd;
	sf::SoundBuffer magBuff;

	sf::Sound kickSnd;
	sf::SoundBuffer kickBuff;


	sf::RectangleShape flash;

	sf::Sprite keySpr;

	sf::Sprite tpSpr;

	sf::Sprite magSpr;

	sf::Vector2f tpPos;

	sf::Text debugTxt;

	sf::Clock deadClock;

	sf::View deathView;

	std::vector<sf::Vector2f> teleports;
	std::map<std::string,sf::Vector2f> teleportsEnds;

	Boss boss;


	int currentLevel;

	bool flashing;

	bool key;
	bool isKey;

	bool hasTp;

	bool debug;

	float timescale;
	int animState;

	bool finished;

	bool magnet;
	bool hasMagnet;
	int force;

	bool bossLevel;
};

