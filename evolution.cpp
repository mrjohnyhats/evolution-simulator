#include <SDL2/SDL.h>
#ifdef __OSX__
#include <SDL2_ttf/SDL_ttf.h>
#else
#include <SDL2/SDL_ttf.h>
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
// extern float mouse_food;

using namespace std;

//TODO: fix line drawing algorithm

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

void k_dead_ents(){
	vector<int> dead_indices;

	Entity* ent;

	for(int i = 0; i < ents->size(); i++){
		ent = ents->at(i);
		if(ent->is_dead()){
			dead_indices.push_back(i);
			ents->erase(ents->begin() + i);
		}
	}

	for(int i = 0; i < ents->size(); i++){
		ent = ents->at(i);
		ent->update_indices(dead_indices);
	}
}

void a_on_alist(){
	while(alist->size() > 0){
		ents->push_back(alist->at(0));
		alist->erase(alist->begin());
	}
}

vector<Entity*>* make_ents(){
	vector<Entity*>* ents = new vector<Entity*>;
	for(int i = 0; i < 2; i++){
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

void reset_things_done(){
	things_done["getting_food"] = 0;
	things_done["moving_to_r"] = 0;
	things_done["reproducing"] = 0;
	things_done["chose_mate"] = 0;
	things_done["no_mate_found"] = 0;
	things_done["abandoned_r"] = 0;
	things_done["moving_to_t"] = 0;
	things_done["fighting"] = 0;
	things_done["chose_t"] = 0;
	things_done["no_t_found"] = 0;
	things_done["abandoned_t"] = 0;
	things_done["food_dead"] = 0;
	things_done["sanity_dead"] = 0;
	things_done["moving"] = 0;
	things_done["new_randmove"] = 0;
	things_done["clost"] = 0;
}

int main( int argc, char* args[] ){
	if( !init() ){
		cout << "Failed to initialize!\n";
		close();
	}
	else{
		ents = make_ents();
		reset_things_done();
		bool quit = false;

		SDL_Event e;

		double t = 0;
		chrono::high_resolution_clock::time_point lastt;
		int tick = 0;

		while(!quit) {
			lastt = chrono::high_resolution_clock::now();
				pcounter->update(ents->size());
			Entity* ent;
			int i = 0;
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			while(i < ents->size()){
				ent = ents->at(i);
				ent->update_ents_ptr(ents);
				ent->update_time(t);
				ent->behave(alist);
				ent->inc_age();
				ent->die_if_old();
				ent->draw(renderer);
				if(mouse_food < 10){
					mouse_food += 0.01;
				}
				i++;
			}
			pcounter->draw(renderer);
			tick++;
			// for(auto a = things_done.begin(); a != things_done.end(); a++){
			// 	cout << a->first << ": " << a->second << endl;
			// }
			// cout << "\n";
			reset_things_done();
			k_dead_ents();
			a_on_alist(); 
			// cout << "mouse_food " << mouse_food << endl;
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
			//SDL_Delay(FRAMETIME - t);
		}
	}


	delete ents;
	delete klist;
	delete alist;
	delete pcounter;	
	close();

	return 0;
}
