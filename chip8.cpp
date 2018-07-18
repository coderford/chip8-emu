#include "chip8.h"

void chip8::initialize() {
	// Initialize registers and memory once

	pc = 0x200;			// Program counter starts at 0x200
	opcode = 0;			// Reset current opcode
	I = 0;				// Reset index register
	sp = 0;				// Reset stack pointer
	drawFlag = false;   // Reset drawFlag

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
				case 0x0000:			// 00E0: Clears the screen
					gfx = {0};
					pc += 2;
				break;

				case 0x000E:			// 00EE: Return from subroutine
					if(sp == 0) {
						printf("STACK UNDERFLOW\n");
						// abort??
					} else {
						sp--;
						pc = stack[sp];
					}
				break;

				default:
					printf("Unknown opcode [0x0000]: 0x%X\n", opcode);
		}
		break;

		case 0x1000:			// 1NNN: jump to NNN
			pc = opcode & 0x0FFF;
		break;

		case 0x2000:			// 2NNN: call subroutine at address NNN
			stack[sp] = pc;		// store return address
			sp++;
			pc = opcode & 0xFFF;	// set pc to subroutine address
		break;

		case 0x3000:			// 3XNN: skip next if VX = NN
			if(V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
				pc += 4;
			else 
				pc += 2;
		break;

		case 0x4000:			// 4XNN: Skip next if VX != NN
			if(V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
				pc += 4;
			else
				pc += 2;
		break;

		case 0x5000:			// 5XY0: Skip if VX = VY
			if(V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
				pc += 4;
			else
				pc += 2;
		break;

		case 0x6000:			// 6XNN: Set VX = NN
			V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
			pc += 2;
		break;

		case 0x7000:			// 7XNN: Set VX = VX + NN
			V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
			pc += 2;
		break;

		case 0x8000:
			switch(opcode & 0x000F) {
				case 0x0000:	// 8XY0: Set VX = VY
					V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
					pc += 2;
				break;

				case 0x0001:	// 8XY1: Set VX = VX OR VY
					V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
					pc += 2;
				break;

				case 0x0002:	// 8XY2: Set VX = VX AND VY
					V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
					pc += 2;
				break;

				case 0x0003:	// 8XY3: Set VX = VX XOR VY
					V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
					pc += 2;
				break;

				case 0x0004:	// 8XY4: add VY to VX
					if(V[(opcode & 0x00F0) >> 4] > \
									(0xFF - V[(opcode & 0x0F00) >> 8])) {
						V[0xF] = 1; // set VF to indicate carry
					else
						V[0xF] = 0;
					V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
					pc += 2;
				break;

				case 0x0005:	// 8XY5: Set VX = VX - VY, VF = NOT borrow
					if(V[(opcode & 0x00F0) >> 4] < V[(opcode & 0x0F00) >> 8])
						V[0xF] = 1;	// set VF to indicate NO borrow
					else 
						V[0xF] = 0;
					V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
					pc += 2;
				break;

				case 0x0006:	// 8XY6: Set VX = VX SHR 1
					if(V[(opcode & 0x0F00) >> 8] & 0x0001 == 1)
						V[0xF] = 1;
					else
						V[0xF] = 0;
					V[(opcode & 0x0F00) >> 8] >>= 1;
					pc += 2;
				break;

				case 0x0007:	// 8XY7: Set VX = VY - VX, VF = NOT borrow
					if(V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8])
						V[0xF] = 1;	// set VF to indicate NO borrow
					else 
						V[0xF] = 0;
					V[(opcode & 0x0F00) >> 8] = 
						V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
					pc += 2;
				break;

				case 0x000E:	// 8XYE: Set VX = VX SHL 1
					if(V[(opcode & 0x0F00) >> 8] & 0x8000 != 0)
						V[0xF] = 1;
					else
						V[0xF] = 0;
					V[(opcode & 0x0F00) >> 8] <<= 1;
					pc += 2;
				break;

				default:
					printf("Unknown opcode [0x8000]: 0x%X\n", opcode);
					pc += 2;
			}
		break;

		case 0x9000:	// 9XY0: Skip next if VX != VY
			if(V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
				pc += 4;
			else 
				pc += 2;
		break;

		case 0xA000:	// ANNN: Set I = NNN
			I = opcode & 0x0FFF;
			pc += 2;
		break;

		case 0xB000:	// BNNN: Jump to NNN + V0
			pc = (opcode & 0x0FFF) + V[0];
		break;

		case 0xC000:	// CXKK: Set VX = pseudorandom_number & KK
			V[(opcode & 0x0F00) >> 8] = ;//rand & (opcode & 0x00FF);
			pc += 2;
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
				case 0x0007:	// FX07: Set VX = delay_timer
					V[(opcode & 0x0F00) >> 8] = delay_timer;
					pc += 2;
				break;

				case 0x000A:	// FX0A: Wait for keypress, store key in VX
					// to be implemented
					pc += 2;
				break;

				case 0x0015:	// FX15: Set delay_timer = VX
					delay_timer = V[(opcode & 0x0F00) >> 8];
					pc += 2;
				break;

				case 0x0018:	// FX18: Set sound_timer = VX
					sound_timer = V[(opcode & 0x0F00) >> 8];
					pc += 2;
				break;

				case 0x001E:	// FX1E: Set I = I + VX
					if(I + V[(opcode & 0x0F00) >> 8] > 0xFFF)
						V[0xF] = 1;		// this overflow check is actually
					else				// an undocumented feature used by 
						V[0xF] = 0;		// the game Spacefight 2091!
					I += V[(opcode & 0x0F00) >> 8];
					pc += 2;
				break;

				case 0x0029:	// FX29: Set I = sprite_addr[VX]
					I = 0x50 + 5 * V[(opcode & 0x0F00) >> 8]
					//				^ assuming VX stores a character (0-F)
					pc += 2;
				break;

				case 0x0033:	// FX33: Store BCD of VX at I, I+1 and I+2
					memory[I]		= V[(opcode & 0x0F00) >> 8] / 100;
					memory[I + 1]	= (V[(opcode & 0x0F00) >> 8] / 10) % 10; 
					memory[I + 2]	= (V[(opcode & 0x0F00) >> 8] % 100) % 10; 
					pc += 2; 
				break;

				case 0x0055:	// FX55: Store V0..VX in mem starting at M(I)
					for(int i = 0; i<=((opcode & 0x0F00) >> 8); i++)
						memory[I+i] = V[i];
					pc += 2;
				break;
					
				case 0x0065:	// FX65: Read V0..VX from mem starting at M(I)
					for(int i = 0; i<=((opcode & 0x0F00) >> 8); i++)
						V[i] = memory[I+i];
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
	FILE *fp = fopen(file_name, "rb");	// open file in binary mode

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
