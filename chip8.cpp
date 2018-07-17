#include "chip8.h"

void chip8::initialize() {
	// Initialize registers and memory once

	pc = 0x200;		// Program counter starts at 0x200
	opcode = 0;		// Reset current opcode
	I = 0;			// Reset index register
	sp = 0;			// Reset stack pointer
	
	// Clear display
	// Clear stack
	// Clear registers V0-VF
	// Clear memory

	// Load fontset
	for(int i = 0; i < 0x50; i++) 
		memory[i+0x50] = chip8_fontset[i];
	
	// Reset timers
}

void chip8::emulateCycle() {
	// Fetch opcode
	opcode = memory[pc] << 8 | memory[pc+1];
	// ^ we need to read memory twice as each opcode is 2 bytes while the_
	// memory words are only one byte

	// Decode and execute opcode
	switch(opcode & 0xF000) {		// using first 4 bits to determine instr
		case 0xA000:				// ANNN: Sets I to the address NNN
			I = opcode & 0x0FFF;
			pc += 2;
		break;

		case 0x0000:
			switch(opcode & 0x000F) {	// differentiate b/w 0x00E0 & 0x00EE
				case 0x0000:			// 0x00E0: Clears the screen
					// Execute
				break;

				case 0x000E:			// 0x00EE: Return from subroutine
					// Execute
				break;

				default:
					printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
		}
		break;

		// More opcodes //
		default:
			printf("Unknown opcode: 0x%X\n", opcode);
	}

	// Update timers
	if(delay_timer > 0)
		--delay_timer;
	if(sound_timer > 0) {
		if(sound_timer == 1)
			printf("BEEP!\n");
		--sound_timer;
	}
}

void chip8::loadGame(char *file_name) {
	file *fp = fopen(file_name, "rb");	// open file in binary mode

	fseek(fp, 0, SEEK_END);				//
	int file_size = ftell(fp);			// determine the file size (in bytes)
	fseek(fp, 0, SEEK_SET);				//

	char *buffer = (char *)malloc(file_size);	// setup buffer
	fread(buffer, 1, file_size, fp);			//

	for(int i = 0; i<file_size; i++)			// load program to memory
		memory[i+0x200] = buffer[i];			//

	fclose(fp);
}
