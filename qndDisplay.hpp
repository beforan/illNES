/*
 * qndDisplay.h
 *
 *  Created on: 8 Sep 2013
 *      Author: Jon
 */

#ifndef QNDDISPLAY_H_
#define QNDDISPLAY_H_

#include <cstdint>

#include "SDL2/SDL.h"

#include "BaseMemoryInterface.hpp"
#include "Log.hpp"

/*
 * This is a very Quick n' Dirty Display "chip" for the 6502
 * it can share the 6502's main memory (via a BaseMemoryInterface*)
 * so no mapping is necessary, or you can map to its own Video RAM
 *
 * In shared memory:
 * it treats 0x2000 - 0x03FCE as pixel addresses, so you can have a max of 8142 pixels
 * these can be arranged how you wish, and don't all have to be used
 *
 * it uses 0x3FCF - 0x3FFF as a 16 colour 8-bit per channel RGB palette
 * each 3 addresses from 0x3FCF is r,g,b for each palette colour.
 * therefore by poking the relevant address with values in assembly one
 * can change the colour of the pixels using a given palette colour
 *
 * Using its own Video RAM, the address assignments are the same, less the 0x2000 offset
 */

class qndDisplay {
public:
	qndDisplay(int w, int h, int x, int y);
	qndDisplay(int w, int h, int x, int y, BaseMemoryInterface* m);
	~qndDisplay();
	void Draw(SDL_Surface* surface);
	void SetPalette(uint8_t pal,uint8_t r, uint8_t g, uint8_t b);
protected:
	int screenWidth;  //width of the native display (e.g. OpenGL window size)
	int screenHeight; //height as above
	int numX; //width of 6502 display output
	int numY; //height of 6502 display output  numX* numY can't exceed 8142
	int pixelW; //"pixel" (i.e. rect) size based on the target
	int pixelH; //window size and the display output dimensions

	uint16_t getPixelOffset(int x, int y);

	//palette address arrays
	uint16_t r[16];
	uint16_t g[16];
	uint16_t b[16];

	void commonCtor(int w, int h, int x, int y);
	bool sharedMem;
	uint16_t memOffset;
	uint8_t vram[0x7f];

	BaseMemoryInterface* M;
};

#endif /* QNDDISPLAY_H_ */
