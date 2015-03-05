#include <iostream>
#include <cstdint>

#include "mos6502.hpp"
#include "BaseMemoryInterface.hpp"
#include "Log.hpp"
#include "qndDisplay.hpp"
#include "SDL2/SDL.h"

int main(int argc, char** argv) {

	if (argc == 3)
		Log::SetOutput(argv[2]); //use a file if one's specified (gets big quickly at the moment!)
	else
		Log::SetOutput(&std::cout); //else use stdout

	//attempt to init SDL before bothering to go further
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		*Log::Output << SDL_GetError() << std::endl;
		return 1;
	}

	//Memory Read/Write testing
	BaseMemoryInterface* mem = new BaseMemoryInterface();

	//dump a binary file into ROM address range
	mem->FileDump(argv[1], 0x8000);

	//set Reset vector to point to ROM start
	mem->Write(0xfffc, 0x0);
	mem->Write(0xfffd, 0x80);

	//reset 32x32 mapped video memory to palette 0, if a little inelegantly
	for(int i=0x0200; i < 0x21cf; i++)
		mem->Write(i,0);

	//initial CPU testing
	mos6502* cpu = new mos6502(mem); //get the cpu, hook up the memory

	//let's try and create a window!
	SDL_Window *window = nullptr;
	window = SDL_CreateWindow("illNES", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 300, SDL_WINDOW_SHOWN);
	if (window == nullptr){
		*Log::Output << SDL_GetError() << std::endl;
		return 1;
	}

	//let's try and create an SDL Renderer for our window
	SDL_Renderer *renderer = nullptr;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr){
		*Log::Output << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Event event; //so we can poll :O

	//Our display array
	qndDisplay* disp = new qndDisplay(300,300,32,32,mem);

	disp->SetPalette(0,0,0,0);
	disp->SetPalette(1,255,255,255);
	disp->SetPalette(2,0,255,255);
	disp->SetPalette(3,255,0,255);
	disp->SetPalette(4,255,255,0);
	disp->SetPalette(5,0,0,255);
	disp->SetPalette(6,255,0,0);
	disp->SetPalette(7,128,128,128);
	disp->SetPalette(8,33,66,99);
	disp->SetPalette(9,66,99,33);
	disp->SetPalette(10,99,33,66);
	disp->SetPalette(11,0,128,255);
	disp->SetPalette(12,128,255,0);
	disp->SetPalette(13,255,0,128);
	disp->SetPalette(14,50,150,225);
	disp->SetPalette(15,225,150,50);

	while(1) {
		//poll for events and handle the ones we care about
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				*Log::Output << "SDL EVENT: User Quit" << std::endl;
				return 0;
			}
		}

		//update
		cpu->Tick();

		//draw
		SDL_Surface* surface = SDL_CreateRGBSurface(0, 300, 300, 32, 0xff, 0xff00, 0xff0000, 0xff000000);
		disp->Draw(surface);
		SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, tex, NULL, NULL);
		SDL_RenderPresent(renderer);

		SDL_DestroyTexture(tex);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	Log::Terminate();
	return 0;
}
