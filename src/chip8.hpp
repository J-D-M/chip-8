#pragma once

#include <array>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <unordered_map>

#include "./mem.hpp"
#include "./opcode.hpp"
#include "./stack.hpp"

namespace chip8
{
/*
 * return nth hex digit starting from the right
 * eg isolate< 2 >(0xFA0) -> 0xF
 */
template < size_t N >
constexpr auto
isolate(uint16_t x) -> uint8_t
{
	return (x >> (4 * N)) & 0xf;
};

class chip8
{
       public:
	chip8();

       private:
	std::array< uint8_t, 0x1000 > memory;
	std::array< uint8_t, 0xf >    v;  // registers
	std::array< uint8_t, 0x10 >   stack;

	uint8_t stack_pointer;

	uint16_t op_code;
	uint16_t index_register;   // I
	uint16_t program_counter;  // pc

	std::array< std::array< uint8_t, 0x20 >, 0x40 > gfx;

	uint8_t delay_timer;
	uint8_t sound_timer;

	auto op_handler(uint16_t) -> void;

	//	const std::unordered_map< uint16_t, std::function<
	// void(uint16_t) > > 	    op_codes{
	//		    /* TODO CLS */ { 0x00E0, [](auto x) {} },
	//		    /* RND */
	//		    { 0xC000,
	//		      [this](uint16_t x) {
	//		              memory[isolate< 2 >(x)]
	//		                  = (std::rand() % 256) & (x & 0xff);
	//		      } },
	//		    /* TODO DRW */ { 0xD000, [](auto x) {} },
	//		    /* ADD */
	//		    { 0x7000,
	//		      [this](uint16_t x) {
	//		              memory[isolate< 2 >(x)] += (x & 0xff);
	//		      } },
	//		    /* ADD_V */
	//		    { 0x8004,
	//		      [this](uint16_t x) {
	//		              two_reg_op(x, [this](auto &l, auto &r) {
	//			              v[0xF] = l + r > 256;
	//			              l += r;
	//		              });
	//		      } },
	//		    /* SUB_V */
	//		    { 0x8005,
	//		      [this](uint16_t x) {
	//		              two_reg_op(x, [this](auto &l, auto &r) {
	//			              v[0xf] = l > r;
	//			              l -= r;
	//		              });
	//		      } },
	//		    /* SUB_VN */
	//		    { 0x8007,
	//		      [this](uint16_t x) {
	//		              two_reg_op(x, [this](auto &l, auto &r) {
	//			              v[0xf] = l < r;
	//			              l -= r;
	//		              });
	//		      } },
	//		    /* ADD_I */
	//		    { 0xF01E,
	//		      [this](uint16_t x) {
	//		              index_register += memory[isolate< 2 >(x)];
	//		      } },
	//		    /* OR */
	//		    { 0x0,
	//		      [this](uint16_t x) {
	//		              two_reg_op(x, [](auto &l, auto &r) { l |=
	// r;
	//}); 		      } },
	//		    /* TODO AND */ { 0x0, [](auto x) {} },
	//		    /* TODO XOR */ { 0x0, [](auto x) {} },
	//		    /* TODO */ { 0x0, [](auto x) {} },
	//		    /* TODO */ { 0x0, [](auto x) {} },
	//		    /* TODO */ { 0x0, [](auto x) {} },
	//		    /* TODO */ { 0x0, [](auto x) {} },
	//		    /* TODO */ { 0x0, [](auto x) {} },
	//		    /* TODO */ { 0x0, [](auto x) {} },
	//		    /* TODO */ { 0x0, [](auto x) {} },
	//		    /* TODO */ { 0x0, [](auto x) {} },
};
}  // namespace chip8
