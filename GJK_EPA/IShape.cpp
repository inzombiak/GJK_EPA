#include "IShape.h"
#include "Math.h"

IShape::IShape()
{
	m_label.setCharacterSize(18);
	m_label.setFillColor(sf::Color::Black);
}
bool IShape::IsInUse() const
{
	return m_inUse;
}
void IShape::SetInUse(bool val)
{
	m_inUse = val;
}

ShapeType IShape::GetType() const
{
	return m_shapeType;
}

std::vector<sf::Vector2f> IShape::GetVerticies()
{
	return m_vertices;
}
void IShape::Selected(bool val, sf::Vector2f mousePos)
{
	if (val)
		m_oldMousePos = mousePos;
	if (m_marked)
		return;
		

	if (val)
	{
		SetFillColor(SELECTED_COLOR);
	}
	else
		SetFillColor(UNSELECTED_COLOR);

}

void IShape::Marked(bool val)
{
	m_marked = val;
}

void IShape::SetText(std::string text)
{
	m_label.setString(text);
	m_label.setPosition(m_currentPosition);
}	

void IShape::SetFont(const sf::Font& font)
{
	m_label.setFont(font);
}

void IShape::CalcCentroid()
{
	float x = 0, y = 0;
	int pointCount = 0;
	for (unsigned int i = 0; i < m_vertices.size(); ++i)
	{
		x += m_vertices[i].x;
		y += m_vertices[i].y;

		pointCount++;
	}

	m_currentPosition.x = x / pointCount;
	m_currentPosition.y = y / pointCount;
}

const sf::Color IShape::UNSELECTED_COLOR = sf::Color(0.f, 0.f, 200.f, 25.f);
const sf::Color IShape::SELECTED_COLOR = sf::Color(0.f, 0.f, 200.f, 125.f);