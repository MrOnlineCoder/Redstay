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
	
}


Player::~Player(void)
{
}

void Player::init() {
	tex.loadFromFile("data/images/player.png");
	spr.setTexture(tex);
	spr.setPosition(10,10);
	spr.setOrigin(sf::Vector2f(spr.getLocalBounds().width/2, spr.getLocalBounds().height/2 ));

	jumpBuff.loadFromFile("data/sfx/jump.ogg");
	jumpSnd.setBuffer(jumpBuff);

	vel.y = Constants::GRAVITY;
	onGround = false;
	hp = Constants::MAXHP;

	hpbar.setFillColor(sf::Color::Green);
	hpbar.setPosition(38,38);
	hpbar.setSize(sf::Vector2f(Constants::HPBAR_WIDTH-2, 30));

	hpbarOutline.setPosition(36,36);
	hpbarOutline.setSize(sf::Vector2f(Constants::HPBAR_WIDTH, 32));
	hpbarOutline.setOutlineThickness(2);
	hpbarOutline.setOutlineColor(sf::Color::White);
	hpbarOutline.setFillColor(sf::Color(0,0,0,0));
}

void Player::update(float timescale, int force)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) { 
		if (onGround) {
			vel.y += Constants::JUMP_POWER*force;
			onGround = false;
			jumpSnd.play();
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		if (right) {
			right = false;
			spr.setScale(-1.f, spr.getScale().y);
		}
		vel.x = -Constants::PLAYER_SPEED;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		if (!right) {
			right = true;
			spr.setScale(1.f, spr.getScale().y);
		}
		vel.x = Constants::PLAYER_SPEED;
	} 

	vel.y += Constants::GRAVITY * timescale * force;


	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		vel.x = 0;
	}

	
}

bool Player::damage(int v) {
	if (hp+v > Constants::MAXHP) return false;
	hp += v;
	int nw = hp * (Constants::HPBAR_WIDTH-2) / Constants::MAXHP;
	hpbar.setSize(sf::Vector2f(nw, 30));
	return (hp == 0);
}


void Player::draw(sf::RenderWindow& window)
{
	window.draw(spr);
	if (hp > 0) { 
		window.draw(hpbar);
		window.draw(hpbarOutline);
	}
}
