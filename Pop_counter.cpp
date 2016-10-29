#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

#include "Pop_counter.h"
#include "globals.h"

using namespace std;

void Pop_counter::draw(SDL_Renderer* ren){
	s = TTF_RenderText_Solid(font, to_string(pop).c_str(), color);
	r->w = s->w;
	r->h = s->h;
	t = SDL_CreateTextureFromSurface(ren, s);
	SDL_RenderCopy(ren, t, NULL, r);
}
void Pop_counter::update(int npop){
	pop = npop;
}
Pop_counter::~Pop_counter(){
	SDL_FreeSurface(s);
	SDL_DestroyTexture(t);
	TTF_CloseFont(font);
	delete r;
}
Pop_counter::Pop_counter(int startp){
	if(!TTF_WasInit() && TTF_Init() == -1){
		printf("error initializing ttf library: %s\n", TTF_GetError());
	} else {
		font = TTF_OpenFont("font.ttf", 40);
		if(!font){
			printf("error initializing font: %s\n", SDL_GetError());
		}
		pop = startp;
		r->x = SCREEN_WIDTH/80;
		r->y = SCREEN_HEIGHT/80;
	}
}
