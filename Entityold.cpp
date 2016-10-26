#include <vector>
#include <tuple>
#include <utility>
#include <map>

#include "globals.h"
#include "Entity.h"

using namespace std;

//color attraction
//contrast attraction
//direction
//hostility (mutate traits of other entities)
//gene entropy
//tolerance for dislikes
//viewing distance
//task persistence
//energy level (sleep interval)


class Entity{
	vector<int> color;
	vector<int> fav_color;
	vector<int> lfav_color;
	float positivity;
	float pickiness;
	float hostility;
	float agro_color_tol;
	float fav_color_tol;
	float sanity = 1.0;
	float psych_sensitivity;
	float x;
	float y;
	float age = 0;
	float agetol;
	tuple<int, int> newc;
	bool moving = false;
	float move_speed = 0;
	bool reproducing = false;
	int matei;
	int targeti;
	bool is_mate = false;
	string id;
	SDL_Rect* choord_r = new SDL_Rect;
	int self_entsi;
	int targetc = 0;
	int repc = 0;
	float mutation_prob;

public:
	void update_indices(int i){
		int diff = i - self_entsi;
		if(targeti >= self_entsi) targeti += diff;
		if(matei >= self_entsi) matei += diff;
		self_entsi += diff;
	}
	void defect(int t){
		int change_type;
		for(int i = 0; i < 3; i++){
			change_type = float_rand() > 0.5;
			if(change_type) change_type = -1;
			color[i] = color.at(i) += change_type*sanity*(255/4)*t/5;
			color[i] %= 256;
		}
	}
	void move(tuple<int, int> inewc, float speed, int t){
		newc = inewc;
		move_speed = speed;

		// cout << "speed " << speed << endl;

		int change_type = float_rand() > 0.5;
		if(change_type == 0) change_type = -1;

		int newx = abs(get<0>(newc) - x) < fmod(get<0>(newc) + x, SCREEN_WIDTH) ? get<0>(newc) - x : fmod(get<0>(newc) + x, SCREEN_WIDTH);  
		int newy = abs(get<1>(newc) - y) < fmod(get<1>(newc) + y, SCREEN_HEIGHT) ? get<1>(newc) - y : fmod(get<1>(newc) + y, SCREEN_HEIGHT);

		moving = false;

		double xinc = newx / (800.0/speed + (1.0 - sanity)*(4.0*speed + float_rand()*(double)100)*change_type)*t;
		double yinc = newy / (800.0/speed + (1.0 - sanity)*(4.0*speed + float_rand()*(double)100)*change_type)*t;


		if(abs((int)get<0>(newc) - (int)x) > max(15.0, xinc)){
			x += xinc;
			moving = true;
		}
		if(abs((int)get<1>(newc) - (int)y) > max(15.0, yinc)) {
			y += yinc;
			moving = true;
		}
	}
	void behave(vector<Entity*>* ents, vector<int>* klist, vector<Entity*>* alist, int t, int MPC[6]){
		if(!is_mate){
			if(!moving && !reproducing && (((positivity+float_rand()*3)/4 >= (double)1.0-t/(18.0-age/600)) || ents->size() < REP_CHOICE_START_POP)) {
				MPC[0]++;
				// cout << id << " pursuing reproduction" << endl;

				pair<float, vector<Entity*>::iterator> easiest_e = make_pair(2.0, ents->begin()+self_entsi);

				if(matei == self_entsi){
					int color_dist;
					for(auto e = ents->begin(); e != ents->end(); e++){
						int color_dist = get_color_dist((*e)->color, color);
						float color_closeness = 1.0-color_dist/255.0;
						float map_closeness = 1.0-distance_formula(make_tuple((*e)->x, (*e)->y), make_tuple(x, y))/(float)DIAG_DIST;
						float effort = (float)((map_closeness+color_closeness)/4.0);
						if(((color_dist <= fav_color_tol*255 && effort < easiest_e.first) || matei < REP_CHOICE_START_POP) && distance(ents->begin(), e) != self_entsi){
							easiest_e = make_pair(effort, e);
						}
					}
					if(distance(ents->begin(), easiest_e.second) != self_entsi){
						matei = distance(ents->begin(), easiest_e.second);
						reproducing = true;
						(*easiest_e.second)->reg_as_mate();
						cout << id << " started reproduction" << endl;
					}
				}
			}
			else if(reproducing){
				MPC[1]++;
				if(find(klist->begin(), klist->end(), matei) == klist->end() && matei < ents->size()){
					vector<Entity*>::iterator mate = ents->begin() + matei;
					int color_dist = get_color_dist((*mate)->fav_color, color);

					if((ents->size() < REP_CHOICE_START_POP || color_dist <= fav_color_tol*255) && repc < 2500){
						repc+=t;
						if(distance_formula(make_tuple((int)(*mate)->x, (int)(*mate)->y), make_tuple((int)x, (int)y)) <= 30){
							cout << id << " finished reproducing" << endl;
							reproduce(ents->at(matei), ents, alist);
							reproducing = false;
							moving = false;
							matei = self_entsi;
							repc = 0;
						} else {
							if(move_speed != 8.0) move_speed = 8.0;
							move(make_tuple((*mate)->x, (*mate)->y), move_speed, t);
						}
					} else {
						repc = 0;
						moving = false;
						reproducing = false;
						matei = self_entsi;
					}
				} else {
					repc = 0;
					reproducing = false;
					moving = false;
					matei = self_entsi;
				}
			} else if(!moving && float_rand()/20 > positivity && targeti == self_entsi) {
				MPC[2]++;
				cout << id << " pursuing fighting" << endl;

				pair<float, vector<Entity*>::iterator> easiest_e = make_pair(2.0, ents->begin()+self_entsi);

				if(targeti == self_entsi){
					int color_dist;
					for(auto e = ents->begin(); e != ents->end(); e++){
						int color_dist = get_color_dist((*e)->color, lfav_color);
						float color_closeness = 1.0-color_dist/255.0;
						float map_closeness = 1.0-distance_formula(make_tuple((*e)->x, (*e)->y), make_tuple(x, y))/(float)DIAG_DIST;
						float effort = (float)((map_closeness*3.0+color_closeness)/8.0);
						if(distance(ents->begin(), e) != self_entsi && color_dist <= agro_color_tol*255 && effort < easiest_e.first){
							easiest_e = make_pair(effort, e);
						}
					}
					if(distance(ents->begin(), easiest_e.second) != self_entsi){
						targeti = distance(ents->begin(), easiest_e.second);
					}
				}
			}
			else if(targeti != self_entsi){
				MPC[3]++;
				if(targeti < ents->size()){
					vector<Entity*>::iterator target = ents->begin() + targeti;
					int color_dist = get_color_dist((*target)->color, color);

					if(color_dist <= agro_color_tol*255 && targetc < 2500){
						targetc+=t;

						if(distance_formula(make_tuple((int)(*target)->x, (int)(*target)->y), make_tuple((int)x, (int)y)) <= 15){
							(*target)->defect(t);
							(*target)->sanity -= ((*target)->sanity - (*target)->sanity * (*target)->agetol) * (*target)->psych_sensitivity;
						} else {
							if(move_speed != 6.0) move_speed = 6.0;
							move(make_tuple((*target)->x, (*target)->y), move_speed, t);
						}

						if(round((*target)->sanity) < 1.0){
							klist->push_back(targeti);
							targeti = self_entsi;
							moving = false;
							targetc = 0;
							targeti = self_entsi;
						}
					} else {
						targetc = 0;
						moving = false;
						targeti = self_entsi;
					}

				} else {
					targetc = 0;
					moving = false;
					targeti = self_entsi;
				}
			} else if(!moving) {
				MPC[4]++;
				int rx = SCREEN_WIDTH*float_rand();
				int ry = SCREEN_HEIGHT*float_rand();
				move(make_tuple(rx, ry), 5.0, t);
			} else {
				MPC[5]++;
				move(newc, move_speed, t);
			}
		}
	}
	void reg_as_mate(){
		is_mate = true;
		moving = false;
		reproducing = false;
	}
	void reg_as_not_mate(){
		is_mate = false;
	}
	void inc_age(int n){
		age += n;
	}
	bool die_if_old(vector<int>* klist){
		if(age > (double)4000*agetol){
			klist->push_back(self_entsi);
			return true;
		}
		return false;
	}
	void reproduce(Entity* mate, vector<Entity*>* ents, vector<Entity*>* alist){
		map<string, float> opts;
		int mutprob;
		if(float_rand() > 0.3){
			bool mate_is_mutprob = float_rand() > 0.5;
			if(mate_is_mutprob){
				mutprob = mate->mutation_prob;
			} else {
				mutprob = mutation_prob;
			}
		} else {
			cout << "retarded entity produced" << endl;
			float predef = 0.75 + float_rand()/4;
			mutprob = predef + float_rand()*(1.0-predef);
		}

		opts = {
			{"pos", get_rep_trait(mate->positivity, positivity, mutprob)},
			{"pick", get_rep_trait(mate->pickiness, pickiness, mutprob)},
			{"hostl", get_rep_trait(mate->hostility, hostility, mutprob)},
			{"psysense", get_rep_trait(mate->psych_sensitivity, psych_sensitivity, mutprob)},
			{"atol", get_rep_trait(mate->agetol, agetol, mutprob)},
			{"mutprob", mutprob},
			{"x", get_rep_trait(mate->x, x, mutprob)},
			{"y", get_rep_trait(mate->y, y, mutprob)}
		};		

		vector<int> c = {(int)get_rep_trait(mate->color.at(0), color.at(0), mutprob), (int)get_rep_trait(mate->color.at(1), color.at(1), mutprob), (int)get_rep_trait(mate->color.at(2), color.at(2), mutprob)};
		vector<int> favc = {(int)get_rep_trait(mate->fav_color.at(0), fav_color.at(0), mutprob), (int)get_rep_trait(mate->fav_color.at(1), fav_color.at(1), mutprob), (int)get_rep_trait(mate->fav_color.at(2), fav_color.at(2), mutprob)};

		alist->push_back(new Entity(c, favc, &opts, ents, ents->size()+alist->size()));
		mate->reg_as_not_mate();
	}
	void draw(SDL_Renderer* ren, vector<Entity*>* ents){
		if(x > SCREEN_WIDTH){
			x = fmod(x, SCREEN_WIDTH);
		} else if(x < 0){
			x = SCREEN_WIDTH + x;
		}

		if(y > SCREEN_HEIGHT){
			y = fmod(y, SCREEN_HEIGHT);
		} else if(y < 0){
			y = SCREEN_HEIGHT + y;
		}

		choord_r->x = x;
		choord_r->y = y;

		SDL_SetRenderDrawColor(ren, color.at(0), color.at(1), color.at(2), 255);
		if(SDL_RenderFillRect(ren, choord_r) != 0){
			printf("error drawing rect %s\n", SDL_GetError());
		}

		if(reproducing && !is_mate){
			vector<Entity*>::iterator mate = ents->begin() + matei;
			SDL_RenderDrawLine(ren, (int)x, (int)y, (int)(*mate)->x, (int)(*mate)->y);
		}
	}
	Entity(vector<int> c, vector<int> favc, map<string, float>* opts, vector<Entity*>* ents, int selfi){
		id = rand_str(15);
		color = c;
		positivity = (*opts).at("pos");
		pickiness = (*opts).at("pick");
		hostility = (*opts).at("hostl");
		agro_color_tol = pickiness*hostility;
		fav_color_tol = pickiness*0.75;
		psych_sensitivity = (*opts).at("psysense");
		self_entsi = selfi;
		matei = self_entsi;
		targeti = self_entsi;
		agetol = opts->at("atol");
		fav_color = favc;
		lfav_color = invert_color(favc);
		x = (*opts).at("x");
		y = (*opts).at("y");
		choord_r->x = x;
		choord_r->y = y;
		choord_r->w = 15;
		choord_r->h = 15;
		mutation_prob = (*opts).at("mutprob");
	}
	~Entity(){
		delete choord_r;
	}
};