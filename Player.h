/*
  Copyright (c) 2017 Nikita Kogut (vk.com/mronlinecoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
  All rights reserved.
  
  Only original author is allowed to edit and share this file.
  Unauthorized copying or editing this file, via any medium is strictly prohibited
  Proprietary and confidential

*/

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#pragma once
class Player
{
public:
	Player(void);
	~Player(void);
	void update(float timescale, int force);
	void draw(sf::RenderWindow& window);
	void init();

	sf::Sprite spr;
	sf::Texture tex;
	sf::Vector2f vel;
	sf::RectangleShape hpbar;
	sf::RectangleShape hpbarOutline;
	bool right;
	bool onGround;
	int hp;
	bool damage(int v);

	
	sf::Sound jumpSnd;
	sf::SoundBuffer jumpBuff;
};

