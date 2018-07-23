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

	// Initialize the Chip8 system and load the game into the memory
	myChip8.initialize();
	myChip8.loadGame(argv[1]);

	// Emulation loop
	while(window.isOpen()) {
		sf::Event event;
		while(window.pollEvent(event)) {

			if(event.type == sf::Event::Closed)
				window.close();

			// Store key press state (Press and Release)
			if(event.type == sf::Event::KeyPressed) {
				switch(event.key.code) {
					case sf::Keyboard::X:	 myChip8.key[0x0] = 1; break;
					case sf::Keyboard::Num1: myChip8.key[0x1] = 1; break;
					case sf::Keyboard::Num2: myChip8.key[0x2] = 1; break;
					case sf::Keyboard::Num3: myChip8.key[0x3] = 1; break;
					case sf::Keyboard::Q:	 myChip8.key[0x4] = 1; break;
					case sf::Keyboard::W:	 myChip8.key[0x5] = 1; break;
					case sf::Keyboard::E:	 myChip8.key[0x6] = 1; break;
					case sf::Keyboard::A:	 myChip8.key[0x7] = 1; break;
					case sf::Keyboard::S:	 myChip8.key[0x8] = 1; break;
					case sf::Keyboard::D:	 myChip8.key[0x9] = 1; break;
					case sf::Keyboard::Z:	 myChip8.key[0xA] = 1; break;
					case sf::Keyboard::C:	 myChip8.key[0xB] = 1; break;
					case sf::Keyboard::Num4: myChip8.key[0xC] = 1; break;
					case sf::Keyboard::R:	 myChip8.key[0xD] = 1; break;
					case sf::Keyboard::F:	 myChip8.key[0xE] = 1; break;
					case sf::Keyboard::V:	 myChip8.key[0xF] = 1; break;
				}
			}
			if(event.type == sf::Event::KeyReleased) {
				switch(event.key.code) {
					case sf::Keyboard::X:	 myChip8.key[0x0] = 0; break;
					case sf::Keyboard::Num1: myChip8.key[0x1] = 0; break;
					case sf::Keyboard::Num2: myChip8.key[0x2] = 0; break;
					case sf::Keyboard::Num3: myChip8.key[0x3] = 0; break;
					case sf::Keyboard::Q:	 myChip8.key[0x4] = 0; break;
					case sf::Keyboard::W:	 myChip8.key[0x5] = 0; break;
					case sf::Keyboard::E:	 myChip8.key[0x6] = 0; break;
					case sf::Keyboard::A:	 myChip8.key[0x7] = 0; break;
					case sf::Keyboard::S:	 myChip8.key[0x8] = 0; break;
					case sf::Keyboard::D:	 myChip8.key[0x9] = 0; break;
					case sf::Keyboard::Z:	 myChip8.key[0xA] = 0; break;
					case sf::Keyboard::C:	 myChip8.key[0xB] = 0; break;
					case sf::Keyboard::Num4: myChip8.key[0xC] = 0; break;
					case sf::Keyboard::R:	 myChip8.key[0xD] = 0; break;
					case sf::Keyboard::F:	 myChip8.key[0xE] = 0; break;
					case sf::Keyboard::V:	 myChip8.key[0xF] = 0; break;
				}
			}
		}
		// Emulate one cycle
		myChip8.emulateCycle();

		// If the draw flag is set, update the screen
		if(myChip8.drawFlag)
			drawGraphics(&window);
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
