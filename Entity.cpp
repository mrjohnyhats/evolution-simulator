#include <SDL2/SDL.h>
#include <vector>
#include <tuple>
#include <utility>
#include <map>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <iterator>

#include "globals.h"
#include "Entity.h"
// extern float mouse_food;

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

void Entity::update_indices(vector<int>* klist){
	for(int i : *klist){
		if(self_entsi > i) self_entsi--;
		if(targeti > i) targeti--;
		if(matei > i) matei--;
	}
}
void Entity::defect(int t){
	int change_type;
	for(int i = 0; i < 3; i++){
		change_type = float_rand() > 0.5;
		if(change_type) change_type = -1;
		color[i] = color.at(i) += change_type*sanity*(255/4)*t/5;
		color[i] %= 256;
	}
}
void Entity::move(tuple<int, int> inewc, float speed){
	newc = inewc;
	move_speed = speed;

	// cout << "speed " << speed << endl;

	if(food > 0.4){
		speed *= 4*food;
	}

	food -= e-2*t*(speed/10)

	int change_type = (float_rand() > 0.5);

	if(change_type == 0) change_type = -1;

	moving = false;

	int xdiff = (int)abs(x - get<0>(newc)); 
	int oxdiff = (int)(SCREEN_WIDTH - x + get<0>(newc)); 
	int ydiff = (int)abs(y - get<1>(newc));
	int oydiff = (int)(SCREEN_HEIGHT - y + get<1>(newc));

	int xmul = xdiff*xdiff < oxdiff*oxdiff ? (get<0>(newc) - x)/abs(get<0>(newc) - x) : -1*(get<0>(newc) - x)/abs(get<0>(newc) - x);
	int ymul = ydiff*ydiff < oydiff*oydiff ? (get<1>(newc) - y)/abs(get<1>(newc) - y) : -1*(get<1>(newc) - y)/abs(get<1>(newc) - y);

	double inc = (speed + (1.0 - sanity)*(speed*float_rand())*change_type)*t;

	if(abs((int)get<0>(newc) - (int)x) > max(15.0, inc*xmul)){
		x += inc*xmul;
		moving = true;
	}
	if(abs((int)get<1>(newc) - (int)y) > max(15.0, inc*ymul)) {
		y += inc*ymul;
		moving = true;
	}
}
int Entity::get_neighbor_closeness(){
	int distq = 0;
	unsigned long dists = 0;

	int dist;
	for(auto e = ents->begin(); e != ents->end(); e++){
		dist = distance_formula(make_tuple(x, y), make_tuple((*e)->x, (*e)->y));
 		if(distance(ents->begin(), e) != matei && dist < 45){
			distq++;
			dists += dist;
		}
	}

	if(distq > 0){
		return round(dists/distq);
	}
	return DIAG_DIST;
}
void Entity::behave(vector<int>* klist, vector<Entity*>* alist){
	food -= 1e-3*t/4;
	int mx, my;
	SDL_GetMouseState(&mx, &my);
	int mdist = distance_formula(make_tuple((int)x, (int)y), make_tuple(mx, my));
	if(has_disease){
		float sanitydiff = disease.get_sanitydiff(sanity, color, t);
		sanity -= sanitydiff;
		cout << "sanitydiff " << sanitydiff << endl;
		if(food > 0) food -= sanitydiff*disease.get_food_effect()/10;
		cout << "fooddiff " << sanitydiff*disease.get_food_effect()/10 << endl;
		if (food < 0) food = 0;
		disease.change_strength(strength);
		if(disease.get_strength() < 0.1){
			has_disease = false;
		}
	}

	if(mdist < 30){
		if(mouse_food > 0){
			if(mouse_food > 1.0-food){
				mouse_food -= (1.0-food);
				food = 1.0;
			} else {
				food += mouse_food;
				mouse_food = 0;
			}
			randmove(0.25, 6.0);
		}
		// cout << id << " got food" << endl;
	}

	// cout << "sanity " << sanity << endl;

	float open_space_level = get_neighbor_closeness();

	if(clostc > 0 && clostc < 18)clostc += (int)(2-open_space_level/10);

	if(food > 0.3 || sanity < 0.3){
		if(reproducing){
			if(find(klist->begin(), klist->end(), matei) == klist->end() && matei < ents->size()){
				vector<Entity*>::iterator mate = ents->begin() + matei;
				int color_dist = get_color_dist((*mate)->fav_color, color);

				if(repc < 2500){
					repc+=t;
					if(distance_formula(make_tuple((int)(*mate)->x, (int)(*mate)->y), make_tuple((int)x, (int)y)) <= 30){
						cout << id << " finished reproducing" << endl;
						reproduce(ents->at(matei), alist);
						reproduced = true;
						stop_reproducing();
					} else {
						if(move_speed != 8.0) move_speed = 8.0;
						// cout << "moving towards reproduction" << endl;
						move(make_tuple((*mate)->x, (*mate)->y), move_speed);
					}
				} else {
					stop_reproducing();
				}
			} else {
				stop_reproducing();
			}
		} else if(open_space_level > 0 && open_space_level < 18){
			randmove(0.5, 20-open_space_level/2);
		} else if(!reproducing && !is_mate && ((!reproduced && (positivity+float_rand())/2 <= t/(2.0-age/4000)) || ents->size() < REP_CHOICE_START_POP)) {
			cout << "pursuing reproduction" << endl;

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
					// cout << id << " found mate" << endl;
					matei = distance(ents->begin(), easiest_e.second);
					reproducing = true;
					(*easiest_e.second)->reg_as_mate();
					//cout << id << " started reproduction" << endl;
				}
			}
		} else if(!reproducing && float_rand()/6 + age/15000 > (positivity+float_rand())/2 && targeti == self_entsi) {
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
					// cout << "found target" << endl;
					targeti = distance(ents->begin(), easiest_e.second);
				}
			}
		}
		else if(targeti != self_entsi){
			if(targeti < ents->size()){
				vector<Entity*>::iterator target = ents->begin() + targeti;
				int color_dist = get_color_dist((*target)->color, color);

				if(color_dist <= agro_color_tol*255 && targetc < 2500){
					targetc+=t;

					if(distance_formula(make_tuple((int)(*target)->x, (int)(*target)->y), make_tuple((int)x, (int)y)) <= 15){
						(*target)->defect(t);
						(*target)->sanity -= ((*target)->sanity - (*target)->sanity * (*target)->agetol) * (*target)->psych_sensitivity;
					} else {
						cout << "going to target" << endl;
						if(move_speed != 6.0) move_speed = 6.0;
						move(make_tuple((*target)->x, (*target)->y), move_speed);
					}

					if(round((*target)->sanity*10)/10 <= 0.75){
						// cout << "killed target" << endl;
						stop_fighting();
						randmove(0.5);
					}
				} else {
					stop_fighting();
				}

			} else {
				stop_fighting();
			}
		} else if(!moving) {
			// cout << "randmove" << endl;
			randmove();
		} else {
			move(newc, move_speed);
		}
	} else {
	 	if(mdist < DIAG_DIST/4){
	 		if(reproducing) stop_reproducing();
			// cout << id << " getting food, food " << food << endl;
			move(make_tuple(mx, my), 10.0*(1.0-food));
		}
	}

	if(round(sanity*10)/10 <= 0.75){
		printf("ent died of unknown causes\n");
		klist->push_back(self_entsi);
	} else if(food <= 0){
		printf("ent died of hunger\n");
		klist->push_back(self_entsi);
	}
}
void Entity::randmove(float req_factor, float speed){
	double ra = M_PI*2*float_rand();
	double rd = (float_rand() - req_factor)*DIAG_DIST + req_factor*DIAG_DIST;
	int rx = (int)abs(x+cos(ra)*rd)%SCREEN_WIDTH;
	int ry = (int)abs(y+cos(ra)*rd)%SCREEN_HEIGHT;
	move(make_tuple(rx, ry), speed);
}
void Entity::stop_reproducing(){
	repc = 0;
	moving = false;
	reproducing = false;
	matei = self_entsi;
	if(matei < ents->size()){
		ents->at(matei)->reg_as_not_mate();
	}
}
void Entity::add_disease(Disease d){
	disease = d;
	has_disease = true;
}
void Entity::stop_fighting(){
	// cout << "stopped fighting" << endl;
	targeti = self_entsi;
	moving = false;
	targetc = 0;
}
void Entity::update_ents_ptr(vector<Entity*>* e){
	ents = e;
}
void Entity::update_time(double time){
	t = time;
}
void Entity::reg_as_mate(){
	is_mate = true;
	moving = false;
	reproducing = false;
	matei = self_entsi;
}
void Entity::reg_as_not_mate(){
	is_mate = false;
}
void Entity::inc_age(){
	age += t;
}
bool Entity::die_if_old(vector<int>* klist){
	if(age > (double)8000*agetol){
		klist->push_back(self_entsi);
		return true;
	}
	return false;
}
void Entity::reproduce(Entity* mate, vector<Entity*>* alist){
	map<string, float> opts;
	int offspring_quan = (int)4*(mate->fertility + fertility)/2;
	int mutprob;
	vector<int> c;
	vector<int> favc;
	vector<Entity*>::iterator enti;

	for(int i = 0; i < offspring_quan; i++){
		if(float_rand() > 0.075){
			if(float_rand() > 0.5){
				mutprob = mate->mutation_prob;
			} else {
				mutprob = mutation_prob;
			}
		} else {
			mutprob = 1.0;
		}

		opts = {
			{"pos", get_rep_trait(mate->positivity, positivity, mutprob)},
			{"pick", get_rep_trait(mate->pickiness, pickiness, mutprob)},
			{"hostl", get_rep_trait(mate->hostility, hostility, mutprob)},
			{"psysense", get_rep_trait(mate->psych_sensitivity, psych_sensitivity, mutprob)},
			{"atol", get_rep_trait(mate->agetol, agetol, mutprob)},
			{"mutprob", get_rep_trait(mate->mutation_prob, mutation_prob, mutprob)},
			{"fert", get_rep_trait(mate->fertility, fertility, mutprob)},
			{"stren", get_rep_trait(mate->strength/2.0, strength/2.0, mutprob)*2.0},
			{"x", get_rep_trait(mate->x, x, 0)},
			{"y", get_rep_trait(mate->y, y, 0)}
		};

		
		for(int i = 0; i < 3; i++){
			float val = color.at(i)/(float)255;
			float mval = mate->color.at(i)/(float)255;
			c.push_back((int)(get_rep_trait(val, mval, mutprob)*255.0));
		}
		for(int i = 0; i < 3; i++){
			float val = fav_color.at(i)/(float)255;
			float mval = mate->fav_color.at(i)/(float)255;
			favc.push_back((int)(get_rep_trait(val, mval, mutprob)*255.0));
		}

		alist->push_back(new Entity(c, favc, &opts, ents, ents->size()+alist->size(), t));

		enti = alist->end()-1;

		if(has_disease){
			if(float_rand()*2 > disease.get_mobility()){
				(*enti)->add_disease(disease);
			}
		}
		if(mate->has_disease){
			if(float_rand()*2 > mate->disease.get_mobility()){
				(*enti)->add_disease(mate->disease);
			}
		}
		cout << "clostc " << clostc << endl;
		if(clostc > MIN_CLOSTC_FOR_DISEASE){
			if(float_rand() < 0.25*(clostc/MIN_CLOSTC_FOR_DISEASE)){
				(*enti)->add_disease(make_disease(clostc/(MIN_CLOSTC_FOR_DISEASE*10)));
			}
		}
	}
	mate->reg_as_not_mate();
}
void Entity::draw(SDL_Renderer* ren){
	// cout << "x " << x << endl;
	// cout << "y " << y << endl;
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
		printf("error drawing filled rect %s\n", SDL_GetError());
	}
	SDL_SetRenderDrawColor(ren, 255-color.at(0), 255-color.at(1), 255-color.at(2), 255);
	if(SDL_RenderDrawRect(ren, choord_r) != 0){
		printf("error drawing rect %s\n", SDL_GetError());
	}

	if(reproducing && !is_mate && matei != self_entsi){
		vector<Entity*>::iterator mate = ents->begin() + matei;
		SDL_RenderDrawLine(ren, (int)x, (int)y, (int)(*mate)->x, (int)(*mate)->y);
	}
}
Entity::Entity(vector<int> c, vector<int> favc, map<string, float>* opts, vector<Entity*>* e, int selfi, int time){
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
	fertility = opts->at("fert");
	strength = opts->at("stren");
	ents = e;
	t = time;
}
Entity::~Entity(){
	delete choord_r;
}