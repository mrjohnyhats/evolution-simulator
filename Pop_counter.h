#ifndef PCDEFINED
#define PCDEFINED
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <iostream>

using namespace std;

class Pop_counter{
	int pop = 2;
	TTF_Font* font;
	SDL_Color color = {255, 255, 255};
	SDL_Surface* s;
	SDL_Texture* t;
	SDL_Rect* r = new SDL_Rect;
public:
	void draw(SDL_Renderer* ren);
	void update(int npop);
	~Pop_counter();
	Pop_counter(int startp);
};
#endif