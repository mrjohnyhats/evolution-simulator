#include <SDL2/SDL.h>
#ifdef __WIN32__
#include <SDL2/SDL_ttf.h>
#else
#include <SDl2_ttf/SDL_ttf.h>
#endif
#include <stdio.h>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <cstdlib>
#include <utility>
#include <random>
#include <time.h>
#include <functional>
#include <map>
#include <algorithm>
#include <chrono>

#include "globals.h"
#include "Entity.h"
#include "Disease.h"
#include "Pop_counter.h"

using namespace std;

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
SDL_Renderer* renderer = NULL;


bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		if(TTF_Init() < 0){
			printf("SDL_TTF could not initialize! %s\n", SDL_GetError());
		} else {
			//Set texture filtering to linear
			if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
			{
				printf( "Warning: Linear texture filtering not enabled!" );
			}

			//Create window
			gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
			if( gWindow == NULL )
			{
				printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Create renderer for window
				renderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
				if( renderer == NULL )
				{
					printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
					success = false;
				}
				else
				{
					//Initialize renderer color
					SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
				}
			}
		}
	}

	return success;
}

void close(){
	//Destroy window
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}


vector<Entity*>* ents = new vector<Entity*>;
vector<int>* klist = new vector<int>;
vector<Entity*>* alist = new vector<Entity*>;
Pop_counter* pcounter = new Pop_counter(0);

void k_on_klist(){
	vector<Entity*>* nents = new vector<Entity*>;
	vector<int>::iterator klisti;
	int idist = 0;
	for(auto i = ents->begin(); i != ents->end(); i++, idist++){
		klisti = find(klist->begin(), klist->end(), idist);
		if(klisti == klist->end()){
			nents->push_back(*i);
		} else {
			klist->erase(klisti);
		}
	}
	ents = nents;
}

void a_on_alist(){
	while(alist->size() > 0){
		ents->push_back(alist->at(0));
		alist->erase(alist->begin());
	}
}

vector<Entity*>* make_ents(){
	vector<Entity*>* ents = new vector<Entity*>;
	for(int i = 0; i < REP_CHOICE_START_POP; i++){
		map<string, float> opts {
			{"pos", float_rand()},
			{"pick", float_rand()},
			{"hostl", float_rand()},
			{"psysense", float_rand()*0.95 + 0.05},
			{"atol", float_rand()},
			{"mutprob", float_rand()},
			{"fert", float_rand()},
			{"stren", float_rand()*2},
			{"x", float_rand()*SCREEN_WIDTH},
			{"y", float_rand()*SCREEN_HEIGHT}
		};
		vector<int> color {(int)(float_rand()*255), (int)(float_rand()*255), (int)(float_rand()*255)};
		vector<int> favc = {(int)(float_rand()*255), (int)(float_rand()*255), (int)(float_rand()*255)};

		ents->push_back(new Entity(color, favc, &opts, ents, ents->size(), 0));
		if(float_rand() < 0.1){
			(*(ents->end()-1))->add_disease(make_disease());
		}
	}
	return ents;
}

int main( int argc, char* args[] ){
	if( !init() ){
		cout << "Failed to initialize!\n";
		close();
	}
	else{
		ents = make_ents();
		bool quit = false;

		SDL_Event e;

		double t = 0;
		chrono::high_resolution_clock::time_point lastt;
		int tick = 0;
		int t_since_popc_update = 0;

		while(!quit) {
			lastt = chrono::high_resolution_clock::now();
			if(t_since_popc_update >= 10){
				t_since_popc_update %= 10;
				pcounter->update(ents->size());
			}
			Entity* ent;
			int i = 0;
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			while(i < ents->size()){
				ent = ents->at(i);
				ent->update_ents_ptr(ents);
				ent->update_time(t);
				ent->update_indices(i);
				ent->behave(klist, alist);
				ent->inc_age();
				ent->die_if_old(klist);
				ent->draw(renderer);
				i++;
			}
			pcounter->draw(renderer);
			tick++;
			a_on_alist();
			k_on_klist();
			// for(int c = 0; c < 6; c++){
			// 	cout << c << " " << MPC[c] << endl;
			// }
			//cout << "population " << ents->size() << endl;
			// cout << "tick " << tick << endl;
			if(ents->size() < 2){
				cout << "civilization has failed\n";
				quit = true;
			}

			SDL_RenderPresent(renderer);

			while( SDL_PollEvent(&e) != 0 ){
				if( e.type == SDL_QUIT ){
					quit = true;
				}
			}


			t = 1e-7*chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now()-lastt).count();
			t_since_popc_update+=t;
			//SDL_Delay(FRAMETIME - t);
		}
	}

	//Free resources and close SDL
	delete ents;
	delete klist;
	delete alist;
	delete pcounter;
	close();

	return 0;
}
