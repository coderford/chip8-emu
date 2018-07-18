// #include graphics and input libraries
#include "chip8.h"	// cpu core implementation

chip8 myChip8;

void drawGraphics();

int main(int argc, char **argv) {
	if(argc < 2) {
		printf("Usage: chip8-emu <chip-8 application>\n\n");
		return 1;
	}
	// Set up render system and register input callbacks
	// setupGraphics();
	// setupInput();

	// Initialize the Chip8 system and load the game into the memory
	myChip8.initialize();
	myChip8.loadGame(argv[1]);

	// Emulation loop
	for(;;) {
		// Emulate one cycle
		myChip8.emulateCycle();

		// If the draw flag is set, update the screen
		if(myChip8.drawFlag)
			drawGraphics();

		// Store key press state (Press and Release)
		// myChip8.setKeys();
	}

	return 0;
}

void drawGraphics() {
	system("clear");
	myChip8.debugRender();
	myChip8.drawFlag = false;
}
