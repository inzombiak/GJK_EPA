#include "ConvexShape.h"
#include "Math.h"

ConvexShape::ConvexShape(const sf::ConvexShape& newShape)
{
	m_shape = newShape;
	m_shape.setFillColor(UNSELECTED_COLOR);

	int pc = m_shape.getPointCount();

	for (unsigned int i = 0; i < pc; ++i)
	{
		m_vertices.push_back(m_shape.getPoint(i));
	}

	CalcCentroid();
}

void ConvexShape::Draw(sf::RenderWindow& rw)
{
	if (m_inUse)
		rw.draw(m_shape);

	if (m_marked)
		rw.draw(m_label);
}

bool ConvexShape::ContainsPoint(const sf::Vector2f& point)
{
	int cross = 0;

	sf::Vector2f dir = m_currentPosition - point;
	if (dir.x == 0 && dir.y == 0)
		return true;
	
	//Normalize ray direction
	dir /= sfmath::Length(dir);
	sfmath::Ray ray;
	ray.origin = point;
	ray.direction = dir;
	int vertexCount = m_vertices.size();
	for (unsigned int i = 0; i < vertexCount; ++i)
	{
		if (sfmath::RayLineIntersect(ray, m_vertices[i], m_vertices[(i + 1) % vertexCount]))
			cross++;
	}

	return (cross % 2 == 1 ? true : false);
}

void ConvexShape::SetPosition(const sf::Vector2f& newPos)
{
	sf::Vector2f diff = newPos - m_oldMousePos;
	sf::Vector2f oldPos = m_shape.getPosition();

	for (unsigned int i = 0; i < m_vertices.size(); ++i)
	{
		m_vertices[i] += diff;
	}

	m_oldMousePos = newPos;
	m_shape.setPosition(oldPos + diff);
	m_currentPosition += diff;
	m_label.setPosition(m_currentPosition);
}

void ConvexShape::SetFillColor(const sf::Color& newColor)
{
	m_shape.setFillColor(newColor);
}