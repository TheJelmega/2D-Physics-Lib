#include "SFML/Graphics.hpp"

int main(int argc, char* argv[])
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "2D Physics Library");

	while (window.isOpen())
	{
		window.clear(sf::Color::Black);

		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				window.close();
		}

		window.display();
	}
}
