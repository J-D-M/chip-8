#include "./pixel.hpp"

namespace chip8
{
pixel::pixel(unsigned len)
    : sf::RectangleShape(sf::Vector2f(len, len))
{
	setFillColor(sf::Color(255, 255, 255));
}
}  // namespace chip8
