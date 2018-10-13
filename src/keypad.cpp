#include "../header/keypad.hpp"

#include <algorithm>

namespace chip8
{
const auto keypad::key_set{ std::vector< sf::Keyboard::Key >{
    sf::Keyboard::X,
    sf::Keyboard::Num1,
    sf::Keyboard::Num2,
    sf::Keyboard::Num3,
    sf::Keyboard::Q,
    sf::Keyboard::W,
    sf::Keyboard::E,
    sf::Keyboard::A,
    sf::Keyboard::S,
    sf::Keyboard::D,
    sf::Keyboard::Z,
    sf::Keyboard::C,
    sf::Keyboard::Num4,
    sf::Keyboard::R,
    sf::Keyboard::F,
    sf::Keyboard::V,
} };

keypad::keypad() : state(16, false)
{
}

auto
keypad::press(uint8_t i) -> void
{
	auto lock{ std::unique_lock< std::shared_mutex >(mutx) };
	state[i] = true;
}
auto
keypad::release(uint8_t i) -> void
{
	auto lock{ std::unique_lock< std::shared_mutex >(mutx) };
	state[i] = false;
}

auto
keypad::first() const -> std::optional< uint8_t >
{
	auto       lock{ std::shared_lock< std::shared_mutex >(mutx) };
	const auto i{ std::find(std::begin(state), std::end(state), true) };

	if (i == std::end(state))
		return {};
	return { i - std::begin(state) };
}

auto
keypad::is_down(uint8_t i) -> bool
{
	auto lock{ std::shared_lock< std::shared_mutex >(mutx) };
	return state[i];
}

}  // namespace chip8
