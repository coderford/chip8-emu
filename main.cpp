// #include graphics and input libraries
#include "chip8.h"	// cpu core implementation
#include <SFML/Graphics.hpp>
#define SCALE 10	// scaling factor

chip8 myChip8;

void drawGraphics(sf::RenderWindow *);
sf::VertexArray screen(sf::Quads, 64*32*4); 
int main(int argc, char **argv) {
	if(argc < 2) {
		printf("Usage: chip8-emu <chip-8 application>\n\n");
		return 1;
	}
	// Create window and set framerate
	int win_w = 64*SCALE;		// Window width
	int win_h = 32*SCALE;		// Window height
	sf::RenderWindow window(sf::VideoMode(win_w, win_h), "Chip-8 Emulator");
	window.setFramerateLimit(60);

	// Setup up screen VertexArray
	for(int i = 0; i<64*32; i++) {
		int base_x = (i%64)*SCALE;	// top-left point on quad
		int base_y = (i/64)*SCALE;	//

		// set positions for points on current quad (clockwise)
		int q = i*4;	// index of the quad's first point in VertexArray
		screen[q+0].position = sf::Vector2f(base_x, base_y);
		screen[q+1].position = sf::Vector2f(base_x+SCALE, base_y);
		screen[q+2].position = sf::Vector2f(base_x+SCALE, base_y+SCALE);
		screen[q+3].position = sf::Vector2f(base_x, base_y+SCALE);
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
	for(int i = 0; i<64*32; i++) {
		// Update color of every quad in screen according to gfx array
		if(myChip8.gfx[i] != 0) {
			int q = i*4;
			for(int j = 0; j<4; j++)
				screen[q+j].color = sf::Color::White;
		}
		else {
			int q = i*4;
			for(int j = 0; j<4; j++)
				screen[q+j].color = sf::Color::Black;
		}
	}
	// Now update the window with vertex array:
	window_ptr->clear();
	window_ptr->draw(screen);
	window_ptr->display();

	// Reset drawFlag:
	myChip8.drawFlag = false;
}
