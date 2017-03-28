#ifndef GJK_CALCULATOR_H
#define GJK_CALCULATOR_H

#include <SFML\Graphics.hpp>
#include <vector>

class GJKCalculator
{
public:
	GJKCalculator();
	
	void CalculateGJKFull(const std::vector<sf::Vector2f> shapeA, const std::vector<sf::Vector2f> shapeB);
	void Draw(sf::RenderWindow& rw);

private:
	sf::Vector2f GetSupportPoint(const std::vector<sf::Vector2f> vertices, const sf::Vector2f& dir);


};

#endif