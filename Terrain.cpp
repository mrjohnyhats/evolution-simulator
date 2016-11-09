#include <map>
#include <iostream>
#include <tuple>
#include <math.h>

#include "globals.h"
#include "Terrain.h"
using namespace std;

float Terrain::elvt_at_p(int x, int y){
	int px = fmod(x/10.0, 1.0);
	int py = fmod(y/10.0, 1.0);
	//get dist between px and py
	tuple<int, int> p = tmap[(int)x/10][(int)y/10];
	float diff = get<1>(p) - get<0>(p);
	return get<0>(p) + diff;
}

void Terrain::gen_rand(){
	int m_made = 0;
	int m_to_make = (int)round(float_rand()*6);
	int m_area = 0;
	int radius, height;
	for(int x = 0; x < SCREEN_WIDTH/10; x++){
		for(int y = 0; y < SCREEN_HEIGHT/10; y++){		
			if(float_rand() < 1.0/(float)(SCREEN_WIDTH/10-m_area)/(float)(SCREEN_HEIGHT/10-m_area)*m_to_make*3){
				radius = float_rand()*6;
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
	int quad_sects = sqrt(pow(radius, 2)*2)/sqrt(2);
	float rad;
	float lh, hh;
	for(int rs = 0; rs <= radius; rs++){
		for(int q = 0; q < 4; q++){
			for(int qs = 0; qs < quad_sects; qs++){
				rad = (quadrant_rad*q) + quadrant_rad/quad_sects*qs;
				x += ceil(sin(rad) + (float_rand()*2 - 1));
				y += ceil(cos(rad) + (float_rand()*2 - 1));
				x %= (SCREEN_WIDTH/10);
				y %= (SCREEN_HEIGHT/10);
				cout << x << " " << y << endl;
				lh = (float)height/(float)rs;
				hh = (float)height*(1.0+float_rand())/(float)rs;
				if(lh == 0) lh = 1;
				if(hh == 0) hh = 1;
				cout << "lh " << lh << " hh " << hh << endl;
				tmap[x][y] = make_tuple(lh, hh);
			}
		}
	}

}

Terrain::Terrain(){
	return;
}