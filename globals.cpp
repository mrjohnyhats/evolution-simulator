#include <tuple>
#include <cstdlib>
#include <cmath>
#include <stdio.h>
#include <sstream>
#include <time.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>

#include "Disease.h"
#include "globals.h"

using namespace std;

bool r_seeded = false;
float mouse_food = 10.0;
map<string, int> things_done;

template <typename T>
string to_string(T value){
  ostringstream os;
  os << value;
  return os.str();
}

template string to_string<float>(float value);
template string to_string<int>(int value);

int get_color_dist(vector<int> c1, vector<int> c2){
	int r = abs(c1.at(0) - c2.at(0));
	int g = abs(c1.at(2) - c2.at(1));
	int b = abs(c1.at(1) - c2.at(2));
	return sqrt(r*r+g*g+b*b);
}

template <typename T>
long distance_formula(tuple<T, T> p1, tuple<T, T> p2){
	long dists[4];
	int x = abs(get<0>(p1) - get<0>(p2));
	int ox = SCREEN_WIDTH-x;
	int y = abs(get<1>(p1) - get<1>(p2));
	int oy = SCREEN_HEIGHT-y;
	long dist = 0;
	int k = 0;
	for(int i = 0; i < 2; i++){
		if(i == 0) dist += x*x;
		else dist += ox*ox;
		for(int j = 0; j < 2; j++){
			k++;
			if(j == 0) dist += y*y;
			else dist += oy*oy;
			dists[k] = dist;
			dist = 0;
		}
	}
	return (long)sqrt(*(min_element(dists, dists+4)));
}
template long distance_formula<int>(tuple<int, int> p1, tuple<int, int> p2);
template long distance_formula<float>(tuple<float, float> p1, tuple<float, float> p2);

int gen_rand(){
	if(!r_seeded){
		srand(time(NULL));
		r_seeded = true;
	}
	return rand() % 10000;
}

double float_rand(){
	return (double)gen_rand()/10000;
}

string rand_str(int len){
	string all_chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	int all_char_len = all_chars.length();
	string out = "";
	for(int i = 0; i < len; i++){
		out += all_chars[float_rand()*all_char_len];
	}
	return out;
}

float get_rep_trait(float t1, float t2, float mutprob){
	float unmut_t = (t1+t2)/2;
	if(mutprob > float_rand()){
		return float_rand();
	} else {
		float mut_size = 0.1*mutprob;
		return unmut_t*(1.0-mut_size) + float_rand()*mut_size;
	}
}

vector<int> invert_color(vector<int> c){
	vector<int> newc;
	newc.push_back(255-c[0]);
	newc.push_back(255-c[1]);
	newc.push_back(255-c[2]);
	return newc;
}

Disease make_disease(){
	float startstren = float_rand()/2;
	map<string, float> opts = {
		{"startstren", startstren},
		{"endstren", startstren + float_rand()*(1-startstren)},
		{"streninc", float_rand()/10},
		{"pick", float_rand()},
		{"mobility", float_rand()},
		{"foodeff", float_rand()}
	};
	vector<int> favc;
	for(int i = 0; i < 3; i++){
		favc.push_back(float_rand()*255);
	}
	return Disease(&opts, favc);
}

Disease make_disease(float strength){
	if(strength > 1){
		strength = 1;
	}
	float basestren = strength + float_rand()*(1-strength)/2;
	 map<string, float> opts = {
		{"startstren", basestren/2},
		{"endstren", basestren/2 + float_rand()*(1-basestren/2)},
		{"streninc", basestren/10},
		{"pick", float_rand()},
		{"mobility", float_rand()},
		{"foodeff", float_rand()}
	};
	vector<int> favc;
	for(int i = 0; i < 3; i++){
		favc.push_back(float_rand()*255);
	}
	return Disease(&opts, favc);
}
