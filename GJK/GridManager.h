#ifndef GRID_MANAGER_H
#define GRID_MANAGER_H

#include <vector>
#include <queue>

#include "ConcaveShape.h"
#include "ConvexShape.h"
#include "GJKCalculator.h"

class GridManager
{

public:
	GridManager();

	void GenerateGrid(int windowWidth, int windowHeight, unsigned int rows, unsigned int columns);

	void Draw(sf::RenderWindow& rw);

	void AddVertex();
	void SelectShape(const sf::Vector2f& mousePos);
	void MarkShape(const sf::Vector2f& mousePos);

	void DropShape();

	void MoveDot(const sf::Vector2f& mousePos);
	void MoveShape(const sf::Vector2f& mousePos);
	void CreateShape();
	void ShowPointer(bool val);
	void DeleteCurrentShape();

private:

	GJKCalculator m_gjkCalc;

	//TODO: Should asssign to a pointer not to m_currentShape(which should be a pointer)
	bool GetShapeContainingPoint(const sf::Vector2f& point);

	struct Node
	{
		sf::Vector2f value;
		int next = -1;
		int prev = -1;

		bool isReflex = false;
		int index;
	};

	bool IsEar(const Node& n, const std::vector<Node>& nodes, const std::vector<sf::Vector2f>& vertices);
	void CreateConcaveShape(const std::vector<sf::Vector2f>& vertices);
	void CreateConvexShape(const std::vector<sf::Vector2f>& vertices);
	void DeleteShape(std::pair<int, ShapeType> shapeDeets);

	sf::Vector2i SnapToGrid(const sf::Vector2f& pos);

	int m_windowHeight;
	int m_windowWidth;
	int m_rowCount;
	int m_columnCount;
	float m_xSpacing;
	float m_ySpacing;

	const std::pair<int, ShapeType> NULL_SHAPE = std::make_pair(-1, ShapeType::Concave);
	std::pair<int, ShapeType> m_currentShape = NULL_SHAPE;
	std::pair<int, ShapeType> m_shapeA = NULL_SHAPE;
	std::pair<int, ShapeType> m_shapeB = NULL_SHAPE;
	std::pair<int, ShapeType> m_invertedB = NULL_SHAPE; //This is used for reference and is cleared each time a new difference is calulated

	bool m_drawPointer;
	sf::CircleShape m_pointer;

	std::vector<sf::RectangleShape> m_gridLines;
	std::vector<sf::Vector2f> m_shapeVertexPos;
	std::vector<sf::CircleShape> m_shapeVertices;
	bool m_shapeIsConcave = false;
	


	std::vector<ConvexShape> m_convexShapes;
	std::vector<ConcaveShape> m_concaveShapes;

	std::queue<int> m_unusedConvexShapes;
	std::queue<int> m_unusedConcaveShapes;

	float m_shoelaceSum = 0;

	static const int LINE_WIDTH = 2;
	static const int CIRCLE_RADIUS = 4;

	static const sf::Color MARKED_A;
	static const sf::Color MARKED_B;
	sf::Vector2f ORIGIN_OFFSET;
	sf::Font m_labelFont;
};

#endif
