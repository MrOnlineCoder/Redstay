/*
  Copyright (c) 2017 Nikita Kogut (vk.com/mronlinecoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
  All rights reserved.
  
  Only original author is allowed to edit and share this file.
  Unauthorized copying or editing this file, via any medium is strictly prohibited
  Proprietary and confidential

*/

#pragma once

#include <SFML/Graphics.hpp>

namespace Constants {
	const int GAME_WIDTH = 1024;
	const int GAME_HEIGHY = 768;

	const float PLAYER_SPEED = 5.0f;
	const float JUMP_POWER = -14.0f;
	const float GRAVITY = 0.6f;

	const float BG_SPEED = 0.6f;

	const int TILESIZE = 32;
	const int WIDTH = 32;
	const int HEIGHT = 32;

	const sf::Color RED(213,14,14,255);
	const sf::Color BLUE(0,89,190,255);

	const int HPBAR_WIDTH = 128;
	const int MAXHP = 5;

	const int DEATHVIEW_WIDTH = 256;
	const int DEATHVIEW_HEIGHT = 192;
};