#include "GJKCalculator.h"


GJKCalculator::GJKCalculator()
{
}


bool GJKCalculator::CalculateGJKFull(const std::vector<sf::Vector2f>& shapeA, const std::vector<sf::Vector2f>& shapeB, std::vector<sfmath::SupportPoint>& simplexOut)
{
	sf::Vector2f dir;
	sfmath::SupportPoint nextSupportPoint;

	std::vector<sfmath::SupportPoint> simplex;

	//Init simplex
	dir = shapeA[0];
	nextSupportPoint = sfmath::GetSupportPoint(shapeA, shapeB, dir);
	simplex.push_back(nextSupportPoint);
	

	dir =  dir * -1.f;

	bool intersection = false;

	while (true)
	{
		nextSupportPoint = sfmath::GetSupportPoint(shapeA, shapeB, dir);
		//No intersection
		if (sfmath::Dot(nextSupportPoint.position, dir) <= 0)
		{
			intersection = false;
			break;
		}
		simplex.push_back(nextSupportPoint);
		if (DoSimplex(simplex, dir))
		{
			intersection = true;
			break;
		}
	}

	simplexOut = simplex;
	return intersection;
}

bool GJKCalculator::DoSimplexEdge(int aIndex, int bIndex, std::vector<sfmath::SupportPoint>& simplex, sf::Vector2f& dir)
{
	sf::Vector3f newDir;
	sfmath::SupportPoint a = simplex[aIndex];
	sfmath::SupportPoint b = simplex[bIndex];
	sf::Vector2f ab = b.position - a.position;
	std::vector<sfmath::SupportPoint> newSimplex;
	if (sfmath::SameDirection(ab, -a.position))
	{
		newDir = sfmath::Cross3D(sfmath::Cross3D(sf::Vector3f(ab.x, ab.y, 0), -sf::Vector3f(a.position.x, a.position.y, 0)), sf::Vector3f(ab.x, ab.y, 0));
		dir.x = newDir.x;
		dir.y = newDir.y;
		
		newSimplex.push_back(b);
		newSimplex.push_back(a);
		simplex = newSimplex;

		return true;
	}
	else
	{
		dir = -a.position;
		newSimplex.push_back(a);
		simplex = newSimplex;

		return false;
	}
}

bool DoSimplexTriangle(std::vector<sfmath::SupportPoint>& simplex, sf::Vector2f& dir)
{
	return false;
}

bool GJKCalculator::DoSimplex(std::vector<sfmath::SupportPoint>& simplex, sf::Vector2f& dir)
{
	sf::Vector3f newDir;
	if (simplex.size() == 1)
	{
		//Vertex
		dir = -simplex[0].position;
	}
	else if (simplex.size() == 2)
	{
		//Edge
		DoSimplexEdge(1, 0, simplex, dir);
		
	}
	else
	{
		//Triangle
		sf::Vector3f a = sf::Vector3f(simplex[2].position.x, simplex[2].position.y, 0);
		sf::Vector3f b = sf::Vector3f(simplex[1].position.x, simplex[1].position.y, 0);
		sf::Vector3f c = sf::Vector3f(simplex[0].position.x, simplex[0].position.y, 0);

		sf::Vector3f ac = c - a;
		sf::Vector3f ab = b - a;

		sf::Vector3f abc = sfmath::Cross3D(ab, ac);

		//If AC is closest to origin
		if (sfmath::SameDirection(sfmath::Cross3D(abc, ac), -a))
		{
			if (sfmath::SameDirection(ac, -a))
			{
				newDir = sfmath::Cross3D(sfmath::Cross3D(ac, -a), ac);
				dir.x = newDir.x;
				dir.y = newDir.y;

				std::swap(simplex[1], simplex[2]);
				simplex.pop_back();
			}
			else
			{

				DoSimplexEdge(2, 1, simplex, dir);
				/*
				BETTER BUT MORE WORDY METHOD
				if (sfmath::SameDirection(ab, -a))
				{
					newDir = sfmath::Cross3D(sfmath::Cross3D(ab, -a), ab);
					dir.x = newDir.x;
					dir.y = newDir.y;

					std::swap(simplex[1], simplex[0]);
					std::swap(simplex[2], simplex[1]);
					simplex.pop_back();
				}
				else
				{
					dir = -simplex[2].position;
					std::swap(simplex[2], simplex[0]);
					simplex.pop_back();
					simplex.pop_back();
				}
				*/
			}
		}
		else
		{
			//If AB is closest to origin
			if (sfmath::SameDirection(sfmath::Cross3D(ab, abc), -a))
			{
				DoSimplexEdge(2, 1, simplex, dir);
				/*
				if (sfmath::SameDirection(ab, -a))
				{
					newDir = sfmath::Cross3D(sfmath::Cross3D(ab, -a), ab);
					dir.x = newDir.x;
					dir.y = newDir.y;

					std::swap(simplex[1], simplex[0]);
					std::swap(simplex[2], simplex[1]);
					simplex.pop_back();
				}
				else
				{
					dir = -simplex[2].position;
					std::swap(simplex[2], simplex[0]);
					simplex.pop_back();
					simplex.pop_back();
				}*/
			}
			else
				return true;
		}
	}
	return false;
}
