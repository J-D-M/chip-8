#pragma once

#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

namespace chip8
{
class emulator
{
       private:
	std::vector< uint8_t > memory;  // main mem
	std::vector< uint8_t > v;       // registers
	std::vector< uint16_t > stack;  // call stack

	std::vector< std::vector< bool > > gfx;

	uint16_t op_code;
	uint16_t index_register;   // I
	uint16_t program_counter;  // pc

	uint8_t stack_pointer;
	uint8_t delay_timer;
	uint8_t sound_timer;

	// helper functions for opcodes
	auto op0x0(uint16_t) -> void;
	auto op0x8(uint16_t) -> void;
	auto op0xF(uint16_t) -> void;
	auto op0xE(uint16_t) -> void;

	// functions that change the gfx buffer
	auto clear_screen() -> void;
	auto draw(uint8_t, uint8_t, uint8_t) -> void;

	bool halt_flag;

       public:
	emulator();
	// load game
	auto load(const std::string_view&) -> bool;
	// opcode cycle
	auto cycle() -> void;
	// return graphics buffer
	auto get_gfx() -> std::vector< std::vector< bool > >;
};
}  // namespace chip8
