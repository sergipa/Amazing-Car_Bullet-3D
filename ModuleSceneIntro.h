#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

enum StraightDirection {
	STRAIGHT_UP,
	STRAIGHT_DOWN,
	STRAIGHT_RIGHT,
	STRAIGHT_LEFT
};

enum StraightType {
	STRAIGHT_VERTICAL,
	STRAIGHT_HORIZONTAL
};

enum CurveDirection {
	CURVE_UP_RIGHT,
	CURVE_UP_LEFT,
	CURVE_DOWN_RIGHT,
	CURVE_DOWN_LEFT,
	CURVE_RIGHT_UP,
	CURVE_RIGHT_DOWN,
	CURVE_LEFT_UP,
	CURVE_LEFT_DOWN
};

enum CurveType {
	CURVE_FULL = 1,
	CURVE_HALF = 2
};

enum CircuitsSpace {
	SPACE_RIGHT,
	SPACE_LEFT,
	SPACE_UP,
	SPACE_DOWN
};

enum RampDirection {
	RAMP_RIGHT,
	RAMP_LEFT,
	RAMP_UP,
	RAMP_DOWN
};

struct Coin {
	Cylinder bodyInfo;
	vec3 position;
	bool active;
	PhysBody3D* coinPhysBody;
};

struct Boost {
	Cylinder bodyInfoB;
	vec3 position;
	bool active;
	PhysBody3D* boostPhysBody;
};

struct Checkpoint {
	Cube bodyInfo;
	bool active;
	PhysBody3D* checkpointPhysBody;
};

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	void ResetSceneElements();

private:
	void CreateCircuitStraight(uint cubesAmount, enum StraightType type, enum StraightDirection direction);
	void CreateCircuitCurve(enum CurveType type, enum CurveDirection direction);
	void CreateRamp(int dir, float angle, vec3 size, vec3 pos);
	void CreateCoins(vec3 pos);
	void CreateColumns(uint radius, uint height, vec3 pos, float angle, vec3 angleDirection);
	void CreatePendulum(uint ballRadius, vec3 boxPos, vec3 ballFromBoxPos);
	void CreateTunnel(vec3 pos, float radius, float height);
	void CreatePlaneJumpZones(vec3 pos, vec3 size);
	void CreateBoost(vec3 pos);
	void CreateCheckpoint(vec3 pos);

public:
	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	p2DynArray<Checkpoint> checkpointList;
	p2DynArray<PhysBody3D*> checkpointPhysBodies;

private:
	p2List<Cube> leftCircuitCubes;
	vec3 leftLastCubePos;
	p2List<Cube> rightCircuitCubes;
	vec3 rightLastCubePos;
	p2List<Cube> rampList;
	bool lastRectIsVertical = true;
	bool lastCurveIsVertical = true;
	StraightDirection straightDir;
	StraightType straightType;
	CurveType curveType;
	CurveDirection curveDir;
	Cube ObstacleZone;
	Cube ObstacleZone2;
	Cube floor;
	p2DynArray<Coin> coinList;
	p2DynArray<PhysBody3D*> coinsPhysBodies;
	p2List<Cylinder> columnsList;
	p2DynArray<Sphere> ballsRampLists;
	p2DynArray<PhysBody3D*> ballsRampPhysBody;
	PhysBody3D* cannonSensorPhysBody = nullptr;
	Cube cannonSensor;
	bool cannonActivated;
	p2DynArray<Boost> boostList;
	p2DynArray<PhysBody3D*> boostPhysBodies;

	Cube goalZone;
	PhysBody3D* goalZoneBody;
	p2List<Cube> goal;
	Timer gameTimer;
	uint timerGetTime;
	//music
	uint background_music;
	uint coin_sound;
	uint checkpoint_sound;
	uint boost_sound;

};
