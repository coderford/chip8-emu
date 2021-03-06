#ifndef CHIP8_H			// Header guard
#define CHIP8_H

#include <cstdio>		// for loadGame() function
#include <cstdlib>
#include <ctime>		// for seeding pseudorandom number generator

class chip8 {
	private:
		unsigned short opcode;				// stores current opcode
		unsigned char memory[4096];			// 1-byte words
		unsigned char V[16];				// general purpose CPU registers
		unsigned short I;					// index register
		unsigned short pc;					// program counter

		unsigned char delay_timer;			// these timers count down at 60Hz
		unsigned char sound_timer;

		unsigned short stack[16];			// call stack
		unsigned short sp;					// stack pointer
		
	public:
		unsigned char gfx[64 * 32];			// graphics array
		unsigned char key[16];				// hex keypad
		bool drawFlag;						// chip 8 doesn't draw each cycle

		void initialize();
		void emulateCycle();
		void loadGame(char *);
		void debugRender();
};

#endif
