#include "GJKCalculator.h"
#include "Math.h"

GJKCalculator::GJKCalculator()
{
}


void GJKCalculator::CalculateGJKFull(const std::vector<sf::Vector2f>& shapeA, const std::vector<sf::Vector2f>& shapeB)
{
	sf::Vector2f dir;
	sf::Vector2f nextSupportPoint;

	sf::Vector2f simplex[3];
	int simplexIndex = 0;

	//Init simplex
	dir = shapeA[0];
	nextSupportPoint = GetSupportPoint(shapeA, dir) - GetSupportPoint(shapeB, -dir);
	simplex[simplexIndex] = nextSupportPoint;
	simplexIndex++;

	dir =  dir * -1.f;

	while (true)
	{
		nextSupportPoint = GetSupportPoint(shapeA, dir) - GetSupportPoint(shapeB, -dir);
		//No intersection
		if (sfmath::Dot(nextSupportPoint, dir) <= 0)
		{
			printf("NO INTERSECTION \n");
			break;
		}
		simplex[simplexIndex] = nextSupportPoint;
		simplexIndex++;
		if (DoSimplex(simplex, simplexIndex, dir))
		{
			printf("INTERSECTION \n");
			break;
		}
	}
}


bool GJKCalculator::DoSimplex(sf::Vector2f(&simplex)[3], int& simplexCount, sf::Vector2f& dir)
{
	sf::Vector3f newDir;
	if (simplexCount == 1)
	{
		//Vertex
		dir = -simplex[0];
	}
	else if (simplexCount == 2)
	{
		//Edge
		sf::Vector2f ab = simplex[0] - simplex[1];
		if (sfmath::SameDirection(ab, -simplex[1]))
		{
			newDir = sfmath::Cross3D(sfmath::Cross3D(sf::Vector3f(ab.x, ab.y, 0), -sf::Vector3f(simplex[1].x, simplex[1].y, 0)), sf::Vector3f(ab.x, ab.y, 0));
			dir.x = newDir.x;
			dir.y = newDir.y;
		}
		else
		{
			dir = -simplex[1];
			std::swap(simplex[1], simplex[0]);
			simplexCount = 1;
		}
	}
	else
	{
		//Triangle
		sf::Vector3f a = sf::Vector3f(simplex[2].x, simplex[2].y, 0);
		sf::Vector3f b = sf::Vector3f(simplex[1].x, simplex[1].y, 0);
		sf::Vector3f c = sf::Vector3f(simplex[0].x, simplex[0].y, 0);

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
				
				simplexCount = 2;
				std::swap(simplex[2], simplex[1]);
			}
			else
			{
				if (sfmath::SameDirection(ab, -a))
				{
					newDir = sfmath::Cross3D(sfmath::Cross3D(ab, -a), ab);
					dir.x = newDir.x;
					dir.y = newDir.y;

					simplexCount = 2;
					std::swap(simplex[1], simplex[0]);
					std::swap(simplex[2], simplex[1]);
				}
				else
				{
					dir = -simplex[2];
					std::swap(simplex[2], simplex[0]);
					simplexCount = 1;
				}
			}
		}
		else
		{
			//If AB is closest to origin
			if (sfmath::SameDirection(sfmath::Cross3D(ab, abc), -a))
			{
				if (sfmath::SameDirection(ab, -a))
				{
					newDir = sfmath::Cross3D(sfmath::Cross3D(ab, -a), ab);
					dir.x = newDir.x;
					dir.y = newDir.y;

					simplexCount = 2;
					std::swap(simplex[1], simplex[0]);
					std::swap(simplex[2], simplex[1]);
				}
				else
				{
					dir = -simplex[2];
					std::swap(simplex[2], simplex[0]);
					simplexCount = 1;
				}
			}
			else
				return true;
		}
	}
	return false;
}



sf::Vector2f GJKCalculator::GetSupportPoint(const std::vector<sf::Vector2f>& vertices, const sf::Vector2f& dir)
{
	float max = -FLT_MAX;
	sf::Vector2f result;
	float dot;

	for (int i = 0; i < vertices.size(); ++i)
	{
		dot = sfmath::Dot(dir, vertices[i]);
		if (dot > max)
		{
			max = dot;
			result = vertices[i];
		}
	}

	return result;
}
