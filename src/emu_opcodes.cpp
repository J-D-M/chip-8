#include "../header/emulator.hpp"
#define NDEBUG 1

namespace chip8
{
namespace
{
// helper functions
template < unsigned N >
auto
get_nib(uint16_t x) -> uint16_t
{
	return (x >> (4 * N) & 0xf);
}

}  // namespace
auto
emulator::cycle() -> void
{
	const auto opt{ static_cast< uint16_t >(memory[pc] << 8
		                                | memory[pc + 1]) };
	const auto x_index{ get_nib< 2 >(opt) };

	auto &v_x{ v[x_index] };
	auto &v_y{ v[get_nib< 1 >(opt)] };

	auto halt{ bool{ false } };
	auto bad_opt{ bool{ false } };

#ifndef NDEBUG
	puts("-----------------------------------------------------------");
	std::printf(
	    "OP: %#06x\t PC: %#06x\n"
	    "I : %#06x\t SP: %#06x\t TOP: %#06x\n",
	    opt,
	    pc,
	    i_register,
	    sp,
	    stack[sp]);
	puts("-----------------------------------------------------------");
	for (size_t i{ 0 }; i <= 0xf; i++) {
		if (i % 4 == 0 && i > 0)
			std::putchar('\n');
		std::printf("V%2lu :%#06x\t", i, v[i]);
	}
	puts("\n");
#endif
	// left most nibble of optcode
	switch (get_nib< 3 >(opt)) {
	case 0x0:
		switch (opt) {
		case 0x00e0:  // clear screen
			gfx_buf.clear();
			break;
		case 0x00ee:  // return
			--sp;
			pc   = stack[sp];
			halt = true;
			break;
		default:
			bad_opt = true;
			break;
		}  // 0x0
		break;
	case 0x1:  // jmp to addr
		pc   = opt & 0xfff;
		halt = true;
		break;
	case 0x2:  // call addr
		stack[sp] = pc + 2;
		++sp;
		pc   = opt & 0xfff;
		halt = true;
		break;
	case 0x3:  // jmpe
		pc += (v_x == (opt & 0xff)) ? 2 : 0;
		break;
	case 0x4:  // jmpne
		pc += (v_x != (opt & 0xff)) ? 2 : 0;
		break;
	case 0x5:  // jmpe registers
		pc += (v_x == v_y) ? 2 : 0;
		break;
	case 0x6:  // mov
		v_x = opt & 0xff;
		break;
	case 0x7:  // add
		v_x += opt & 0xff;
		break;
	case 0x8:
		// check rightmost nib
		switch (opt & 0xf) {
		case 0x0:  // mov
			v_x = v_y;
			break;
		case 0x1:  // or
			v_x |= v_y;
			break;
		case 0x2:  // and
			v_x &= v_y;
			break;
		case 0x3:  // xor
			v_x ^= v_y;
			break;
		case 0x4:  // add
			v[0xf] = (static_cast< unsigned >(v_x + v_y) > 0xff)
			             ? 1
			             : 0;
			v_x += v_y;
			break;
		case 0x5:  // sub no borrow
			v[0xf] = (v_x > v_y) ? 1 : 0;
			v_x -= v_y;
			break;
		case 0x6:  // shr
			v[0xf] = v_x & 1;
			v_x >>= 1;
			break;
		case 0x7:  // sub borrow
			v[0xf] = (v_x < v_y) ? 1 : 0;
			v_x -= v_y;
			break;
		case 0xe:  // shl
			v[0xf] = (v_x >> 7) & 1;
			v_x <<= 1;
			break;
		default:
			bad_opt = true;
			break;
		}  // 0x8
		break;
	case 0x9:  // jmpne registers
		pc += (v_x != v_y) ? 2 : 0;
		break;
	case 0xa:  // mov
		i_register = opt & 0xfff;
		break;
	case 0xb:  // jmp
		pc   = (opt & 0xfff) + v[0];
		halt = true;
		break;
	case 0xc:  // mov rnd
		v_x = static_cast< uint8_t >(std::rand() % 256) & (opt & 0xff);
		break;
	case 0xd:  // draw
		for (size_t i{ 0 }; i < (opt & 0xf); i++) {
			if (gfx_buf.draw_ln(
			        v_x, v_y + i, memory[i_register + i])) {
				v[0xf] = 1;
			}
		}
		break;
	case 0xe:
		// lowest byte
		switch (opt & 0xff) {
		case 0x9e:  // jmp key
			pc += (k_pad.is_down(v_x)) ? 2 : 0;
			break;
		case 0xa1:  // jmp no key
			pc += (!k_pad.is_down(v_x)) ? 2 : 0;
			break;
		default:
			bad_opt = true;
			break;
		}  // 0xe
		break;
	case 0xf:
		// lowest byte
		switch (opt & 0xff) {
		case 0x07:  // mov
			v_x = d_timer;
			break;
		case 0x0a:  // mov key or halt
			if (auto key{ k_pad.first() }) {
				v_x = key.value();
			} else {
				halt = true;
			}
			break;
		case 0x15:  // mov
			d_timer = v_x;
			break;
		case 0x18:  // mov
			s_timer = v_x;
			break;
		case 0x1e:  // add
			i_register += v_x;
			break;
		case 0x29:  // mov sprite location
			i_register = (v_x * 5);
			break;
		case 0x33:  // dump deci digits of v_x
			memory[i_register]     = (v_x / 100) % 10;
			memory[i_register + 1] = (v_x / 10) % 10;
			memory[i_register + 2] = v_x % 10;
			break;
		case 0x55:  // dump registers into mem
			for (size_t i{ 0 }; i < x_index; i++) {
				memory[i_register + i] = v[i];
			}
			break;
		case 0x65:  // dump mem into registers
			for (size_t i{ 0 }; i < x_index; i++) {
				v[i] = memory[i_register + i];
			}
			break;
		default:
			bad_opt = true;
			break;
		}  // 0xf
		break;
	default:
		bad_opt = true;
		break;
	}

	if (bad_opt)
		std::printf("Unknown opcode: %#06x\n", opt);

	s_timer -= s_timer > 0 ? 1 : 0;
	d_timer -= d_timer > 0 ? 1 : 0;

	pc += halt ? 0 : 2;
}
}  // namespace chip8
