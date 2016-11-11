#ifndef ENTITY_DEFINED
#define ENTITY_DEFINED
#include <SDL2/SDL.h>
#include <vector>
#include <tuple>
#include <utility>
#include <map>

#include "Disease.h"
#include "Terrain.h"

class Entity{
	vector<Entity*>* ents;
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
	bool is_mate = false;
	int matei;
	int targeti;
	string id;
	SDL_Rect* choord_r = new SDL_Rect;
	int self_entsi;
	int targetc;
	float mutation_prob;
	float fertility;
	float strength;
	double t;
	bool has_disease = false;
	Disease disease;
	double food = 1.0;
	unsigned long clostc;
	bool dead = false;
	float hunger_tol;
	Terrain* terrain;
	double cur_elvt;
public:
	bool is_dead();
	bool die_if_old();
	int get_neighbor_closeness();
	void update_indices(vector<int> dead_indices);
	void defect(int i);
	void move(tuple<int, int> inewc, float speed);
	void behave(vector<Entity*>* alist);
	void reg_as_mate(int i);
	void reg_as_not_mate();
	void inc_age();
	void reproduce(Entity* mate, vector<Entity*>* alist);
	void draw(SDL_Renderer* ren);
	void stop_reproducing(vector<Entity*>::iterator mate);
	void stop_reproducing();
	void randmove(float req_factor = 0, float speed = 5.0);
	void stop_fighting();
	void update_ents_ptr(vector<Entity*>* e);
	void update_time(double time);
	void add_disease(Disease d);
	void kill();
	Entity(vector<int> c, vector<int> favc, map<string, float>* opts, int selfi, Terrain* ter);
};
#endif