#pragma once
#include <SFML/Graphics.hpp>

namespace chip8
{
class pixel : public sf::RectangleShape
{
       public:
	pixel(unsigned len);
};
}  // namespace chip8
