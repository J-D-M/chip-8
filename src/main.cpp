#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "./chip8.hpp"

constexpr unsigned scale{ 10 };
constexpr unsigned width{ 64 };
constexpr unsigned height{ 32 };

class pixel : public sf::RectangleShape
{
       public:
	pixel(unsigned len) : sf::RectangleShape(sf::Vector2f(len, len))
	{
		setFillColor(sf::Color(255, 255, 255));
	}
};

auto
draw_screen(std::vector< std::vector< bool > > grid, sf::RenderWindow &win)
    -> void
{
	auto pix{ pixel(scale) };
	auto draw_cell = [&](auto y, auto x) {
		if (grid[y][x]) {
			pix.setPosition(x * scale, y * scale);
			win.draw(pix);
		}
	};

	for (size_t y{ 0 }; y < std::size(grid); y++) {
		for (size_t x{ 0 }; x < std::size(grid[0]); x++) {
			draw_cell(y, x);
		}
	}
}

// std::vector< std::vector< bool > > dumy(32, std::vector< bool >(64));

auto
main(int argc, char **argv) -> int
{
	using namespace chip8;
	using namespace std::literals::chrono_literals;
	//	auto lel = false;
	//	for (auto &row : dumy) {
	//		for (size_t i{ 0 }; i < dumy[0].size(); i++) {
	//			row[i] = lel;
	//			lel ^= 1;
	//		}
	//	}
	if (argc < 2) {
		std::cout << "No file path\n";
	}

	auto win{ sf::RenderWindow(sf::VideoMode(width * scale, height * scale),
		                   "chip-8") };
	win.setFramerateLimit(60);
	auto event{ sf::Event{} };

	auto emu{ emulator{} };

	if (!emu.load(argv[1])) {
		std::cout << "Failed to load game\n";
		return 0;
	}

	while (win.isOpen()) {
		while (win.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				win.close();
		}

		emu.cycle();
		win.clear();

		// draw_screen(dumy, win);
		draw_screen(emu.get_gfx(), win);

		win.display();
//		std::this_thread::sleep_for(1ms);
	}
}
