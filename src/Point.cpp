#pragma once
#include "Point.h"

//point constructor will build the point object include edges
Point::Point(sf::Vector2f vertexLocation, bool isCenter, std::vector<float> edges, int verId) : m_point(pointSize), m_verLoc(vertexLocation)
{
	m_id = verId;
	m_point.setOrigin(m_point.getRadius(), m_point.getRadius());
	if (isCenter || verIsOn)
	{
		pointColor = sf::Color::Magenta;
		verIsOn = true;
		m_center = true;
	}
	else
	{
		pointColor = sf::Color(128,128,128);
		verIsOn = false;
	}
	m_point.setFillColor(pointColor);
	m_point.setPosition(m_verLoc);

	for (auto i = 0; i < edges.size(); ++i)
	{
		m_edges.push_back(sf::RectangleShape({ edgeWidth, edgeLength }));
		auto& e = m_edges.back();
		e.setFillColor(edgeColor);
		e.setOrigin(e.getSize().x / 2, -m_point.getRadius());
		e.setPosition(m_point.getPosition());
		e.setRotation(edges[i] - 30); 
	}
}

//draw points
void Point::draw(sf::RenderTarget& target) const
{
	target.draw(m_point);
	for (const auto& e : m_edges)
		target.draw(e);

}

//rotate edges by pressed button
void Point::rotate(char dir, sf::Vector2f location, std::vector<Point>& points, Point& current)
{
	if (m_point.getGlobalBounds().contains(location))
	{
		for (auto& e : m_edges)
		{
			if (dir == 'L')
				e.rotate(-rotation);
			else if (dir == 'R')
				e.rotate(rotation);
		}
	}
}

//check if current point conncted to source RECURSIVE FUNCTION
bool Point::is_connect_to_source(std::vector<Point>& points, int current, Point& prev)
{
	points[current].checked = true;
	if (points[current].m_center)
		return true;
	for (int i = 0; i < points[current].m_neighbors.size(); i++)
	{
		if (points[points[current].m_neighbors[i]].checked != true)
		{
			if (points[points[current].m_neighbors[i]].m_center)
				return true;
			if (is_connect_to_source(points, points[current].m_neighbors[i], points[current]))
				return true;
		}
	}
	return false;
}


//reset point neighbours vector
void Point::reset_neighbors(std::vector<Point>& points, Point& current)
{
	current.m_neighbors.clear();
	for (auto& e : this->m_edges)
	{
		for (int i = 0; i < points.size(); i++)
		{
			auto& p = points[i];
			for (int j = 0; j < p.m_edges.size(); j++)
			{
				if (e.getGlobalBounds().intersects(points[i].m_edges[j].getGlobalBounds())
					&& m_point.getPosition() != p.m_point.getPosition())
				{
					current.m_neighbors.push_back(points[i].m_id);
				}
			}
		}
	}
}