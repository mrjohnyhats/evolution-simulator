#include <map>
#include <iostream>
#include <tuple>
#include <math.h>

#include "globals.h"
using namepspace std;

float Terrain::elvt_at_p(int x, int y){
	int px = fmod(x/10.0, 1.0);
	int py = fmod(y/10.0, 1.0);
	//get dist between px and py
	tuple<int, int> p = tmap[x/10][(int)fy];
	float diff = get<1>(p) - get<0>(p);
	return get<0>(p) + diff;
}

void Terrain::gen_rand(){
	int m_made = 0;
	int m_to_make = (int)round(float_rand()*6);
	int m_area = -;
	int rad, height;
	for(int x = 0; x < SCREEN_WIDTH/10; x++){
		for(int y = 0; y < SCREEN_HEIGHT/10; y++){		
			if(float_rand() < 1.0/(float)(SCREEN_WIDTH/10-m_area)/(float)(SCREEN_HEIGHT/10-m_area)*m_to_make*3){
				rad = float_rand()*6;
				height = 0.25 + float_rand()*0.75;
				make_mountain(rad, height, x, y);
			}
		}
	}
}

void Terrain::make_mountain(int radius, float height, int x, int y){
	angle_splits = radius % 2 == 0 ? radius : radius + 1;
	quadrant_rad = M_PI/2;
	float ax, ay;
	for(ai = 0; ai < angle_splits/2; ai++){
		ax = sin(quadrant_rad/pow(2, ai+1));
		ay = cos(quadrant_rad/pow(2, ai+1));
		tmap[ceil(ax) % SCREEN_WIDTH][ceil(ay) % SCREEN_HEIGHT] = make_tuple(height/radius, height*(1+float_rand())/radius);
	}
}

Terrain::Terrain(){
	return;
}