#pragma once
#include "DrawContext.h"

struct Context
{
	DrawContext draw;

	Context(sf::RenderWindow& window)
	: draw(window)
	{
	}
};
