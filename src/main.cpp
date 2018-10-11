#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "./chip8.hpp"
#include "./pixel.hpp"

constexpr unsigned scale{ 20 };
constexpr unsigned width{ 64 };
constexpr unsigned height{ 32 };

auto
draw_screen(std::array< std::array< bool, 64 >, 32 > grid,
            sf::RenderWindow &win) -> void
{
	static auto pix{ chip8::pixel(scale) };

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

auto
main(int argc, char **argv) -> int
{
	using namespace chip8;
	if (argc < 2) {
		std::cout << "No file path\n";
	}
	//	using namespace std::literals::chrono_literals;

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
		draw_screen(emu.get_gfx() /* test_grid*/, win);
		win.display();
		//		std::this_thread::sleep_for(2ms);
	}
}
