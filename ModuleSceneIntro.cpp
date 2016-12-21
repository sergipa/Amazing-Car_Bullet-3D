#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	leftLastCubePos = { -5.8f,1,0 };
	rightLastCubePos = { 8.6f,1,0 };

	floor.size = vec3(1000, 0.01f, 1000);
	floor.axis = true;
	floor.color = Grey;
	floor.wire = false;

	CreateCircuitStraight(20,STRAIGHT_VERTICAL,STRAIGHT_UP);
	CreateCircuitCurve(CURVE_FULL, CURVE_UP_LEFT);
	CreateCircuitCurve(CURVE_HALF, CURVE_DOWN_LEFT);
	CreateCircuitStraight(10, STRAIGHT_HORIZONTAL, STRAIGHT_LEFT);
	CreateCircuitCurve(CURVE_FULL, CURVE_LEFT_UP);
	CreateCircuitCurve(CURVE_FULL, CURVE_RIGHT_UP);
	CreateCircuitCurve(CURVE_FULL, CURVE_LEFT_UP);
	CreateCircuitStraight(30, STRAIGHT_HORIZONTAL, STRAIGHT_RIGHT);
	
	vec3 rampPos = rightLastCubePos + (leftLastCubePos - rightLastCubePos) / 2;
	CreateRamp(RAMP_RIGHT, 23, vec3(15,2,11), rampPos);

	ObstacleZone.size = vec3(350, 1, 50);
	vec3 zonePos = rightLastCubePos + (leftLastCubePos - rightLastCubePos) / 2;
	ObstacleZone.SetPos(zonePos.x - 205, zonePos.y + 8, zonePos.z);
	App->physics->AddBody(ObstacleZone, this, 0);

	CreateColumns(2, 10, { ObstacleZone.GetPosition().x + (ObstacleZone.size.x / 2.5f), ObstacleZone.GetPosition().y + 2, ObstacleZone.GetPosition().z - (ObstacleZone.size.z / 3.5f) }, 90, { 0,-1,1 });
	CreateColumns(2, 10, { ObstacleZone.GetPosition().x + (ObstacleZone.size.x / 3), ObstacleZone.GetPosition().y + 2, ObstacleZone.GetPosition().z + (ObstacleZone.size.z / 3) }, 90, { 0,1,1 });
	CreateColumns(2, 10, { ObstacleZone.GetPosition().x + (ObstacleZone.size.x / 4.5f), ObstacleZone.GetPosition().y + 2, ObstacleZone.GetPosition().z }, 90, { 0,-1,1 });
	CreateColumns(2, 10, { ObstacleZone.GetPosition().x + (ObstacleZone.size.x / 6), ObstacleZone.GetPosition().y + 2, ObstacleZone.GetPosition().z + (ObstacleZone.size.z / 5) }, 90, { 0,1,1 });
	CreateColumns(2, 10, { ObstacleZone.GetPosition().x + (ObstacleZone.size.x / 10), ObstacleZone.GetPosition().y + 2, ObstacleZone.GetPosition().z - (ObstacleZone.size.z / 3) }, 90, { 0,1,1 });
	CreateColumns(2, 10, { ObstacleZone.GetPosition().x - 10, ObstacleZone.GetPosition().y + 2, ObstacleZone.GetPosition().z }, 90, { 0,0,1 });
	CreateColumns(2, 30, { ObstacleZone.GetPosition().x, ObstacleZone.GetPosition().y + 10, ObstacleZone.GetPosition().z + (ObstacleZone.size.z /8) }, 90, { 0,1,1 });
	CreateColumns(2, 30, { ObstacleZone.GetPosition().x, ObstacleZone.GetPosition().y + 10, ObstacleZone.GetPosition().z - (ObstacleZone.size.z / 8) }, 90, { 0,-1,1 });

	ObstacleZone2.size = vec3(50, 1, 350);
	ObstacleZone2.SetPos(ObstacleZone.GetPosition().x * 2 + 4, ObstacleZone.GetPosition().y + 0.1f, 12.5f);
	App->physics->AddBody(ObstacleZone2, this, 0);

	CreateRamp(RAMP_RIGHT, 23, vec3(ObstacleZone2.size.x / 2, ObstacleZone2.size.y, ObstacleZone2.size.z), 
		vec3(ObstacleZone2.GetPosition().x - ObstacleZone2.size.x / 1.5f, ObstacleZone2.GetPosition().y + 3, 12.5f));

	CreateRamp(RAMP_LEFT, 23, vec3(ObstacleZone2.size.x / 2, ObstacleZone2.size.y, ObstacleZone2.size.z - ObstacleZone.size.z),
		vec3(ObstacleZone2.GetPosition().x + ObstacleZone2.size.x / 1.5f, ObstacleZone2.GetPosition().y + 3, ObstacleZone2.GetPosition().z - ObstacleZone.size.z / 2));

	CreatePendulum(5, { ObstacleZone2.GetPosition().x, ObstacleZone2.GetPosition().y + 40, ObstacleZone2.GetPosition().z - ObstacleZone2.size.x * 2 }, { -34,0,0 });
	CreatePendulum(5, { ObstacleZone2.GetPosition().x, ObstacleZone2.GetPosition().y + 40, ObstacleZone2.GetPosition().z - ObstacleZone2.size.x}, { 34,0,0 });
	CreatePendulum(5, { ObstacleZone2.GetPosition().x, ObstacleZone2.GetPosition().y + 40, ObstacleZone2.GetPosition().z }, { -34,0,0 });
	CreatePendulum(5, { ObstacleZone2.GetPosition().x, ObstacleZone2.GetPosition().y + 40, ObstacleZone2.GetPosition().z + ObstacleZone2.size.x}, { 34,0,0 });
	CreatePendulum(5, { ObstacleZone2.GetPosition().x, ObstacleZone2.GetPosition().y + 40, ObstacleZone2.GetPosition().z + ObstacleZone2.size.x * 2}, { -34,0,0 });

	//Cannon
	CreateTunnel({ ObstacleZone2.GetPosition().x,ObstacleZone2.GetPosition().y + 0.5f,ObstacleZone2.GetPosition().z - ObstacleZone2.size.z / 2 }, 7, 30);
	cannonSensor.size = vec3(7, 1, 1);
	cannonSensor.SetPos(ObstacleZone2.GetPosition().x, ObstacleZone2.GetPosition().y + 2, ObstacleZone2.GetPosition().z - ObstacleZone2.size.z / 2 - 8);
	cannonSensorPhysBody = App->physics->AddBody(cannonSensor, this, 0);
	cannonSensorPhysBody->collision_listeners.add(this);
	cannonSensorPhysBody->SetAsSensor(true);
	cannonActivated = false;

	//goal platform
	goalZone.size = vec3(400, 1, 500);
	goalZone.SetPos(ObstacleZone2.GetPosition().x, -0.1f, ObstacleZone2.GetPosition().z - ObstacleZone2.size.z - 300);
	goalZoneBody = App->physics->AddBody(goalZone, this, 0);
	goalZoneBody->SetAsSensor(true);

	Cube goalLineCube(10, 0.1f, 10);
	for (int i = 0; i < (goalZone.size.x / goalLineCube.size.x); i++) {
		goalLineCube.SetPos((goalZone.GetPosition().x - goalZone.size.x / 2) + i * goalLineCube.size.x, goalZone.GetPosition().y + 1, goalZone.GetPosition().z);
		if (i % 2 == 0) {
			goalLineCube.color = Black;
		}
		else {
			goalLineCube.color = White;
		}
		goal.add(goalLineCube);
	}

	gameTimer.Start();
	timerGetTime = 0;

	App->player->playerCoins = 0;
	for (float i = 1; i < 6; i++) {
		CreateCoins({ 1.5f,1,10+(8*i) });
	}
	CreateCoins({ 10,1,82 });
	CreateCoins({ 47,1,52 });
	CreateCoins({ 57,1,40 });
	CreateCoins({ 77,1,40 });
	CreateCoins({ 107,1,40 });
	CreateCoins({ 107,1,70 });
	CreateCoins({ 107,1,125 });
	CreateCoins({ 107,1,155 });
	CreateCoins({ 50,1,160 });
	CreateBoost({ 35,1,165 });
	CreateBoost({ -40,10.4f,165 });
	CreateCoins({ -60,10.4f,165 });
	CreateCoins({ -80,10,175 });
	CreateCoins({ -130,10,175 });
	CreateCoins({ -145,10,165 });
	CreateCoins({ -155,10,175 });
	CreateCoins({ -212,10,175 });
	CreateCoins({ -400,10,135 });
	for (float i = 1; i < 18; i++) {
		CreateCoins({ -400,10,135-15*i });
	}
	for (float i = 0; i < 6; i++) {
		CreateBoost({ -400,10,135 - 40 * i });
	}

	CreateCheckpoint({ 1.5f,0.2f,0.5f });
	CreateCheckpoint({ -50,10,165 });
	CreateCheckpoint({ -400,10,135});

	App->audio->PlayMusic("Background_music.ogg");
	coin_sound = App->audio->LoadFx("smw_coin.wav");
	checkpoint_sound = App->audio->LoadFx("Checkpoint.wav");
	boost_sound = App->audio->LoadFx("boost.wav");

	//checkpoint_sound;
	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{

	for (p2List_item<Cube>* item = leftCircuitCubes.getFirst(); item != nullptr; item = item->next) {
		item->data.Render();
	}
	for (p2List_item<Cube>* item = rightCircuitCubes.getFirst(); item != nullptr; item = item->next) {
		item->data.Render();
	}
	for (p2List_item<Cube>* item = rampList.getFirst(); item != nullptr; item = item->next) {
		item->data.Render();
	}
	for (p2List_item<Cylinder>* item = columnsList.getFirst(); item != nullptr; item = item->next) {
		item->data.Render();
	}

	for (p2List_item<Cube>* item = goal.getFirst(); item != nullptr; item = item->next) {
		item->data.Render();
	}

	for (uint i = 0; i < ballsRampPhysBody.Count(); i++) {
		ballsRampPhysBody[i]->GetTransform(&(ballsRampLists[i].transform));
		ballsRampPhysBody[i]->body->applyCentralForce({ 0,-25000,0 });
		ballsRampLists[i].Render();
	}

	for (uint i = 0; i < coinList.Count(); i++) {
		if (coinList[i].active == true) {
			coinList[i].bodyInfo.Render();
		}
	}

	for (uint i = 0; i < boostList.Count(); i++) {
		if (boostList[i].active == true) {
			boostList[i].bodyInfoB.Render();
		}
	}

	for (uint i = 0; i < checkpointList.Count(); i++) {
		if (checkpointList[i].active == true) {
			checkpointList[i].bodyInfo.Render();
		}
	}

	floor.Render();
	ObstacleZone.Render();
	ObstacleZone2.Render();
	goalZone.Render();

	if (cannonActivated) {
		if ((gameTimer.Read() / 1000) % 60 < timerGetTime + 3) {
			App->player->vehicle->body->setLinearVelocity(btVector3(0, 0, 0));
			App->player->vehicle->body->setAngularVelocity(btVector3(0, 0, 0));
		}
		if ((gameTimer.Read() / 1000) % 60 < timerGetTime + 10) {
			App->player->vehicle->body->applyCentralImpulse(App->player->vehicle->body->getWorldTransform().getBasis().getColumn(2) * 1000);
		}
		else {
			cannonActivated = false;
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body1->is_sensor) {
		for (uint i = 0; i < coinsPhysBodies.Count(); i++) {
			if (body1 == coinsPhysBodies[i]) {
				App->player->playerCoins += 1;
				body1->is_sensor = false;
				coinList[i].active = false;
				App->audio->PlayFx(coin_sound);
			}
			if (body1 == cannonSensorPhysBody) {
				cannonActivated = true;
			}
		}
		for (uint i = 0; i < boostPhysBodies.Count(); i++) {
			if (body1 == boostPhysBodies[i]) {
				App->player->turbo += 50;
				body1->is_sensor = false;
				boostList[i].active = false;
				App->audio->PlayFx(boost_sound);
			}
		}
		for (uint i = 0; i < checkpointPhysBodies.Count(); i++) {
			if (body1 == checkpointPhysBodies[i]) {
				App->player->current_checkpoint = i;
				checkpointList[i].bodyInfo.color = Green;
				body1->is_sensor = false;
				App->player->vehicle->GetTransform(App->player->car_transform_matrix);
				App->audio->PlayFx(checkpoint_sound);
				if (i < 0) {
					checkpointList[i - 1].active = false;
				}
			}
		}
		if (body1 == cannonSensorPhysBody) {
			cannonActivated = true;
			timerGetTime = (gameTimer.Read() / 1000) % 60;
			body1->is_sensor = false;
			body1->SetAsSensor(false);
		}
		if (body1 == goalZoneBody) {
			if (App->player->playerCoins > App->player->bestPlayerCoins) App->player->bestPlayerCoins = App->player->playerCoins;
			if (App->player->lapTime.Read() > App->player->bestTime) App->player->bestTime = App->player->lapTime.Read();
			App->player->vehicle->Brake(5000);
		}
	}
}

void ModuleSceneIntro::ResetSceneElements() {
	for (uint i = 0; i < coinList.Count(); i++) {
		if (!coinList[i].active) {
			coinList[i].active = true;
		}
		if (!coinsPhysBodies[i]->is_sensor) {
			coinsPhysBodies[i]->is_sensor = true;
		}
	}

	for (uint i = 0; i < boostList.Count(); i++) {
		if (!boostList[i].active) {
			boostList[i].active = true;
		}
		if (!boostPhysBodies[i]->is_sensor) {
			boostPhysBodies[i]->is_sensor = true;
		}
	}

	for (uint i = 0; i < checkpointList.Count(); i++) {
		if (!checkpointList[i].active) {
			checkpointList[i].active = true;
		}
		if (!checkpointPhysBodies[i]->is_sensor) {
			checkpointPhysBodies[i]->is_sensor = true;
		}
	}
}

void ModuleSceneIntro::CreateCircuitStraight(uint cubesAmount, enum StraightType type, enum StraightDirection direction)
{
	Cube cubeCircuit;
	cubeCircuit.size = { 1,1,1 };
	int negative = -1;
	
	if (direction == STRAIGHT_UP || direction == STRAIGHT_RIGHT) {
		negative = 1;
	}

	switch (type) {
	case STRAIGHT_VERTICAL:
		for (int i = 0; i < cubesAmount; i++) {
			if (i % 2 == 0) {
				cubeCircuit.color = Red;
			}
			else {
				cubeCircuit.color = White;
			}
			cubeCircuit.SetPos(leftLastCubePos.x, leftLastCubePos.y, leftLastCubePos.z);
			leftLastCubePos.z += 3 * negative;
			leftCircuitCubes.add(cubeCircuit);
			App->physics->AddBody(cubeCircuit, this, 0);

			cubeCircuit.SetPos(rightLastCubePos.x, rightLastCubePos.y, rightLastCubePos.z);
			rightLastCubePos.z += 3 * negative;
			rightCircuitCubes.add(cubeCircuit);
			App->physics->AddBody(cubeCircuit, this, 0);
		}
		break;
	case STRAIGHT_HORIZONTAL:
		for (int i = 0; i < cubesAmount; i++) {
			if (i % 2 == 0) {
				cubeCircuit.color = Red;
			}
			else {
				cubeCircuit.color = White;
			}
			cubeCircuit.SetPos(leftLastCubePos.x, leftLastCubePos.y, leftLastCubePos.z);
			leftLastCubePos.x -= 3 * negative;
			leftCircuitCubes.add(cubeCircuit);
			App->physics->AddBody(cubeCircuit, this, 0);

			cubeCircuit.SetPos(rightLastCubePos.x, rightLastCubePos.y, rightLastCubePos.z);
			rightLastCubePos.x -= 3 * negative;
			rightCircuitCubes.add(cubeCircuit);
			App->physics->AddBody(cubeCircuit, this, 0);
		}
		break;
	}
}

void ModuleSceneIntro::CreateCircuitCurve(CurveType type, CurveDirection direction)
{
	Cube cubeCircuit;
	cubeCircuit.size = { 1,1,1 };

	bool horizontalDirection = false;

	if (direction == CURVE_RIGHT_UP || direction == CURVE_RIGHT_DOWN || direction == CURVE_LEFT_UP || direction == CURVE_LEFT_DOWN) {
		horizontalDirection = true;
	}

	float addLongZPos;
	float addLongXPos;
	float addShortZPos;
	float addShortXPos;

	float Znegative = 1;
	float Xnegative = 1;

	float x;
	float z;

	if (direction == CURVE_UP_LEFT || direction == CURVE_LEFT_UP) {
		Xnegative = -1;
	}
	if (direction == CURVE_DOWN_LEFT || direction == CURVE_LEFT_DOWN) {
		Znegative = -1;
		Xnegative = -1;
	}
	if (direction == CURVE_DOWN_RIGHT || direction == CURVE_RIGHT_DOWN) {
		Znegative = -1;
	}

	for (int i = 0; i < 40 / type; i++) {
		if (i < 12) {
			addLongZPos = ((horizontalDirection) ? 1 : 2) * Znegative;
			addLongXPos = ((horizontalDirection) ? -2 : -1) * Xnegative;
		}
		else if (i >= 12 && i < 18) {
			addLongZPos = ((horizontalDirection) ? 2 : 1) * Znegative;
			addLongXPos = ((horizontalDirection) ? -1 : -2) * Xnegative;
		}
		else if (i >= 18 && i < 22) {
			addLongZPos = ((horizontalDirection) ? 2 : 0) * Znegative;
			addLongXPos = ((horizontalDirection) ? 0 : -2) * Xnegative;
		}
		else if (i >= 22 && i < 28) {
			addLongZPos = ((horizontalDirection) ? 2 : -1) * Znegative;
			addLongXPos = ((horizontalDirection) ? 1 : -2) * Xnegative;
		}
		else if (i >= 28) {
			addLongZPos = ((horizontalDirection) ? 1 : -2) * Znegative;
			addLongXPos = ((horizontalDirection) ? 2 : -1) * Xnegative;
		}

		addShortZPos = addLongZPos / 2;
		addShortXPos = addLongXPos / 2;

		/*if (i % 2 == 0) {
		cubeCircuit.color = Red;
		}
		else {
		cubeCircuit.color = White;
		}*/

		if (direction == CURVE_UP_RIGHT || direction == CURVE_DOWN_LEFT || direction == CURVE_RIGHT_DOWN || direction == CURVE_LEFT_UP) {
			cubeCircuit.SetPos(leftLastCubePos.x, leftLastCubePos.y, leftLastCubePos.z);
			leftLastCubePos.z += addShortZPos;
			leftLastCubePos.x += addShortXPos;
			if (i % 2 == 0) {
				leftCircuitCubes.add(cubeCircuit);
				App->physics->AddBody(cubeCircuit, this, 0);
			}

			cubeCircuit.SetPos(rightLastCubePos.x, rightLastCubePos.y, rightLastCubePos.z);
			rightLastCubePos.z += addLongZPos;
			rightLastCubePos.x += addLongXPos;
			rightCircuitCubes.add(cubeCircuit);
			App->physics->AddBody(cubeCircuit, this, 0);
		}
		else {
			cubeCircuit.SetPos(leftLastCubePos.x, leftLastCubePos.y, leftLastCubePos.z);
			leftLastCubePos.z += addLongZPos;
			leftLastCubePos.x += addLongXPos;
			leftCircuitCubes.add(cubeCircuit);
			App->physics->AddBody(cubeCircuit, this, 0);

			cubeCircuit.SetPos(rightLastCubePos.x, rightLastCubePos.y, rightLastCubePos.z);
			rightLastCubePos.z += addShortZPos;
			rightLastCubePos.x += addShortXPos;
			if (i % 2 == 0) {
				rightCircuitCubes.add(cubeCircuit);
				App->physics->AddBody(cubeCircuit, this, 0);
			}
		}
	}
	int i = 0;
	for (p2List_item<Cube>* item = leftCircuitCubes.getFirst(); item != nullptr; item = item->next, i++) {
		item->data.Render();
		if (i % 2 == 0) {
			item->data.color = Red;
		}
		else {
			item->data.color = White;
		}
	}
	i = 0;
	for (p2List_item<Cube>* item = rightCircuitCubes.getFirst(); item != nullptr; item = item->next, i++) {
		item->data.Render();
		if (i % 2 == 0) {
			item->data.color = Red;
		}
		else {
			item->data.color = White;
		}
	}
}

void ModuleSceneIntro::CreateRamp(int dir, float angle, vec3 size, vec3 pos)
{
	Cube ramp(size.x, size.y, size.z);
	ramp.SetPos(pos.x, pos.y, pos.z);

	switch (dir) {
	case RAMP_RIGHT:
		ramp.SetRotation(angle, vec3{ 0.0f, 0.0f, -1.0f });
		break;
	case RAMP_LEFT:
		ramp.SetRotation(angle, vec3{ 0.0f, 0.0f, 1.0f });
		break;
	case RAMP_UP:
		ramp.SetRotation(angle, vec3{ -1.0f, 0.0f, 0.0f });
		break;
	case RAMP_DOWN:
		ramp.SetRotation(angle, vec3{ 1.0f, 0.0f, 0.0f });
		break;
	}

	rampList.add(ramp);
	App->physics->AddBody(ramp, this, 0);
}

void ModuleSceneIntro::CreateCoins(vec3 pos)
{
	Coin coin;
	coin.bodyInfo.radius = 0.5f;
	coin.bodyInfo.SetPos(pos.x,pos.y,pos.z);
	coin.bodyInfo.height = 0.2f;
	coin.bodyInfo.color = Yellow;
	coin.bodyInfo.SetRotation(90, vec3(0, 1, 0));
	coin.active = true;
	coin.coinPhysBody = App->physics->AddBody(coin.bodyInfo, this, 0);
	coin.coinPhysBody->SetAsSensor(true);
	coinList.PushBack(coin);
	coinsPhysBodies.PushBack(coin.coinPhysBody);
}

void ModuleSceneIntro::CreateBoost(vec3 pos)
{
	Boost boost;
	boost.bodyInfoB.radius = 0.5f;
	boost.bodyInfoB.SetPos(pos.x, pos.y, pos.z);
	boost.bodyInfoB.height = 0.2f;
	boost.bodyInfoB.color = Red;
	boost.bodyInfoB.SetRotation(90, vec3(0, 1, 0));
	boost.active = true;
	boost.boostPhysBody = App->physics->AddBody(boost.bodyInfoB, this, 0);
	boost.boostPhysBody->SetAsSensor(true);
	boostList.PushBack(boost);
	boostPhysBodies.PushBack(boost.boostPhysBody);
}

void ModuleSceneIntro::CreateCheckpoint(vec3 pos)
{
	Checkpoint checkpoint;
	checkpoint.bodyInfo.size = (vec3(10, 0.02f, 10));
	checkpoint.bodyInfo.SetPos(pos.x, pos.y, pos.z);
	checkpoint.bodyInfo.color = Red;
	checkpoint.active = true;
	checkpoint.bodyInfo.SetRotation(0, vec3(0, 1, 0));
	checkpoint.checkpointPhysBody = App->physics->AddBody(checkpoint.bodyInfo, this, 0);
	checkpoint.checkpointPhysBody->SetPos(checkpoint.bodyInfo.GetPosition().x,checkpoint.bodyInfo.GetPosition().y+1.6f, checkpoint.bodyInfo.GetPosition().z);
	checkpoint.checkpointPhysBody->SetAsSensor(true);
	//checkpoint.checkpointPhysBody->body->setGravity(btVector3(0.0f, 0.0f, 0.0f));
	checkpointList.PushBack(checkpoint);
	checkpointPhysBodies.PushBack(checkpoint.checkpointPhysBody);
}

void ModuleSceneIntro::CreateColumns(uint radius, uint height, vec3 pos, float angle, vec3 angleDirection)
{
	Cylinder column;
	column.radius = radius;
	column.height = height;
	column.SetPos(pos.x, pos.y, pos.z);
	column.SetRotation(angle, angleDirection);
	columnsList.add(column);
	App->physics->AddBody(column, this, 0);
}

void ModuleSceneIntro::CreatePendulum(uint ballRadius, vec3 boxPos, vec3 ballFromBoxPos)
{

	Cube box(1, 1, 1);
	box.SetPos(boxPos.x, boxPos.y, boxPos.z);
	PhysBody3D* boxPhysBody = App->physics->AddBody(box, this, 2000);
	boxPhysBody->body->setGravity({ 0,0,0 });

	Sphere ball(ballRadius);
	ball.SetPos(box.GetPosition().x + ballFromBoxPos.x, box.GetPosition().y + ballFromBoxPos.y, box.GetPosition().z + ballFromBoxPos.z);
	ballsRampLists.PushBack(ball);
	PhysBody3D* ballPhysBody = App->physics->AddBody(ball, this, 2000);
	ballPhysBody->body->setGravity({ 0,0,0 });
	ballsRampPhysBody.PushBack(ballPhysBody);

	btTransform ballTrans;
	btTransform boxTrans;
	ballTrans.setIdentity();
	boxTrans.setIdentity();
	btQuaternion qt;
	qt.setEuler(0, 0, SIMD_HALF_PI);
	boxTrans.setRotation(qt); //we use -Y like up Axis
	ballTrans.setRotation(qt); //we use -Y like up Axis

	//Obtain the position of ball in local reference frame of box
	btVector3 ballInBoxLRF =
		(boxPhysBody->body->getWorldTransform().inverse()
		(ballPhysBody->body->getWorldTransform().getOrigin()));
	boxTrans.setOrigin(ballInBoxLRF);

	App->physics->AddConstraintP2P(*boxPhysBody, { 0,0,0 });
	App->physics->AddConstraintSlider(*boxPhysBody, *ballPhysBody, boxTrans, ballTrans, true);
	
}

void ModuleSceneIntro::CreateTunnel(vec3 pos, float radius, float height)
{
	Cube startPlane;
	startPlane.size = vec3(radius, 0.2f, height);
	startPlane.SetPos(pos.x,pos.y,pos.z);
	rampList.add(startPlane);
	App->physics->AddBody(startPlane, this, 0);

	int negative = -1;

	for (uint i = 0; i < 2; i++) {
		CreateRamp(i, 30, startPlane.size, vec3((startPlane.GetPosition().x + (startPlane.size.x - 0.1f * radius) * negative), startPlane.GetPosition().y + 0.25f * radius, startPlane.GetPosition().z));
		CreateRamp(i, 60, startPlane.size, vec3((startPlane.GetPosition().x + (startPlane.size.x + 0.55f * radius) * negative), startPlane.GetPosition().y + 0.9f * radius, startPlane.GetPosition().z));
		CreateRamp(i, 90, startPlane.size, vec3((startPlane.GetPosition().x + (startPlane.size.x + 0.8f * radius) * negative), startPlane.GetPosition().y + 1.8f * radius, startPlane.GetPosition().z));
		CreateRamp(i, 120, startPlane.size, vec3((startPlane.GetPosition().x + (startPlane.size.x + 0.55f * radius) * negative), startPlane.GetPosition().y + 2.7f * radius, startPlane.GetPosition().z));
		CreateRamp(i, 150, startPlane.size, vec3((startPlane.GetPosition().x + (startPlane.size.x - 0.1f * radius) * negative), startPlane.GetPosition().y + 3.35f * radius, startPlane.GetPosition().z));
		negative = 1;
	}
	CreateRamp(RAMP_LEFT, 180, startPlane.size, vec3(startPlane.GetPosition().x, startPlane.GetPosition().y + 3.6f * radius, startPlane.GetPosition().z));
}

void ModuleSceneIntro::CreatePlaneJumpZones(vec3 pos, vec3 size)
{
	Cube zone(size.x, size.y, size.z);
	zone.SetPos(pos.x, pos.y, pos.z);
}

