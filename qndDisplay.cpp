/*
 * qndDisplay.cpp
 *
 *  Created on: 8 Sep 2013
 *      Author: Jon
 *
 *  Test edit - Ben
 */

#include "qndDisplay.hpp"

qndDisplay::qndDisplay(int w, int h, int x, int y) {
	M = nullptr;
	sharedMem = false;
	commonCtor(w,h,x,y);
}
qndDisplay::qndDisplay(int w, int h, int x, int y, BaseMemoryInterface* m) {
	M = m;
	sharedMem = true;
	commonCtor(w,h,x,y);
}
void qndDisplay::commonCtor(int w, int h, int x, int y) {
	screenWidth = w; screenHeight = h; numX = x; numY = y;
	pixelW = screenWidth / numX;
	pixelH = screenHeight / numY;
	memOffset = 0x0200;
	if (x*y > 8142) throw 0;

	//build r/g/b arrays pointing to relevant addresses, for each palette colour
	for(int i=0, j=1, k=0; i <48; i++, j++){
		if(j==1) r[k] = i;
		else if(j==2) g[k] = i;
		else if(j==3) {
			b[k] = i;
			j = 0;
			k++;
		}
	}

}

void qndDisplay::Draw(SDL_Surface *surface) {

	for(int y=0; y < numY; y++) {
		for(int x=0; x < numX; x++) {
			//get this pixel's palette index address
			uint16_t addr = getPixelOffset(x,y);

			//get the palette index for this pixel
			uint8_t paletteColour = sharedMem ? M->Read(addr+memOffset) : vram[addr];
			paletteColour &= 0x0f; //discard the most significant nibble; we only have 16 colour options

			//load the colour for the selected palette index
			uint8_t _r = sharedMem ? M->Read(r[paletteColour]+0x1fcf+memOffset) : vram[r[paletteColour]+0x1fcf];
			uint8_t _g = sharedMem ? M->Read(g[paletteColour]+0x1fcf+memOffset) : vram[g[paletteColour]+0x1fcf];
			uint8_t _b = sharedMem ? M->Read(b[paletteColour]+0x1fcf+memOffset) : vram[b[paletteColour]+0x1fcf];

			//draw a rect with the appropriate colour at the appropriate location
			SDL_Rect rect = {x*pixelW, y*pixelH, pixelW, pixelH };
			SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, _r, _g, _b));
		}
	}
}

uint16_t qndDisplay::getPixelOffset(int x, int y){
	uint16_t offset;

	offset = x + (y*numX);

	return offset;
}

void qndDisplay::SetPalette(uint8_t pal,uint8_t _r, uint8_t _g, uint8_t _b){
	if(sharedMem){
		M->Write(r[pal]+0x1fcf+memOffset,_r);
		M->Write(g[pal]+0x1fcf+memOffset,_g);
		M->Write(b[pal]+0x1fcf+memOffset,_b);
	} else {
		vram[r[pal]+0x1fcf] = _r;
		vram[g[pal]+0x1fcf] = _g;
		vram[b[pal]+0x1fcf] = _b;
	}
}
