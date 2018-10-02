#include "./chip8.hpp"

namespace chip8
{
// get nth nibble from the right starting at 0
template < size_t N >
auto
isolate(uint16_t x) -> uint8_t
{
	return (x >> (4 * N)) & 0xf;
}

emulator::emulator()
{
	std::srand(std::time(nullptr));
}

auto
emulator::op0x0(uint16_t x) -> void
{
	switch (x) {
	// TODO CLEAR DISPLAY
	// 		case 0x00E0:
	//			CLS();
	//			break;
	// RETURN
	case 0x00EE:
		program_counter = stack[stack_pointer];
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
		if (reg1 > reg2) // TODO Bug? (should be >= ?)
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
		// TODO
		//	case 0x29:
		//		  index_register = gfx[reg];
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

auto
emulator::cycle() -> void
{
	op_code = (memory[program_counter] << 8) | memory[program_counter + 1];
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
	// 8 codes, bit manipulation and arithmetic working on two registers
	case 8:
		op0x8(op_code);
		break;
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
	// TODO draw v[x] v[y], nibble
	case 0xD:
		break;
	// TODO SKP's working on keypress
	case 0xE:
		break;
	// 0xF misc operations on I and timers
	case 0xF:
		op0xF(op_code);
		break;
	default:
		break;
	};

	program_counter += 2;
}

};  // namespace chip8
