# Chip-8 Emulator

An emulator for the [Chip-8](https://en.wikipedia.org/wiki/CHIP-8) virtual machine proposed by Joseph Weisbecker and first used in the mid-1970s.

I am using this [tutorial](http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/).

Usage: `./chip8-emu <chip8 application file>`

Example: `./chip8-emu games/invaders.c8`

For input, the Chip-8's keypad is mapped like this:

```
Chip-8 Keypad				 Keyboard
	+-+-+-+-+                +-+-+-+-+
	|1|2|3|C|                |1|2|3|4|
	+-+-+-+-+                +-+-+-+-+
	|4|5|6|D|                |Q|W|E|R|
	+-+-+-+-+       =>       +-+-+-+-+
	|7|8|9|E|                |A|S|D|F|
	+-+-+-+-+                +-+-+-+-+
	|A|0|B|F|                |Z|X|C|V|
	+-+-+-+-+                +-+-+-+-+
```
