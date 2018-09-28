#pragma once

#include <cstdint>
#include <functional>
#include <unordered_map>

#include "./mem.hpp"

namespace chip8
{
const auto op_codes{
	std::unordered_map< uint16_t, std::function< void(uint16_t) > >{
	    /* CLS    */ { 0x00E0, [](auto x) {} },
	    /* RND    */ { 0xC000, [](auto x) {} },
	    /* DRW    */ { 0xD000, [](auto x) {} },
	    /* ADD    */ { 0x7000, [](auto x) {} },
	    /* ADD_V  */ { 0x8004, [](auto x) {} },
	    /* SUB_V  */ { 0x8005, [](auto x) {} },
	    /* SUB_VN */ { 0x8007, [](auto x) {} },
	    /* ADD_I  */ { 0xF01E, [](auto x) {} },
	    /* */ { 0x0, [](auto x) {} },
	    /* */ { 0x0, [](auto x) {} },
	    /* */ { 0x0, [](auto x) {} },
	    /* */ { 0x0, [](auto x) {} },
	    /* */ { 0x0, [](auto x) {} },
	    /* */ { 0x0, [](auto x) {} },
	    /* */ { 0x0, [](auto x) {} },
	    /* */ { 0x0, [](auto x) {} },
	    /* */ { 0x0, [](auto x) {} },
	    /* */ { 0x0, [](auto x) {} },
	    /* */ { 0x0, [](auto x) {} },
	}
};

auto
eval(uint16_t arg)
{
	for (const auto& [key, func] : op_codes) {
		if ((arg & key) == key) {
			func(key);
			return;
		}
	}
}

/*
 ***** Misc operations ****
 *
 *
 * CLS = 0x00E0,  // Clear display
 *
 * RND = 0xC000,  // RND 0xCxkk; v[x] := (rand_num() % 256) & kk
 *
 * DRW = 0xD000,  // DRAW 0xDxyn; Display n-byte sprite starting at
 *                // memory
 *                // location I at coordinates (v[x], v[y])
 *
 ***** Arithmetic operations ****
 *
 *
 * ADD = 0x7000,  // ADD 0x7xkk; v[x] := v[y] + kk
 *
 * ADD_V  = 0x8004,  // ADD_V   0x8xy4; v[x] := v[y] + v[y]
 * SUB_V  = 0x8005,  // SUB_V   0x8xy4; v[x] := v[y] - v[y]
 * SUB_VN = 0x8007,  // SUB_VN  0x8xy4; v[x] := v[y] - v[y]
 *
 * ADD_I = 0xF01E,  // ADD_I 0xFx1E; I    := I + v[x]
 *
 *
 **** Bit manipulation ****
 *
 *
 * OR   = 0x8001, // OR  0x8xy1; v[x] := v[y] ^ v[y]
 * AND  = 0x8002, // AND 0x8xy2; v[x] := v[y] & v[y]
 * XOR  = 0x8003, // XOR 0x8xy3; v[x] := v[y] | v[y]
 * SH_R = 0x8006, // SHR 0x8xy5; v[x] := v[y] >> v[y]
 * SH_L = 0x800E, // SHL 0x8xy5; v[x] := v[y] << v[y]
 *
 **** Control flow operations ****
 *
 * * Subroutines *
 *
 * RET  = 0x00EE,  // Return from a subroutine
 * CALL = 0x2000,  // CALL 0x2nnn; call subroutine at nnn
 *
 * * Jumps *
 *
 * JMP    = 0x1000,  // JMP    0x1nnn; jump to location nnn
 * JMP_V0 = 0xB000,  // JMP_V0 0xBadr; jump to location adr + v[0]
 *
 * * Skips *
 *
 * SKP_E  = 0x3000,  // SKP_E  0x3xkk; skip  if v[x] == kk
 * SKP_NE = 0x4000,  // SKP_NE 0x4xkk; skip  if v[x] != kk
 *
 * * Skips working on two registers *
 * SKP_EV  = 0x5000,  // SKP_EV  0x5xy0; skip if v[x] == v[y]
 * SKP_NEV = 0x9000,  // SKP_NEV 0x5xy0; skip if v[x] != v[y]
 * SKP_KEY = 0xE09E,  // SKP_K   0xEx9E; skip if v[x] value is pressed
 *
 **** Load operations ****
 *
 * MOV   = 0x6000,  // MOV   0x6xkk; v[x] := kk
 * MOV_I = 0xA000,  // MOV_I 0xAadr; I := adr
 * MOV_V = 0x8000,  // MOV_V 0x8xy0; v[x] := v[y]
 *
 * // Load key press value into register
 * MOV_KEY = 0xF00A,  // MOV_KEY 0xFx0A; wait for keepress and set
 *                    // v[x] := value of key
 * // Load delay timer into register
 * MOV_DT = 0xF007,  // MOV_DT 0xFx07; v[x] := delay_timer
 *
 * // Set timer to register value
 * SET_DT = 0xF015,  // SET_DT 0xFx15; delay_timer := v[x]
 * SET_ST = 0xF018,  // SET_ST 0xFx15; sound_timer := v[x]
 *
 * MOV_FV = 0xF029,  // MOV_FV 0xFx29;
 * // I :=(location of sprite corresponding to value of v[x])
 *
 * // dump values
 * DMP_DECI = 0xF033,  // DMP_DECI 0xFx33; store decimal value of v[x] in
 *                     // mem[I], mem[I+1], mem[I+2]
 * DMP_IV = 0xF055,    // DMP_IV 0xFx55; store registers v[0] .. v[x] in
 *                     // memory location starting at I
 *
 */
}  // namespace chip8
