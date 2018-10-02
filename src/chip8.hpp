#pragma once

#include <array>
#include <cstdint>
#include <cstdlib>
#include <ctime>

namespace chip8
{
class emulator
{
       private:
	std::array< uint8_t, 0x1000 > memory;                 // main mem
	std::array< uint8_t, 0x10 > v;                        // registers
	std::array< uint16_t, 0x10 > stack;                   // call stack
	std::array< std::array< uint8_t, 0x20 >, 0x40 > gfx;  // gfx buffer

	uint16_t op_code;          // opcode storage
	uint16_t index_register;   // I
	uint16_t program_counter;  // pc current address

	uint8_t stack_pointer;
	uint8_t delay_timer;
	uint8_t sound_timer;

	auto op0x0(uint16_t) -> void;
	auto op0x8(uint16_t) -> void;
	auto op0xF(uint16_t) -> void;

       public:
	emulator();
	auto cycle() -> void;
	auto draw() -> bool;
};
}  // namespace chip8
