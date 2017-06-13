#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#pragma once
class Boss
{
public:
	Boss();
	int update();
	void draw(sf::RenderWindow& window);
	void init(int x, int y);
	void damage();

	int hp;
	int state;

	sf::Texture tex;
	sf::Sprite spr;

	sf::Clock clock;

	sf::RectangleShape proj;

	sf::SoundBuffer shootBuff;
	sf::Sound shootSnd;

	sf::SoundBuffer endBuff;
	sf::Sound endSnd;

	bool launchPos;
	bool onGround;
	sf::Vector2f vel;
	std::vector<sf::RectangleShape> spikes; //spikes?

	sf::RectangleShape hpbar;
	sf::RectangleShape hpbarOutline;
};

