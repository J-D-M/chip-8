#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <chrono>
#include <cstdlib>
#include <functional>
#include <shared_mutex>
#include <thread>

#include "../header/emulator.hpp"

constexpr unsigned scale{ 10 };
constexpr unsigned width{ 64 };
constexpr unsigned height{ 32 };

class shared_flag
{
       private:
	mutable std::shared_mutex mutx;

	bool flag;

       public:
	shared_flag(bool f) : flag{ f }
	{
	}

	operator bool() const
	{
		auto lock{ std::shared_lock< std::shared_mutex >(mutx) };
		return flag;
	}

	auto
	operator=(bool x) -> bool
	{
		auto lock{ std::unique_lock< std::shared_mutex >(mutx) };
		flag = x;
		return flag;
	}
};

class pixel : public sf::RectangleShape
{
       public:
	pixel(unsigned len) : sf::RectangleShape(sf::Vector2f(len, len))
	{
		setFillColor(sf::Color(255, 255, 255));
	}
};

auto
draw_screen(std::vector< std::vector< bool > > gfx, sf::RenderWindow &win)
    -> void
{
	auto pix{ pixel(scale) };

	auto draw_cell = [&](auto y, auto x) {
		if (gfx[y][x]) {
			pix.setPosition(x * scale, y * scale);
			win.draw(pix);
		}
	};

	for (size_t y{ 0 }; y < std::size(gfx); y++) {
		for (size_t x{ 0 }; x < std::size(gfx[0]); x++) {
			draw_cell(y, x);
		}
	}
}

auto
update_keys(chip8::keypad &pad) -> void
{
	for (size_t i{ 0 }; i < std::size(pad.key_set); i++) {
		if (sf::Keyboard::isKeyPressed(pad.key_set[i])) {
			pad.press(i);
		} else {
			pad.release(i);
		}
	}
}

auto
emu_loop(shared_flag &quit, chip8::emulator &emu) -> void
{
	using namespace std::chrono_literals;
	const auto sleep_time{ 16ms };

	while (!quit) {
		auto t1{ std::chrono::high_resolution_clock::now() };

		emu.cycle();

		auto t2{ std::chrono::high_resolution_clock::now() };
		auto dif{ t2 - t1 };

		if (dif < sleep_time)
			std::this_thread::sleep_for(sleep_time - dif);
	}
}

auto
main(int argc, char **argv) -> int
{
	using namespace chip8;
	using namespace std::chrono_literals;

	auto gfx_buf{ gfx{} };
	auto k_pad{ keypad{} };
	auto emu{ emulator(gfx_buf, k_pad) };

	if (argc < 2 || !emu.load(argv[1])) {
		puts("Invalid file path");
		return 1;
	}

	auto quit{ shared_flag{ false } };
	auto win{ sf::RenderWindow(sf::VideoMode(width * scale, height * scale),
		                   "chip-8") };
	win.setVerticalSyncEnabled(true);
	win.setFramerateLimit(60);

	auto event{ sf::Event{} };

	auto emu_t{ std::thread(emu_loop, std::ref(quit), std::ref(emu)) };

	// main loop

	while (win.isOpen()) {
		while (win.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				win.close();
				quit = true;
			}
		}

		if (quit) {
			emu_t.join();
			continue;
		}

		update_keys(k_pad);
		win.clear();
		draw_screen(gfx_buf.get_buf(), win);
		win.display();
	}
}
