#pragma once
#include "Point.h"
#include <time.h>

class Controller
{
public:
	Controller();
	void run();
	void readLevel(int levelNum);
	void buildLevel(std::vector<std::string>& level);
	void setGameClock(int elapsedTime, sf::RenderWindow& window);
	void draw(sf::RenderWindow& window);

private:
	int m_elapsedTime;
	int m_level;
	int m_minutes;
	sf::Music m_backMusic;
	
	sf::Clock m_gameClock;
	sf::Font t_font;
	sf::Text m_clockNum;
	sf::Sprite m_background;
	sf::Texture m_back;
	std::vector<std::string> m_stage;
	std::vector<Point> m_points;
};