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
	fav_color = favc;
	strength = start_strength;
}
Disease::Disease(){}
void Disease::change_strength(float resistance){
	if(strength < end_strength){
		strength+=(strength_inc*resistance);
		if(strength < 0) strength = 0;
	}
}
float Disease::change_sanity(float sanity, vector<int> color){
	float color_dist_factor = get_color_dist(color, fav_color)/(float)MAX_COLOR_DIST;
	float color_effect = color_dist_factor*pickiness;
	float modifier = sanity*strength*(1.0-color_dist_factor)*5.0 - color_effect;
	if(modifier < 0) modifier = 0;
	float retval = sanity - modifier;
	if(retval < 0) retval = 0;
	return retval;
}
float Disease::get_strength(){
	return strength;
}
float Disease::get_mobility(){
	return mobility;
}