#include "../header/gfx.hpp"

namespace chip8
{
gfx::gfx() : height{ 32 }, width{ 64 }, buf(32, std::vector< bool >(64, false))
{
}

auto
gfx::draw_ln(size_t x, size_t y, uint8_t num) -> bool
{
	auto lock{ std::unique_lock< std::shared_mutex >(mutx) };

	bool ret{ false };

	y %= height;

	for (size_t i{ 0 }; i < 8; i++) {
		size_t j = (x + i) % width;
		bool   b = (num >> (7 - i)) & 1;

		if (buf[y][j] & b)
			ret = true;

		buf[y][j] = b ^ buf[y][j];
	}

	return ret;
}

auto
gfx::clear() -> void
{
	auto lock{ std::unique_lock< std::shared_mutex >(mutx) };
	for (auto &row : buf)
		for (size_t i{ 0 }; i < std::size(row); i++)
			row[i] = false;
}

auto
gfx::get_buf() const -> std::vector< std::vector< bool > >
{
	auto lock{ std::shared_lock< std::shared_mutex >(mutx) };
	return buf;
}

}  // namespace chip8
