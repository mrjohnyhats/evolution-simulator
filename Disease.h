#ifndef DISEASE_DEFINED
#define DISEASE_DEFINED
#include <map>
#include <vector>

using namespace std;

class Disease {
	vector<int> fav_color;
	float start_strength;
	float end_strength;
	float strength_inc;
	float pickiness;
	float mobility;
	float strength;
	float food_effect;
public:
	void change_strength(float resistance);
	float get_strength();
	float get_mobility();
	float get_sanitydiff(float sanity, vector<int> color, int t);
	float get_food_effect();
	Disease(map<string, float>* opts, vector<int> favc);
	Disease();
};
#endif