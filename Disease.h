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
public:
	void change_strength(float resistance);
	float get_strength();
	float get_mobility();
	float change_sanity(float sanity, vector<int> color);
	Disease(map<string, float>* opts, vector<int> favc);
	Disease();
};
#endif