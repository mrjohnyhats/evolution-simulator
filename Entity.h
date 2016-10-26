#ifndef ENTITY_DEFINED
#define ENTITY_DEFINED
#include <SDL2/SDL.h>
#include <vector>
#include <tuple>
#include <utility>
#include <map>

#include "Disease.h"

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
	bool reproduced = false;
	int matei;
	int targeti;
	string id;
	SDL_Rect* choord_r = new SDL_Rect;
	int self_entsi;
	int targetc;
	int repc;
	float mutation_prob;
	float immune_sys_strength;
	float fertility;
	int t;
	bool has_disease = false;
	Disease disease;
	int clostc = 0;
public:
	bool die_if_old(vector<int>* klist);
	int get_neighbor_closeness();
	void update_indices(int i);
	void defect(int i);
	void move(tuple<int, int> inewc, float speed);
	void behave(vector<int>* klist, vector<Entity*>* alist);
	void reg_as_mate();
	void reg_as_not_mate();
	void inc_age();
	void reproduce(Entity* mate, vector<Entity*>* alist);
	void draw(SDL_Renderer* ren);
	void stop_reproducing(vector<Entity*>::iterator mate);
	void stop_reproducing();
	void randmove(float req_factor = 0, float speed = 5.0);
	void stop_fighting();
	void update_ents_ptr(vector<Entity*>* e);
	void update_time(int time);
	void add_disease(Disease d);
	Entity(vector<int> c, vector<int> favc, map<string, float>* opts, vector<Entity*>* e, int selfi, int time);
	~Entity();
};
#endif