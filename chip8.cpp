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

		case 0x2000:			// 0x2NNN: call subroutine at address NNN
			stack[sp] = pc;		// store return address
			sp++;
			pc = opcode & 0xFFF;	// set pc to subroutine address
		break;

		case 0x8000:
			switch(opcode & 0x000F) {
				case 0x0004:	// 0x8XY4: add VY to VX
					if(V[(opcode & 0x00F0) >> 4] > \
									(0xFF - V[(opcode & 0x0F00) >> 8])) {
						V[0xF] = 1; // set VF to indicate carry
					else
						V[0xF] = 0;
					V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
					pc += 2;
				break;

				default:
					printf("Unknown opcode [0x8000]: 0x%X\n", opcode);
			}
		break;

		case 0xD000: {	// DXYN: Draw sprite of height N at (X, Y) from I
			unsigned short x = V[(opcode & 0x0F00) >> 8];
			unsigned short y = V[(opcode & 0x00F0) >> 4];
			unsigned short height = opcode & 0x000F;
			unsigned short pixelbyte;
			// ^ stores 8 horizontal black/white pixels

			V[0xF] = 0;					// Reset VF

			for(int yline = 0; yline < height; yline++) {
				pixelbyte = memory[I + yline];	// Get byte of pixels from I
				for(int xline = 0; xline < 8; xline++) {
					if((pixelbyte & (0x80 >> xline)) != 0) {
					// ^ check if xline'th pixel in current byte is set
						if(gfx[(x + xline + ((y + yline) * 64))] == 1)
							V[0xF] = 1;
						gfx[x + xline + ((y + yline) * 64)] ^= 1;
					}
				}
			}

			drawFlag = true;
			pc += 2;
		}
		break;

		case 0xE000:
			switch(opcode & 0x00FF) {
				case 0x009E:  //EX9E: skip next instruction if key in VX on
					if (key[V[(opcode & 0x0F00) >> 8]] != 0)
						pc += 4;
					else 
						pc += 2;
				break;
				
				case 0x00A1:  //EXA1: skip next instruction if key in vx off
					if (key[V[(opcode & 0x0F00) >> 8]] == 0)
						pc += 4;
					else 
						pc += 2;
				break;

				default:
					printf("Unknown opcode [0xE000]: 0x%X\n", opcode);
					
			}
		break;

		case 0xF000:
			switch(opcode & 0x00FF) {
				case 0x0033:	// 0xFX33: store BCD of VX at I, I+1 and I+2
					memory[I]		= V[(opcode & 0x0F00) >> 8] / 100;
					memory[I + 1]	= (V[(opcode & 0x0F00) >> 8] / 10) % 10;
					memory[I + 2]	= (V[(opcode & 0x0F00) >> 8] % 100) % 10;
					pc += 2;
				break;

				default:
					printf("Unknown opcode [0xF000]: 0x%X\n", opcode);
			}
		break;

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

	free(buffer);
	fclose(fp);
}
