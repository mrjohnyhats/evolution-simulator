#ifndef GLOBALS_DEFINED
#define GLOBALS_DEFINED
#include <vector>
#include <tuple>
#include <string>
#include <cmath>

#include "Disease.h"

using namespace std;
template <typename T>
string to_string(T value);
int get_color_dist(vector<int> c1, vector<int> c2);
template <typename T>
long distance_formula(tuple<T, T> p1, tuple<T, T> p2);
int gen_rand();
double float_rand();
string rand_str(int len);
float get_rep_trait(float t1, float t2, float mutprob);
vector<int> invert_color(vector<int> c);
Disease make_disease();
Disease make_disease(float strength);

const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;
const int DIAG_DIST = sqrt(SCREEN_WIDTH*SCREEN_WIDTH+SCREEN_HEIGHT*SCREEN_HEIGHT);
const int FRAMECAP = 60;
const double FRAMETIME = 1000/(double)(FRAMECAP);
const int REP_CHOICE_START_POP = 6;
const int MAX_COLOR_DIST = sqrt(255*255*3);
const unsigned long long MIN_CLOSTC_FOR_DISEASE = 1e11;
const float MAX_MOUSE_FOOD = 1.0;
extern float mouse_food;
extern map<string, int> things_done;
#endif