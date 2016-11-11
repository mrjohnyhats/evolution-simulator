#include <map>
#include <iostream>
#include <tuple>
#include <math.h>
#include <SDL2/SDL.h>

#include "globals.h"
#include "Terrain.h"
using namespace std;

double Terrain::next_elvt_at_p(double x, double y){
	int progx = fmod(x/10.0, 1.0);
	int progy = fmod(y/10.0, 1.0);
	double prog = sqrt(pow(progx, 2) + pow(progy, 2));
	int xi = abs((int)(x/10) % SCREEN_WIDTH/10);
	int yi = abs((int)(y/10) % SCREEN_HEIGHT/10);
	//get dist between px and py
	tuple<int, int> p = tmap[xi][yi];
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
	int m_to_make = 5 + (int)round(float_rand()*10);
	int radius, msect, x, y;
	float height;
	bool broken = false;

	while(m_to_make > 0){
		radius = 20 + (int)round(float_rand()*20);
		height = 0.25 + float_rand()*0.75;
		x = float_rand()*SCREEN_WIDTH/10-1;
		y = float_rand()*SCREEN_HEIGHT/10-1;
		make_mountain(radius, height, x, y);
		m_to_make--;
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
	for(int rs = 0; rs <= radius; rs++){
		max_qs = rs*M_PI*2;
		for(int qs = 1; qs <= max_qs; qs++){
			if(rs > 0){
				rad = 2.0*M_PI*((1.0/(double)max_qs)*(double)qs);

				cx = x + (int)round(sin(rad)*rs);
				cy = y + (int)round(cos(rad)*rs);
				lh = (double)height/(log2(rs));
				hh = (double)height*(1.0+float_rand())/(log2(rs));
			} else {
				cx = x;
				cy = y;
				lh = height;
				hh = height;
			}

			if(rs == radius){
				cx += (int)round(float_rand()*2.0-1.0);
				cy += (int)round(float_rand()*2.0-1.0);
			}

			if(cx < 0) cx = SCREEN_WIDTH/10 + cx;
			if(cy < 0) cy = SCREEN_HEIGHT/10 + cy;

			cx %= SCREEN_WIDTH/10;
			cy %= SCREEN_WIDTH/10;

			if(tmap[cx][cy] == make_tuple((double)0.0, (double)0.0)){
				tmap[cx][cy] = make_tuple(lh, hh);
			} else {
				tuple<double, double> op = tmap[cx][cy];
				tmap[cx][cy] = make_tuple((get<0>(op) + lh)/2, (get<1>(op) + hh)/2);
			}
		}
	}

}

Terrain::Terrain(){
	return;
}