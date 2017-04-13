#ifndef GJK_CALCULATOR_H
#define GJK_CALCULATOR_H

#include <SFML\Graphics.hpp>
#include <vector>
#include "Math.h"

class GJKCalculator
{
public:
	GJKCalculator();
	
	bool CalculateGJKFull(const std::vector<sf::Vector2f>& shapeA, const std::vector<sf::Vector2f>& shapeB, std::vector<sfmath::SupportPoint>& simplex);
	void Draw(sf::RenderWindow& rw);

private:
	bool DoSimplex(std::vector<sfmath::SupportPoint>& simplex, sf::Vector2f& dir);

};

#endif