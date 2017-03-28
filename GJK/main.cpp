#include <iostream>

#include "GridManager.h"

const int WINDOW_HEIGHT = 800;
const int WINDOW_WIDTH = 800;

enum InputState
{
	DrawingShape,
	SelectingShape
};

InputState m_state = DrawingShape;
int main()
{
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "GJK");
	GridManager gm;
	gm.GenerateGrid(WINDOW_WIDTH, WINDOW_HEIGHT, 40, 40);
	gm.ShowPointer(true);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				//Place points
				if (m_state == DrawingShape)
					gm.AddVertex();
				else if (m_state == SelectingShape)
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
						gm.MarkShape(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
					else
						gm.SelectShape(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
				}
			}
			if (event.type == sf::Event::MouseMoved)
			{
				if (m_state == DrawingShape)
					gm.MoveDot(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
				else if (m_state == SelectingShape && sf::Mouse::isButtonPressed(sf::Mouse::Left))
					gm.MoveShape(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
					
			}
			if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
			{
				if (m_state == SelectingShape)
					gm.DropShape();
			}
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Space)
				{
					if (m_state == DrawingShape)
						gm.CreateShape();
					else
						gm.CalculateGJK();
				}
				else if (event.key.code == sf::Keyboard::Delete || event.key.code == sf::Keyboard::BackSpace)
				{
					if (m_state == SelectingShape)
						gm.DeleteCurrentShape();
				}
				else if (event.key.code == sf::Keyboard::Q)
				{
					if (m_state == SelectingShape)
					{
						m_state = DrawingShape;
						gm.ShowPointer(true);
					}
					else
					{
						m_state = SelectingShape;
						gm.ShowPointer(false);
					}

				}
			}
		}

		window.clear(sf::Color::White);
		gm.Draw(window);
		window.display();
	}

	return 0;
}