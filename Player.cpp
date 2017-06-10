/*
  Copyright (c) 2017 Nikita Kogut (vk.com/mronlinecoder | github.com/MrOnlineCoder | mronlinecoder@gmail.com)
  All rights reserved.
  
  Only original author is allowed to edit and share this file.
  Unauthorized copying or editing this file, via any medium is strictly prohibited
  Proprietary and confidential

*/

#include "Player.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Constants.h"


Player::Player(void)
{
	tex.loadFromFile("data/images/player.png");
	spr.setTexture(tex);
	spr.setPosition(10,10);
	spr.setOrigin(sf::Vector2f(spr.getLocalBounds().width/2, 0 ));

	jumpBuff.loadFromFile("data/sfx/jump.ogg");
	jumpSnd.setBuffer(jumpBuff);

	vel.y = Constants::GRAVITY;
	onGround = false;
}


Player::~Player(void)
{
}


void Player::update(void)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { 
		if (onGround) {
			vel.y += Constants::JUMP_POWER;
			onGround = false;
			jumpSnd.play();
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		if (right) {
			right = false;
			spr.setScale(-1.f, 1.f);
		}
		vel.x = -Constants::PLAYER_SPEED;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		if (!right) {
			right = true;
			spr.setScale(1.f, 1.f);
		}
		vel.x = Constants::PLAYER_SPEED;
	} 

	vel.y += Constants::GRAVITY;


	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		vel.x = 0;
	}

	
}


void Player::draw(sf::RenderWindow& window)
{
	window.draw(spr);
}
