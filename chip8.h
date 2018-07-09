 
class chip8 {
	private:
		unsigned short opcode;				// stores current opcode
		unsigned char memory[4096];			// 1-byte words
		unsigned char V[16];				// general purpose CPU registers
		unsigned short I;					// index register
		unsigned short pc;					// program counter

		unsigned char gfx[64 * 32];			// graphics array

		unsigned char delay_timer;			// these timers count down at 60Hz
		unsigned char sound_timer;

		unsigned short stack[16];			// call stack
		unsigned short sp;					// stack pointer

		unsigned char key[16];				// hex keypad

	public:
		void initialize();
		void emulateCycle();
}

void chip8::initialize() {
	// Initialize registers and memory once
}

void chip8::emulateCycle() {
	// Fetch opcode
	opcode = memory[pc] << 8 | memory[pc+1];
	// ^ we need to read memory twice as each opcode is 2 bytes while the_
	// memory words are only one byte

	// Decode and execute opcode
	if((opcode & 0xF000) ^ 0xA000 == 0) { // for opcode 0xANNN, set I to NNN
		I = opcode & 0x0FFF;
		pc += 2;
	}

	// Update timers
}
