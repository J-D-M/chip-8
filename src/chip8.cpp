#include "./chip8.hpp"

#include <fstream>
#include <iostream>

namespace chip8
{
constexpr unsigned char fontset[80] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
	0x20, 0x60, 0x20, 0x20, 0x70,  // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
	0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
	0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
	0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
	0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
	0xF0, 0x80, 0xF0, 0x80, 0x80,  // F
};
// constructor
emulator::emulator()
    // sprite data
    : memory{},
      v{},
      stack{},
      gfx{},
      op_code{},
      index_register{},
      program_counter{ 0x200 },
      stack_pointer{},
      delay_timer{}
{
	std::srand(std::time(nullptr));
	for (size_t i = 0; i < 80; i++) {
		memory[0x50 + i] = fontset[i];
	}
}

// return graphics buffer
auto
emulator::get_gfx() -> std::array< std::array< bool, 64 >, 32 >
{
	return gfx;
}

auto
emulator::load(const std::string_view &file_path) -> bool
{
	auto game_file{ std::ifstream(file_path.data(), std::ios::binary) };
	if (!game_file) {
		return false;
	}
	for (size_t i{ 0x200 }; game_file.peek() != EOF; i++) {
		memory[i] = game_file.get();
	}

	return true;
}

/*
 * OP CODE cycle
 */

// helper functions
// get nth nibble from the right starting at 0
template < size_t N >
inline auto
isolate(uint16_t x) -> uint8_t
{
	return (x >> (4 * N)) & 0xf;
}

auto
emulator::clear_screen() -> void
{
	for (auto &row : gfx) {
		for (auto &cell : row) {
			cell = 0;
		}
	}
}

auto
emulator::draw(size_t x, size_t y, size_t count) -> void
{
	v[0xf] = 0;

	for (size_t i{ 0 }; i < count; i++) {
		auto y_i = (y + i) % std::size(gfx);
		for (auto b{ 7 }; b >= 0; b--) {
			auto x_i = (x + (7 - b)) % std::size(gfx[0]);
			bool bit = (memory[index_register + i] >> b) & 1;

			if (bit & gfx[y_i][x_i])
				v[0xf] = 1;

			gfx[y_i][x_i] ^= bit;
		}
	}
}

auto
emulator::op0x0(uint16_t x) -> void
{
	switch (x) {
	// CLEAR DISPLAY
	case 0x00E0:
		clear_screen();
		break;
	// Return
	case 0x00EE:
		program_counter = stack[stack_pointer];
		--stack_pointer;
		break;
	}
}

auto
emulator::op0x8(uint16_t x) -> void
{
	auto &reg1{ v[isolate< 2 >(x)] };
	auto &reg2{ v[isolate< 1 >(x)] };
	switch (x & 0xf) {
	// MOV %v[y], %v[x]
	case 0:
		reg1 = reg2;
		break;
	// OR %v[y], %v[x]
	case 1:
		reg1 |= reg2;
		break;
	// AND %v[y], %v[x]
	case 2:
		reg1 &= reg2;
		break;
	// XOR %v[y], %v[x]
	case 3:
		reg1 ^= reg2;
		break;
	// ADD %v[y], %v[x]
	case 4:
		reg1 += reg2;
		if (reg1 > 0xff) {
			v[0xf] = 1;
			reg1 &= 0xff;
		}
		break;
	// SUB %v[y], v[x]
	case 5:
		// set flag if NO borrow
		if (reg1 > reg2)  // TODO Bug? (should be >= ?)
			v[0xf] = 1;
		reg1 -= reg2;
		break;
	// SHR %v[y], 1
	case 6:
		v[0xf] = reg1 & 0x1;
		reg1 >>= 1;
		break;
	// SUBN %v[y], v[x]
	case 7:
		// set flag if borrow
		if (reg1 < reg2)
			v[0xf] = 1;
		reg1 -= reg2;
		break;
	// SHL %v[y], 1
	case 0xE:
		v[0xf] = reg1 & 0xf;
		reg1 <<= 1;
		break;
	default:
		break;
	}
}

