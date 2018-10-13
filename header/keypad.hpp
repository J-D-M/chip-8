#pragma once

#include <SFML/Window.hpp>
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

namespace chip8
{
class keypad
{
       public:
	keypad();

	auto press(uint8_t) -> void;
	auto release(uint8_t) -> void;
	auto first() const -> std::optional< uint8_t >;
	auto is_down(uint8_t) -> bool;

	const static std::vector< sf::Keyboard::Key > key_set;

       private:
	std::vector< bool > state;
	mutable std::shared_mutex mutx;
};
}  // namespace chip8
