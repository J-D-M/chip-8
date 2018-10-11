#pragma once

#include <array>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <vector>

namespace chip8
{
class emulator
{
       private:
	std::array< uint8_t, 0x1000 > memory;  // main mem
	std::array< uint8_t, 0x10 > v;         // registers
	std::array< uint16_t, 0x10 > stack;    // call stack
	std::array< std::array< bool, 64 >, 32 > gfx;

	uint16_t op_code;          // opcode storage
	uint16_t index_register;   // I
	uint16_t program_counter;  // pc current address

	uint8_t stack_pointer;
	uint8_t delay_timer;
	uint8_t sound_timer;

	// helper functions for opcodes
	auto op0x0(uint16_t) -> void;
	auto op0x8(uint16_t) -> void;
	auto op0xF(uint16_t) -> void;

	// functions that change the gfx buffer
	auto clear_screen() -> void;
	auto draw(size_t, size_t, size_t) -> void;

       public:
	emulator();

	auto load(const std::string_view&) -> bool;
	auto cycle() -> void;
	auto get_gfx() -> std::array< std::array< bool, 64 >, 32 >;
};
}  // namespace chip8
