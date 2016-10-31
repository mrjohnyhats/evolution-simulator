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

const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;
const int DIAG_DIST = SCREEN_WIDTH*SCREEN_WIDTH+SCREEN_HEIGHT*SCREEN_HEIGHT;
const int FRAMECAP = 60;
const double FRAMETIME = 1000/(double)(FRAMECAP);
const int REP_CHOICE_START_POP = 6;
const int MAX_COLOR_DIST = sqrt(255*255*3);
#endif