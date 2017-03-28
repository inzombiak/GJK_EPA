#ifndef I_SHAPE_H
#define I_SHAPE_H

#include "SFML\Graphics.hpp"

enum ShapeType
{
	Convex,
	Concave,
};

class IShape
{
public:
	IShape();
	virtual ~IShape() {};
	virtual void Draw(sf::RenderWindow& rw) = 0;
	virtual bool ContainsPoint(const sf::Vector2f& point) = 0;

	bool IsInUse() const;
	void SetInUse(bool val);
	std::vector<sf::Vector2f> GetVerticies();
	void Selected(bool val, sf::Vector2f mousePos);
	void Marked(bool val);
	void SetText(std::string text);
	void SetFont(const sf::Font& font);


	virtual void SetPosition(const sf::Vector2f& newPos) = 0;
	virtual void SetFillColor(const sf::Color& newColor) = 0;
	virtual ShapeType GetType() const;

protected:
	void CalcCentroid();

	ShapeType m_shapeType;
	bool m_inUse = true;
	bool m_marked;
	sf::Vector2f m_oldMousePos;
	std::vector<sf::Vector2f> m_vertices;
	sf::Vector2f m_currentPosition;
	sf::Text m_label; //Easier than just having two labels and maintaining position
	static const sf::Color	UNSELECTED_COLOR;
	static const sf::Color	SELECTED_COLOR;
};

#endif

