#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "PhysVehicle3D.h"

class btRigidBody;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();

private:
	float maxDistanceToVehicle = 25.0f;
	void CalculateViewMatrix();
	mat4x4 ViewMatrix, ViewMatrixInverse;
	PhysVehicle3D* vehicleBody;
	bool isPlaying = false;
public:
	
	vec3 X, Y, Z, Position, Reference;
	
};