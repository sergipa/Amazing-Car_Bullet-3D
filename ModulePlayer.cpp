#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	VehicleInfo car;
	// Car properties ----------------------------------------
	car.chassis_mid_size.Set(0.66f, 1, 5);
	car.chassis_mid_offset.Set(0, 0.8f, 0);
	car.chassis_left_size.Set(0.66f, 1, 5);
	car.chassis_left_offset.Set(-0.66f, 0.8f, 0);
	car.chassis_right_size.Set(0.66f, 1, 5);
	car.chassis_right_offset.Set(0.66f, 0.8f, 0);
	car.aileron_mid_size.Set(0.66f, 0.3f, 1);
	car.aileron_mid_offset.Set(0, 1.4f, -2.5f);
	car.aileron_left_size.Set(0.66f, 0.3f, 1);
	car.aileron_left_offset.Set(0.66f, 1.4f, -2.5f);
	car.aileron_right_size.Set(0.66f, 0.3f, 1);
	car.aileron_right_offset.Set(-0.66f, 1.4f, -2.5f);
	car.upPart_mid_size.Set(0.5f, 1, 2.7f);
	car.upPart_mid_offset.Set(0, 1.68f, 0.2f);
	car.upPart_left_size.Set(0.5f, 1, 2.7f);
	car.upPart_left_offset.Set(0.5f, 1.68f, 0.2f);
	car.upPart_right_size.Set(.5f, 1, 2.7f);
	car.upPart_right_offset.Set(-0.5f, 1.68f, 0.2f);
	car.exhaust_pipe_left_size.Set(0.2f, 0.2f, 2);
	car.exhaust_pipe_left_offset.Set(-0.5f, 0.3f, -1.9f);
	car.exhaust_pipe_right_size.Set(0.2f, 0.2f, 2);
	car.exhaust_pipe_right_offset.Set(0.5f, 0.3f, -1.9f);
	car.window_size.Set(1.5f, 1, 0.1f);
	car.window_offset.Set(0, 1.68f, 1.6f);
	car.right_focus_offset.Set(-0.66f, 0.8f, 2.5f);
	car.left_focus_offset.Set(0.66f, 0.8f, 2.5f);

	car.mass = 1000.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 100.0f;
	car.maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.6f;
	float wheel_width = 0.4f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = (car.chassis_mid_size.x * 3)*0.5f;
	float half_length = car.chassis_mid_size.z*0.5f;

	vec3 direction(0, -1, 0);
	vec3 axis(-1, 0, 0);

	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius * 1.5f);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius * 1.5f);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius * 1.5f);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius * 1.5f);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;

	initial_car_pos = { 1.6f, 1, 0 };

	vehicle = App->physics->AddVehicle(car);
	vehicle->SetPos(initial_car_pos.x, initial_car_pos.x, initial_car_pos.z);
	memset(car_transform_matrix, 0, sizeof(car_transform_matrix)); // neccessary to not have memory problems
	vehicle->GetTransform(car_transform_matrix);

	jump_charge = 0;
	turbo = 100;
	bestPlayerCoins = 0;
	bestTime = 0;
	car_velocity = 0;
	current_checkpoint = 0;
	turbo_sound = App->audio->LoadFx("Ludicrous_speed.ogg");
	jump_sound = App->audio->LoadFx("jump.wav");
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

void ModulePlayer::RestartPlayer()
{
		vehicle->SetPos(App->scene_intro->checkpointList[current_checkpoint].bodyInfo.GetPosition().x, App->scene_intro->checkpointList[current_checkpoint].bodyInfo.GetPosition().y, App->scene_intro->checkpointList[current_checkpoint].bodyInfo.GetPosition().z);
		vehicle->body->setLinearVelocity(btVector3(0, 0, 0));
		vehicle->body->setAngularVelocity(btVector3(0, 0, 0));

		vehicle->SetTransform(car_transform_matrix);
		jump_charge = 0;
		turbo = 100;
		bestPlayerCoins = 0;
		bestTime = 0;
		car_velocity = 0;
		acceleration = 0;
		playerCoins = 0;
		App->scene_intro->ResetSceneElements();
	
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN){
		lapTime.Start();
	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		if (vehicle->GetKmh() < 50.0f) {
			acceleration = MAX_ACCELERATION*2;
		}
		else if (vehicle->GetKmh() > 50.0f) {
			acceleration = MAX_ACCELERATION;
		}
		if (vehicle->GetKmh() < 0) {
			brake = BRAKE_POWER / 4;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		if (vehicle->GetKmh() > -30.0f) {
			acceleration = -MAX_ACCELERATION;
		}
		if (vehicle->GetKmh() > 0) {
			brake = BRAKE_POWER/4;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if (turn < TURN_DEGREES)
			turn += TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if (turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
		if (jump_charge < 15000) {
			jump_charge += 100;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP) {
		vehicle->Push(0, jump_charge, 0);
		jump_charge = 0;
		App->audio->PlayFx(jump_sound);
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
		if (vehicle->lights_on == false) vehicle->lights_on = true;
		else if (vehicle->lights_on == true) vehicle->lights_on = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		RestartPlayer();
	}

	if (vehicle->GetKmh() > 0) {
		vehicle->ApplyEngineForce(-200.0f);
	}
	else if (vehicle->GetKmh() < 0) {
		vehicle->ApplyEngineForce(200.0f);
	}

	if (vehicle->GetKmh() > 0 && App->input->GetKey(SDL_SCANCODE_UP) != KEY_REPEAT) {
		acceleration = -200.0f;
	}
	if (vehicle->GetKmh() < 0 && App->input->GetKey(SDL_SCANCODE_DOWN) != KEY_REPEAT) {
		acceleration = 200.0f;
	}

	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_REPEAT) {
		if (turbo > 0) {
			vehicle->body->applyCentralImpulse(vehicle->body->getWorldTransform().getBasis().getColumn(2) * 100);
			turbo--;
			vehicle->turbo_mode = true;
		}
		else {
			vehicle->turbo_mode = false;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_UP) {
		vehicle->turbo_mode = false;
	}
	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN) {
		if (turbo > 0) {
			App->audio->PlayFx(turbo_sound);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_Y) == KEY_REPEAT) {
		
		vehicle->body->applyImpulse(btVector3(0, 70, 0), btVector3(10, 0, 0));
	}

	if (vehicle->GetKmh() > 1 || vehicle->GetKmh() < -1) {
		vehicle->vehicle_running = true;
		car_velocity = vehicle->GetKmh();
	}
	else {
		vehicle->vehicle_running = false;
		car_velocity = 0;
	}

	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();

	char title[200];

	uint minutes = 0;
	uint seconds = 0;
	if (lapTime.getTimerState()) {
		minutes = (lapTime.Read() / 1000) / 60;
		seconds = (lapTime.Read() / 1000) % 60;
	}

	uint bestTime_minutes = (bestTime / 1000) / 60;
	uint bestTime_seconds = (bestTime / 1000) % 60;

	sprintf_s(title, "%s  |  Car Stats: Velocity: %.1f Km/h  Jump Charge: %d  Turbo: %d  |  Race Stats: Coins: %d Lap Time: %2d:%2d  |  Best Score: Coins: %d Best Lap Time: %2d:%2d", 
		TITLE, car_velocity, jump_charge, turbo, playerCoins, minutes, seconds, bestPlayerCoins, bestTime_minutes, bestTime_seconds);
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}



