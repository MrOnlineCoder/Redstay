#include "Boss.h"
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include <iostream>
#include <time.h>

Boss::Boss() {
	srand(time(0));
	hp = Constants::BOSS_HP;
	state = 0;
	launchPos = true;
	onGround = true;
	tex.loadFromFile("data/images/dead.png");
	spr.setTexture(tex);
	spr.setScale(-1.f, 1.f);
	spr.setOrigin(sf::Vector2f(spr.getLocalBounds().width/2, spr.getLocalBounds().height/2 ));

	vel.x = 0;
	vel.y = 0;

	shootBuff.loadFromFile("data/sfx/boss1.ogg");
	shootSnd.setBuffer(shootBuff);

	endBuff.loadFromFile("data/sfx/end.ogg");
	endSnd.setBuffer(endBuff);

	proj.setFillColor(Constants::RED);
	proj.setSize(sf::Vector2f(1024,512));
	proj.setPosition(-2000,-2000);

	hpbar.setFillColor(Constants::RED);
	hpbar.setSize(sf::Vector2f(Constants::HPBAR_WIDTH-2, 30));
	hpbar.setPosition(Constants::GAME_WIDTH-hpbar.getGlobalBounds().width-38,38);

	hpbarOutline.setSize(sf::Vector2f(Constants::HPBAR_WIDTH, 32));
	hpbarOutline.setOutlineThickness(2);
	hpbarOutline.setPosition(Constants::GAME_WIDTH-hpbar.getGlobalBounds().width-40,36);
	hpbarOutline.setOutlineColor(sf::Color::White);
	hpbarOutline.setFillColor(sf::Color(0,0,0,0));
}


int Boss::update() {

	if (state == 0 && clock.getElapsedTime().asMilliseconds() > 4000) {
		state = 1;

		int ny = (int) launchPos * 512;
		proj.setPosition(1024, ny);
		launchPos = !launchPos;
		shootSnd.play();

		onGround = false;
		vel.y = Constants::JUMP_POWER*2;

		clock.restart();
	}

	if (state == 1 ){
		proj.move(-(Constants::PLAYER_SPEED*2),0);

		if (clock.getElapsedTime().asMilliseconds() > 3000 ) {
			state = 2;
			shootSnd.play();

			onGround = false;
			vel.y = Constants::JUMP_POWER*2;
		}
	}

	if (state == 2 && clock.getElapsedTime().asMilliseconds() > 750) {
		for (int i =0;i<20;i++) {
			sf::RectangleShape r;
			r.setPosition(rand() % Constants::GAME_WIDTH, -512);
			r.setFillColor(Constants::RED);
			r.setSize(sf::Vector2f(32, 512));
			spikes.push_back(r);
		}
		state = 3;
	}

	if (state == 3) {
		for (int i =0;i<spikes.size();i++) {
			spikes[i].move(0, Constants::GRAVITY*8);
		}

		if (clock.getElapsedTime().asMilliseconds() > 7500) {
			state = 4;
			vel.x = -Constants::PLAYER_SPEED;
		}
	}

	if (state == 4) {
		if (spr.getPosition().x < 64) {
			vel.x = -vel.x;
			spr.setScale(1.f,1.f);
		}

		if (spr.getPosition().x > 992) {
			vel.x = -vel.x;
			spr.setScale(-1.f,1.f);
		}
	}

	if (state == -1) {
		spr.rotate(6);
		if (spr.getPosition().y > 768) {
			endSnd.play();
			spr.setColor(sf::Color(255,255,255,0));
		}

		if (clock.getElapsedTime().asMilliseconds() > 3000) {
			return 100;
		}
	}

	if (!onGround && state != -1) {
		vel.y += Constants::GRAVITY;
		if (spr.getPosition().y > 704) { //height - 2 tiles
			spr.setPosition(spr.getPosition().x, 704);
			onGround = true;
			vel.y = 0;
		}
	}
	spr.move(vel.x, vel.y);


	return 0;
}

void Boss::damage() {
	hp--;
	int nw = hp * (Constants::HPBAR_WIDTH-2) / Constants::BOSS_HP;
	hpbar.setSize(sf::Vector2f(nw, 30));


	//WE KILLED THE BOSS!
	if (hp == 0) {
		state = -1;
		vel.y = Constants::GRAVITY;
		vel.x = 0;
		clock.restart();
	}
}

void Boss::draw(sf::RenderWindow& window) {
	window.draw(spr);
	window.draw(hpbarOutline);
	window.draw(hpbar);
	if (state == 1) window.draw(proj);
	if (state == 3) for (int i=0;i<spikes.size();i++) {
		window.draw(spikes[i]);
	}
}

void Boss::init(int x, int y) {
	spr.setPosition(x,y);
	clock.restart();
	vel.x = 0;
	vel.y = 0;
	spr.setScale(-1.f,1.f);
}
