#include "./chip8.hpp"

namespace chip8
{
auto
emulator::clear_screen() -> void
{
	for (auto &row : gfx) {
		for (size_t i{ 0 }; i < std::size(row); i++)
			row[i] = 0;
	}
}

auto
emulator::draw(uint8_t x, uint8_t y, uint8_t count) -> void
{
	v[0xf] = 0;

	for (uint8_t i{ 0 }; i < count; i++) {
		auto y_i = (y + i) % std::size(gfx);
		uint8_t num{ memory[index_register + i] };
		for (auto b{ 0 }; b < 8; b++) {

			auto x_i = (x + 7 - b) % std::size(gfx[0]);
			bool bit = (num >> b) & 1;

			if (bit && gfx[y_i][x_i])
				v[0xf] = 1;

			gfx[y_i][x_i] = bit ^ gfx[y_i][x_i];
		}
	}
}
}  // namespace chip8
