#pragma once

#include <cstdint>

namespace chip8
{
/*
 * 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
 * 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
 * 0x200-0xFFF - Program ROM and work RAM
 */

uint8_t memory[0x100];
uint8_t v[0x10]; /* register */
uint8_t gfx[0x20][0x40];
uint8_t delay_timer;
uint8_t sound_timer;
}  // namespace chip8
