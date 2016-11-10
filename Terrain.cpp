#include <map>
#include <iostream>
#include <tuple>
#include <math.h>
#include <SDL2/SDL.h>

#include "globals.h"
#include "Terrain.h"
using namespace std;

double Terrain::next_elvt_at_p(int x, int y){
	int progx = fmod(x/10.0, 1.0);
	int progy = fmod(y/10.0, 1.0);
	double prog = sqrt(pow(progx, 2) + pow(progy, 2));
	//get dist between px and py
	tuple<int, int> p = tmap[(int)x/10][(int)y/10];
	double diff = get<1>(p) - get<0>(p);
	return get<0>(p) + diff*prog;
}

void Terrain::draw(SDL_Renderer* ren){
	double avge;
	SDL_Rect rect;
	rect.w = 10;
	rect.h = 10;
	for(int x = 0; x < SCREEN_WIDTH/10; x++){
		for(int y = 0; y < SCREEN_HEIGHT/10; y++){
			rect.x = (x*10 + 10 < SCREEN_WIDTH) ? x*10 : x*10 + (x*10 - SCREEN_WIDTH);
			rect.y = (y*10 + 10 < SCREEN_HEIGHT) ? y*10 : y*10 + (y*10 - SCREEN_WIDTH);
			avge = (get<0>(tmap[x][y]) + get<1>(tmap[x][y]))/2;

			SDL_SetRenderDrawColor(ren,255*avge,255*avge,255*avge,255);
			if(SDL_RenderFillRect(ren, &rect) != 0){
				cout << "error drawing filled rect at terrain " << SDL_GetError() << endl;
			}
		}
	}
}

void Terrain::gen_rand(){
	int m_made = 0;
	int m_to_make = 2 + (int)round(float_rand()*6);
	int m_area = 0;
	int radius;
	float height;
	for(int x = 0; x < SCREEN_WIDTH/10; x++){
		for(int y = 0; y < SCREEN_HEIGHT/10; y++){		
			if(float_rand() < 1.0/(float)(SCREEN_WIDTH/10-m_area)/(float)(SCREEN_HEIGHT/10-m_area)*m_to_make*3){
				radius = 4 + (int)ceil(float_rand()*6);
				height = 0.25 + float_rand()*0.75;
				make_mountain(radius, height, x, y);
				m_area += 2*radius;
			}
		}
	}
}

void Terrain::log(){
	cout << "TERRAIN MAP" << endl;
	for(int x = 0; x < SCREEN_WIDTH/10; x++){
		for(int y = 0; y < SCREEN_HEIGHT/10; y++){
			cout << get<0>(tmap[x][y]) << " " << get<1>(tmap[x][y]) << ", ";
		}
		cout << endl;
	}
}

void Terrain::make_mountain(int radius, float height, int x, int y){
	int angle_splits = radius % 2 == 0 ? radius : radius + 1;
	double quadrant_rad = M_PI/2;
	double rad;
	double lh, hh;
	int max_qs;
	int cx, cy;
	cout << "MAKING MOUNTAIN" << endl;
	cout << "radius " << radius << endl;
	cout << "x " << x << " y " << y << endl;
	for(int rs = 0; rs <= radius; rs++){
		max_qs = rs*M_PI*2;
		for(int qs = 1; qs <= max_qs; qs++){
			rad = 2.0*M_PI*((1.0/(double)max_qs)*(double)qs);

			cout << sin(rad) << " " << cos(rad) << endl;

			cx = abs(x + (int)round(sin(rad)*(double)rs)) % SCREEN_WIDTH/10;
			cy = abs(y + (int)round(cos(rad)*(double)rs)) % SCREEN_HEIGHT/10;

			// if(rs == radius){
			// 	x += float_rand()*2 - 1;
			// 	y += float_rand()*2 - 1;
			// }

			cout << cx << " " << cy << endl;

			if(rs > 0){
				// lh = (double)height/(double)rs;
				// hh = (double)height*(1.0+float_rand())/(double)rs;
				lh = 1;
				hh = 1;
			} else {
				lh = 1;
				hh = 1;
			}
			tmap[cx][cy] = make_tuple(lh, hh);
		}
	}

}

Terrain::Terrain(){
	return;
}