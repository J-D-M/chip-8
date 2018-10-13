#pragma once

#include <cstdint>
#include <shared_mutex>
#include <vector>

/*
 * Graphics buffer
 */

namespace chip8
{
class gfx
{
       public:
	gfx();
	auto draw_ln(size_t, size_t, uint8_t) -> bool;
	auto clear() -> void;
	auto get_buf() const -> std::vector< std::vector< bool > >;

       private:
	size_t height;
	size_t width;

	std::vector< std::vector< bool > > buf;
	mutable std::shared_mutex mutx;
};
}  // namespace chip8
