#pragma once

#include <SFML/Window.hpp>

namespace chip8
{
std::array< sf::Keyboard::Key, 0x10 > keypad{
	sf::Keyboard::Num1, sf::Keyboard::Num2, sf::Keyboard::Num3,
	sf::Keyboard::Q,    sf::Keyboard::W,    sf::Keyboard::E,
	sf::Keyboard::A,    sf::Keyboard::D,    sf::Keyboard::F,
	sf::Keyboard::X,    sf::Keyboard::Z,    sf::Keyboard::C,
	sf::Keyboard::Num4, sf::Keyboard::R,    sf::Keyboard::F,
	sf::Keyboard::V,
};
auto
pad_any() -> bool
{
	for (const auto &key : keypad) {
		if (sf::Keyboard::isKeyPressed(key))
			return true;
	}
	return false;
}
auto
first_key() -> uint8_t
{
	for (uint8_t i{ 0 }; i < 0x10; i++) {
		if (sf::Keyboard::isKeyPressed(keypad[i]))
			return i;
	}
	return 0x10;
}
}  // namespace chip8
