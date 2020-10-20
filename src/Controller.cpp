#pragma once
#include "Controller.h"
#include "windows.h"
#include <fstream>

//controller constructor upload game files
Controller::Controller() : m_level(1), m_elapsedTime(0), m_minutes(0)
{
	t_font.loadFromFile("FONT.ttf"); 
	m_backMusic.openFromFile("backmusic.ogg");
	m_backMusic.setVolume(0.5);
	m_back.loadFromFile("background.png");
	m_background.setTexture(m_back);
}

//main function manages whole game
void Controller::run() 
{
	int lights_connected = 0;
	bool skip = false;
	bool mute = false;
	float temp = 100.f;
	sf::Vector2f centerVerLoc(250.f, 250.f);
	m_backMusic.setLoop(true); 
	m_backMusic.play();
	
	try 
	{
		for (int level_num = 1; level_num <= 3; level_num++)
		{
			auto window = sf::RenderWindow(sf::VideoMode(500+(level_num-1)*160,500+(level_num-1)*250), "LIGHTS");
			readLevel(level_num);
			while (window.isOpen())
			{

				draw(window);

			
				auto event = sf::Event{};
				while (window.pollEvent(event))
				{
					switch (event.type)
					{
					case sf::Event::Closed:
						window.close();
						return;
					case sf::Event::MouseButtonPressed:
						const auto location = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
						switch (event.mouseButton.button)
						{
						case sf::Mouse::Button::Left:
							for (int i = 0; i < m_points.size(); i++)
							{
								m_points[i].rotate('L', location, m_points, m_points[i]);
							}
							break;
						case sf::Mouse::Button::Right:
							for (int i = 0; i < m_points.size(); i++)
							{
								m_points[i].rotate('R', location, m_points, m_points[i]);
							}
							break;
						}
						lights_connected = 0;
						for (int i = 0; i < m_points.size(); i++) //will reset the neighbors
							m_points[i].reset_neighbors(m_points, m_points[i]);
						for (auto& p : m_points) //check if current vertex is connected to source
						{
							for (int j = 0; j < m_points.size(); j++)
								m_points[j].set_checked_off();
							if (p.is_connect_to_source(m_points, p.get_id(), p))
							{
								lights_connected++;
								p.set_color(sf::Color::Magenta);
							}
							else
								p.set_color(sf::Color(128,128,128));
						}
					}
					if (event.type == sf::Event::KeyPressed) //check for key press
					{
						if (event.key.code == sf::Keyboard::S) //will skip a level.
						{
							skip = true; //update the skip "button" to null.
						}
						if (event.key.code == sf::Keyboard::M) //will mute the sound.
						{
							if (!mute)
							{
								m_backMusic.setVolume(0);
								mute = true;
							}
							else
							{
								m_backMusic.setVolume(0.5);
								mute = false;
							}
						}
					}
				}
				if (lights_connected == m_points.size() || skip) //check if game is done
				{
					skip = false;
					draw(window);
					Sleep(4000);
					m_gameClock.restart();
					window.close();
					break;
				}
			}
		}
	}
	catch (const std::exception & e) //handle excpetion
	{
		std::cout << e.what() << std::endl; 
	}
}

//read the current level from a file
void Controller::readLevel(int levelNum)
{
	m_stage.clear();
	m_points.clear();

	m_level = levelNum; //save level number
	std::ifstream file; 
	char num = (char)(m_level)+'0'; // cast from int to asci
	std::string end = ".txt";
	std::string file_name = "";
	file_name += num;
	file_name += end;
	file.open(file_name); 
	if (!file)
		throw std::exception("File did not open!"); 

	std::string line;
	while (file)
	{
		getline(file, line);
		m_stage.push_back(line);
	}
	buildLevel(m_stage); 
}


//build current level from readed file
void Controller::buildLevel(std::vector<std::string>& level)
{
	level.pop_back();
	int counter = 3, index = 0;
	float maxRowLen = 3.f + (float(m_level) * 2.f), startLen = 3.f, rowNum = 0.f;
	bool isCenter = false;
	std::vector<float> edges;
	std::string edge1, edge2;

	for (int i = 0; i < level.size(); i++)
	{
		float x = 100.f + (maxRowLen - startLen) * 37.5 + index * 75.f;
		float y = 120.f + rowNum * 65.f;
		if (i == level.size() / 2)
			isCenter = true;

		int redCounter = 0;
		sf::Vector2f loc(x, y);
		int begin = 0;
		edges.clear();
		index++;
		while (true)
		{
			edge2 = level[i].substr(begin, level[i].size());
			int space = edge2.find(" ") + begin;
			edge1 = level[i].substr(begin, space - begin);
			redCounter += edge1.size() + 1;
			int tempEdge = std::stoi(edge1);
			edges.push_back(tempEdge);
			begin = space + 1;
			if (redCounter >= level[i].size())
				break;
		}

		
		Point p(loc, isCenter, edges, i);
		m_points.push_back(p);
		isCenter = false;

		if (index == startLen)
		{
			if (counter < maxRowLen)
				startLen++;
			else
				startLen--;

			counter++;
			rowNum++;
			index = 0;
		}
	}
}

//set game clock
void Controller::setGameClock(int elapsedTime, sf::RenderWindow& window)
{
	elapsedTime = m_gameClock.getElapsedTime().asSeconds();
	m_minutes = elapsedTime / 60;

	if ((elapsedTime % 60) < 10 && (m_minutes) < 10)
		m_clockNum.setString("0" + std::to_string(m_minutes) + ":0" + std::to_string(elapsedTime % 60));
	else if ((elapsedTime % 60) >= 10 && (m_minutes) >= 10)
		m_clockNum.setString(std::to_string(m_minutes) + ":" + std::to_string(elapsedTime % 60));
	if ((elapsedTime % 60) >= 10 && (m_minutes) < 10)
		m_clockNum.setString("0" + std::to_string(m_minutes) + ":" + std::to_string(elapsedTime % 60));
	else
		m_clockNum.setString("0" + std::to_string(m_minutes) + ":0" + std::to_string(elapsedTime % 60));

	m_clockNum.setFont(t_font);
	m_clockNum.setStyle(sf::Text::Regular);
	m_clockNum.setCharacterSize(16);
	m_clockNum.setPosition(10, 10);
	m_clockNum.setFillColor(sf::Color::Cyan);

	window.draw(m_clockNum);
}

void Controller::draw(sf::RenderWindow& window)
{
	window.clear();
	window.draw(m_background);
	for (auto p : m_points)
		p.draw(window);

	setGameClock(m_elapsedTime, window);
	window.display();
}


