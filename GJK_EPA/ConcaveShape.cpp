#include "ConcaveShape.h"

#include "Math.h"

ConcaveShape::ConcaveShape(const std::vector<sf::ConvexShape>& shapes, const std::vector<sf::Vector2f>& vertices, const std::vector<int>& reflexVertices)
{
	m_shapes = shapes;
	SetFillColor(UNSELECTED_COLOR);
	m_vertices = vertices;
	CalcCentroid();
	m_shapeType = Concave;
	m_reflexVertices = reflexVertices;
}

ConcaveShape::~ConcaveShape()
{
	m_shapes.clear();
}

void ConcaveShape::SetPosition(const sf::Vector2f& newPos)
{
	sf::Vector2f diff = newPos - m_oldMousePos;
	sf::Vector2f oldPos;

	for (unsigned int i = 0; i < m_shapes.size(); ++i)
	{
		oldPos = m_shapes[i].getPosition();
		m_shapes[i].setPosition(oldPos + diff);
	}

	for (unsigned int i = 0; i < m_vertices.size(); ++i)
	{
		m_vertices[i] += diff;
	}

	m_oldMousePos = newPos;
	m_currentPosition += diff;
	m_label.setPosition(m_currentPosition);
}

void ConcaveShape::SetFillColor(const sf::Color& newColor)
{
	for (unsigned int i = 0; i < m_shapes.size(); ++i)
		m_shapes[i].setFillColor(newColor);
}

void ConcaveShape::Draw(sf::RenderWindow& rw)
{
	if (!m_inUse)
		return;

	for (unsigned int i = 0; i < m_shapes.size(); ++i)
		rw.draw(m_shapes[i]);

	//if (m_marked)
	//	rw.draw(m_label);
}

bool ConcaveShape::ContainsPoint(const sf::Vector2f& point)
{
	bool result = false;
	sf::Vector2f a, b, c;

	//All shapes are triangles
	for (unsigned int i = 0; i < m_shapes.size(); ++i)
	{
		a = m_shapes[i].getPoint(0)+m_shapes[i].getPosition();
		b = m_shapes[i].getPoint(1)+m_shapes[i].getPosition();
		c = m_shapes[i].getPoint(2)+m_shapes[i].getPosition();

		if (sfmath::PointInTriangle(a, b, c, point))
		{
			result = true;
			break;
		}

	}
	return result;
}

std::vector<int> ConcaveShape::GetReflexIndices() const
{
	return m_reflexVertices;
}