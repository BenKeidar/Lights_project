#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>

//constants
constexpr auto pointSize = 8;
constexpr auto edgeWidth = (pointSize -2) / 2;
constexpr auto edgeLength = 9 + pointSize * 3 ;
constexpr auto& edgeColor = sf::Color::White;
constexpr auto maxEdges = 4;
constexpr auto rotation = 60.f;


class Point
{
public:
	Point(sf::Vector2f vertexLocation, bool isCenter, std::vector<float> edges, int verId);

	void draw(sf::RenderTarget& target)  const;
    void rotate(char dir, sf::Vector2f location, std::vector<Point>& points, Point& current);
	void reset_neighbors(std::vector<Point>& points, Point& current);
	void set_color(sf::Color color) {m_point.setFillColor(pointColor = color);}
	void set_checked_off() { checked = false;}
	bool is_connect_to_source(std::vector<Point>& points, int current, Point& prev);
	int get_id() { return m_id; }

	~Point() = default;
private:
	int m_id;
	bool checked = false;
	bool verIsOn = false;
	bool m_center = false;
	sf::Color pointColor;
	sf::CircleShape m_point;
	std::vector<int> m_neighbors;
	std::vector<sf::RectangleShape> m_edges;
	sf::Vector2f m_verLoc;

};