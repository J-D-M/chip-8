#pragma once

namespace chip8
{
enum op_code {
	/*
	 * unused
	 * SYS = 0x00,
	 */

	CLS  = 0x00E0, /* Clear Display */
	RET  = 0x00EE, /* Return from a subroutine */
	JP   = 0x1000, /* Jump 0x1nnn; jump to location nnn */
	CALL = 0x2000, /* CALL 0x2nnn; call subroutine at nnn */
	SE   = 0x3000, /* SE   0x3xkk; skip next instruction if v[x] == kk */
	SNE  = 0x4000, /* SNE  0x4xkk; skip next instruction if v[x] != kk */
	SEV  = 0x5000, /* SEV  0x5xy0; skip next instruction if v[x] == v[y] */
	LD   = 0x6000, /* LD   0x6xkk; v[x] := kk */
	ADD  = 0x7000, /* ADD  0x7xkk; v[x] := v[x] + kk */
	LDV  = 0x8000, /* LD   0x8xy0; v[x] := v[y] */
	ORV  = 0x8001, /* ORV  0x8xy1; v[x] := v[x] ^ v[y] */


	    8xy2
	    - AND Vx,
	Vy Set Vx = Vx AND Vy.

	            Performs a bitwise AND on the values of Vx
	            and Vy,
	then stores the result in Vx.A bitwise AND compares the corrseponding
	    bits from two values,
	and if both bits are 1,
	then the same bit in the result is also 1. Otherwise,
	it                                 is 0.

	    8xy3
	    - XOR Vx,
	Vy Set Vx = Vx XOR Vy.

	            Performs a bitwise exclusive OR on the values of Vx
	            and Vy,
	then stores the result in Vx.An exclusive OR compares the corrseponding
	    bits from two values,
	and if the bits are not both the same,
	then the corresponding bit in the result is set to 1. Otherwise,
	it                                              is 0.

	    8xy4
	    - ADD Vx,
	Vy Set Vx = Vx + Vy,
	set    VF = carry.

	         The values of Vx
	         and Vy are added together.If the result is greater than 8 bits(
	                 i.e.,
	                 > 255, ) VF is set to 1,
	otherwise 0. Only the lowest 8 bits of the result are kept,
	and stored in Vx.

	    8xy5
	    - SUB Vx,
	Vy Set Vx = Vx - Vy,
	set VF    = NOT borrow.

	         If Vx
	         > Vy,
	then VF is set    to 1,
	otherwise 0. Then Vy is subtracted from Vx,
	and the results stored in Vx.

	    8xy6
	    - SHR Vx{, Vy } Set Vx
	= Vx SHR 1.

	  If the        least
	  - significant bit of Vx is 1,
	then VF is set    to 1,
	otherwise 0. Then Vx is divided by 2.

	    8xy7
	    - SUBN Vx,
	Vy Set Vx = Vy - Vx,
	set VF    = NOT borrow.

	         If Vy
	         > Vx,
	then VF is set    to 1,
	otherwise 0. Then Vx is subtracted from Vy,
	and the results stored in Vx.

	    8xyE
	    - SHL Vx{, Vy } Set Vx
	= Vx SHL 1.

	  If the        most
	  - significant bit of Vx is 1,
	then VF is set to 1,
	otherwise to 0. Then Vx is multiplied by 2.

	    9xy0
	    - SNE Vx,
	Vy Skip next instruction if Vx
	    != Vy.

	       The values of Vx and Vy are compared,
	and if they                        are not equal,
	the program counter is increased by 2.

	    Annn
	    - LD I,
	addr Set I = nnn.

	             The value of register I is set to nnn.

	             Bnnn
	             - JP V0,
	addr Jump to location nnn
	    + V0.

	      The program counter is set to nnn plus the value of V0.

	      Cxkk
	    - RND Vx,
	byte Set Vx = random byte AND kk.

};
}
