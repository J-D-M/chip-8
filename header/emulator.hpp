#pragma once

#include "./gfx.hpp"
#include "./keypad.hpp"

namespace chip8
{
class emulator
{
       public:
	emulator(gfx &, keypad &);
	auto cycle() -> void;
	auto load(const std::string &) -> bool;

       private:
	uint8_t memory[0x1000];
	uint8_t v[0x10];

	uint16_t stack[0x10];

	uint16_t i_register;
	uint16_t pc;

	uint8_t sp;
	uint8_t d_timer;
	uint8_t s_timer;

	gfx &gfx_buf;

	keypad &k_pad;
};
}  // namespace chip8