auto
emulator::op0xF(uint16_t x) -> void
{
	const auto reg_index{ isolate< 2 >(x) };
	auto &reg{ v[reg_index] };

	switch (x & 0xf) {
	// MOV %dt, %v[x]
	case 7:
		reg = delay_timer;
		break;
	// TODO MOV key press val, %v[x]
	case 0xA:
		break;
	// MOV %v[x], %dt
	case 0x15:
		delay_timer = reg;
		break;
	// MOV %v[x], %st
	case 0x18:
		sound_timer = reg;
		break;
	// ADD %v[x], %I
	case 0x1E:
		index_register += reg;
		break;
	// Set I to location of char sprite for 0-F
	case 0x29:
		index_register = 0x50 + (op_code & 0xf) * 5;
		break;
	// dump decimal representation of reg into memory
	case 0x33:
		memory[index_register]     = (reg / 100) % 10;
		memory[index_register + 1] = (reg / 10) % 10;
		memory[index_register + 2] = reg % 10;
		break;
	// dump registers into memory
	case 0x55:
		for (size_t i = 0; i <= reg_index; i++) {
			memory[index_register + i] = v[i];
		}
		break;
	// dump memory into registers
	case 0x65:
		for (size_t i = 0; i <= reg_index; i++) {
			v[i] = memory[index_register + i];
		}
		break;
	}
}

/*
 * Main opcode function
 */

auto
emulator::cycle() -> void
{
	if (program_counter > 3998) {
		std::cout << "P_COUNTER OUT OF BOUNDS\n";
		return;
	}

	op_code = (memory[program_counter] << 8) | memory[program_counter + 1];
	std::cout << std::hex << std::showbase << "OP_CODE : " << op_code
	          << "  PC: " << program_counter << '\n';

	auto &reg1{ v[isolate< 2 >(op_code)] };
	auto &reg2{ v[isolate< 1 >(op_code)] };

	switch (isolate< 3 >(op_code)) {
	// Handle misc 0 codes
	case 0:
		op0x0(op_code);
		break;
	// GOTO
	case 1:
		program_counter = op_code & 0xfff;
		return;
		break;
	// CALL
	case 2:
		++stack_pointer;
		stack[stack_pointer] = program_counter;
		program_counter      = op_code & 0xfff;
		return;  // do not increment PC
	// JMPE
	case 3:
		if (reg1 == (op_code & 0xff))
			program_counter += 2;
		break;
	// JMPNE
	case 4:
		if (reg1 != (op_code & 0xff))
			program_counter += 2;
		break;
	// JMPE working on registers
	case 5:
		if (reg1 == reg2)
			program_counter += 2;
		break;
	// MOV %byte, %v[x]
	case 6:
		reg1 = op_code & 0xff;
		break;
	// ADD %byte, %v[x]
	case 7:
		reg1 += op_code & 0xff;
		break;
	// 8 codes, bit manipulation and arithmetic working on two
	// registers
	case 8:
		op0x8(op_code);
		break;
	// JMPNE working on registers
	case 9:
		if ((op_code & 0xf) == 0) {
			if (reg1 != reg2)
				program_counter += 2;
		}
		break;
	// MOV %addr, I
	case 0xA:
		index_register = op_code & 0xfff;
		break;
	// JMP (%addr + %v[0])
	case 0xB:
		program_counter = (op_code & 0xfff) + v[0];
		return;  // do not increment PC
	// MOV v[x], (RND)
	case 0xC:
		reg1 = static_cast< uint8_t >(std::rand() % 256)
		       & (op_code & 0xff);
		break;
	// draw v[x] v[y], nibble
	case 0xD:
		draw(reg1, reg2, op_code & 0xf);
		break;
	// TODO SKP's working on keypress
	case 0xE:
		break;
	// 0xF misc operations on I and timers
	case 0xF:
		op0xF(op_code);
		break;
	default:
		std::cout << "UNKNOWN OPCODE: " << std::hex << std::showbase
		          << op_code << std::endl;
		break;
	};

	delay_timer -= 1;
	sound_timer -= 1;
	program_counter += 2;
}

};  // namespace chip8
