#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1000.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 1000.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void RestartPlayer();

public:
	uint current_checkpoint;
	uint jump_charge;
	uint turbo;
	PhysVehicle3D* vehicle;
	float turn;
	float acceleration;
	float brake;
	uint playerCoins;
	uint bestPlayerCoins;
	uint bestTime;
	Timer lapTime;
	float car_transform_matrix[16];
private:
	

	float car_velocity;
	vec3 initial_car_pos;
	uint turbo_sound;
	uint jump_sound;
	
};