// #include graphics and input libraries
#include "chip8.h"	// cpu core implementation
#include <SFML/Graphics.hpp>

chip8 myChip8;

void drawGraphics(sf::RenderWindow *);
sf::VertexArray screen(sf::Points, 2048);

int main(int argc, char **argv) {
	if(argc < 2) {
		printf("Usage: chip8-emu <chip-8 application>\n\n");
		return 1;
	}
	// Create window and set framerate
	sf::RenderWindow window(sf::VideoMode(164, 132), "Chip-8 Emulator");
	window.setFramerateLimit(60);

	for(int i = 0; i<2048; i++) {
		screen[i].position = sf::Vector2f(i%64+50, i/64+50);
	}
	// setupInput();

	// Initialize the Chip8 system and load the game into the memory
	myChip8.initialize();
	myChip8.loadGame(argv[1]);

	// Emulation loop
	while(window.isOpen()) {
		sf::Event event;
		while(window.pollEvent(event)) {
			if(event.type == sf::Event::Closed)
				window.close();
		}
		// Emulate one cycle
		myChip8.emulateCycle();

		// If the draw flag is set, update the screen
		if(myChip8.drawFlag)
			drawGraphics(&window);

		// Store key press state (Press and Release)
		// myChip8.setKeys();
	}

	return 0;
}

void drawGraphics(sf::RenderWindow *window_ptr) {
	for(int i = 0; i<2048; i++) {
		// update color of every vertex according to gfx array:
		if(myChip8.gfx[i] != 0)
			screen[i].color = sf::Color::White;
		else
			screen[i].color = sf::Color::Black;
	}
	// Now update the window with vertex array:
	window_ptr->clear();
	window_ptr->draw(screen);
	window_ptr->display();

	// Reset drawFlag:
	myChip8.drawFlag = false;
}
