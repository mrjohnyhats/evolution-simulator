#include <vector>
#include <map>
#include <stdio.h>

#include "Disease.h"
#include "globals.h"

using namespace std;

Disease::Disease(map<string, float>* opts, vector<int> favc){
	start_strength = opts->at("startstren");
	end_strength = opts->at("endstren");
	strength_inc = opts->at("streninc");
	mobility = opts->at("mobility");
	pickiness = opts->at("pick");
	food_effect = opts->at("foodeff");
	fav_color = favc;
	strength = start_strength;
}
Disease::Disease(){}
void Disease::change_strength(float resistance){
	if(strength < end_strength){
		strength+=(strength_inc*(1.0-resistance));
		if(strength < 0) strength = 0;
	}
}
float Disease::get_sanitydiff(float sanity, vector<int> color, int t){
	float color_dist_factor = get_color_dist(color, fav_color)/(float)MAX_COLOR_DIST;
	float color_effect = color_dist_factor*pickiness;
	float diff = (t*strength*(1.0-color_dist_factor) - color_effect)/10;
	if(diff < 0) diff = 0;	
	return diff;
}
float Disease::get_strength(){
	return strength;
}
float Disease::get_mobility(){
	return mobility;
}
float Disease::get_food_effect(){
	return food_effect;
}