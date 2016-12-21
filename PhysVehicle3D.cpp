#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Black;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}
	
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();

	//Base chasis mid
	Cube chassis_mid(info.chassis_mid_size.x, info.chassis_mid_size.y, info.chassis_mid_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis_mid.transform);
	btVector3 cmid_offset(info.chassis_mid_offset.x, info.chassis_mid_offset.y, info.chassis_mid_offset.z);
	cmid_offset = cmid_offset.rotate(q.getAxis(), q.getAngle());

	chassis_mid.transform.M[12] += cmid_offset.getX();
	chassis_mid.transform.M[13] += cmid_offset.getY();
	chassis_mid.transform.M[14] += cmid_offset.getZ();

	//Base chasis left
	Cube chassis_left(info.chassis_left_size.x, info.chassis_left_size.y, info.chassis_left_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis_left.transform);
	btVector3 cleft_offset(info.chassis_left_offset.x, info.chassis_left_offset.y, info.chassis_left_offset.z);
	cleft_offset = cleft_offset.rotate(q.getAxis(), q.getAngle());

	chassis_left.transform.M[12] += cleft_offset.getX();
	chassis_left.transform.M[13] += cleft_offset.getY();
	chassis_left.transform.M[14] += cleft_offset.getZ();
	chassis_left.color = Red;

	//Base chasis right
	Cube chassis_right(info.chassis_right_size.x, info.chassis_right_size.y, info.chassis_right_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis_right.transform);
	btVector3 cright_offset(info.chassis_right_offset.x, info.chassis_right_offset.y, info.chassis_right_offset.z);
	cright_offset = cright_offset.rotate(q.getAxis(), q.getAngle());

	chassis_right.transform.M[12] += cright_offset.getX();
	chassis_right.transform.M[13] += cright_offset.getY();
	chassis_right.transform.M[14] += cright_offset.getZ();
	chassis_right.color = Red;

	//aileron mid
	Cube aileron_mid(info.aileron_mid_size.x, info.aileron_mid_size.y, info.aileron_mid_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&aileron_mid.transform);
	btVector3 amid_offset(info.aileron_mid_offset.x, info.aileron_mid_offset.y, info.aileron_mid_offset.z);
	amid_offset = amid_offset.rotate(q.getAxis(), q.getAngle());

	aileron_mid.transform.M[12] += amid_offset.getX();
	aileron_mid.transform.M[13] += amid_offset.getY();
	aileron_mid.transform.M[14] += amid_offset.getZ();

	//aileron left
	Cube aileron_left(info.aileron_left_size.x, info.aileron_left_size.y, info.aileron_left_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&aileron_left.transform);
	btVector3 aleft_offset(info.aileron_left_offset.x, info.aileron_left_offset.y, info.aileron_left_offset.z);
	aleft_offset = aleft_offset.rotate(q.getAxis(), q.getAngle());

	aileron_left.transform.M[12] += aleft_offset.getX();
	aileron_left.transform.M[13] += aleft_offset.getY();
	aileron_left.transform.M[14] += aleft_offset.getZ();
	aileron_left.color = Red;

	//aileron right
	Cube aileron_right(info.aileron_right_size.x, info.aileron_right_size.y, info.aileron_right_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&aileron_right.transform);
	btVector3 aright_offset(info.aileron_right_offset.x, info.aileron_right_offset.y, info.aileron_right_offset.z);
	aright_offset = aright_offset.rotate(q.getAxis(), q.getAngle());

	aileron_right.transform.M[12] += aright_offset.getX();
	aileron_right.transform.M[13] += aright_offset.getY();
	aileron_right.transform.M[14] += aright_offset.getZ();
	aileron_right.color = Red;

	//up part of the cassis 
	Cube upPart_mid(info.upPart_mid_size.x, info.upPart_mid_size.y, info.upPart_mid_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&upPart_mid.transform);
	btVector3 upmid_offset(info.upPart_mid_offset.x, info.upPart_mid_offset.y, info.upPart_mid_offset.z);
	upmid_offset = upmid_offset.rotate(q.getAxis(), q.getAngle());

	upPart_mid.transform.M[12] += upmid_offset.getX();
	upPart_mid.transform.M[13] += upmid_offset.getY();
	upPart_mid.transform.M[14] += upmid_offset.getZ();

	//up part of the cassis 
	Cube upPart_left(info.upPart_left_size.x, info.upPart_left_size.y, info.upPart_left_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&upPart_left.transform);
	btVector3 upleft_offset(info.upPart_left_offset.x, info.upPart_left_offset.y, info.upPart_left_offset.z);
	upleft_offset = upleft_offset.rotate(q.getAxis(), q.getAngle());

	upPart_left.transform.M[12] += upleft_offset.getX();
	upPart_left.transform.M[13] += upleft_offset.getY();
	upPart_left.transform.M[14] += upleft_offset.getZ();
	upPart_left.color = Red;

	//up part of the cassis 
	Cube upPart_right(info.upPart_right_size.x, info.upPart_right_size.y, info.upPart_right_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&upPart_right.transform);
	btVector3 upright_offset(info.upPart_right_offset.x, info.upPart_right_offset.y, info.upPart_right_offset.z);
	upright_offset = upright_offset.rotate(q.getAxis(), q.getAngle());

	upPart_right.transform.M[12] += upright_offset.getX();
	upPart_right.transform.M[13] += upright_offset.getY();
	upPart_right.transform.M[14] += upright_offset.getZ();
	upPart_right.color = Red;

	//exhaust pipe left
	Cube exhaust_pipe_left(info.exhaust_pipe_left_size.x, info.exhaust_pipe_left_size.y, info.exhaust_pipe_left_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&exhaust_pipe_left.transform);
	btVector3 expipeleft_offset(info.exhaust_pipe_left_offset.x, info.exhaust_pipe_left_offset.y, info.exhaust_pipe_left_offset.z);
	expipeleft_offset = expipeleft_offset.rotate(q.getAxis(), q.getAngle());

	exhaust_pipe_left.transform.M[12] += expipeleft_offset.getX();
	exhaust_pipe_left.transform.M[13] += expipeleft_offset.getY();
	exhaust_pipe_left.transform.M[14] += expipeleft_offset.getZ();

	//exhaust pipe right
	Cube exhaust_pipe_right(info.exhaust_pipe_right_size.x, info.exhaust_pipe_right_size.y, info.exhaust_pipe_right_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&exhaust_pipe_right.transform);
	btVector3 expiperight_offset(info.exhaust_pipe_right_offset.x, info.exhaust_pipe_right_offset.y, info.exhaust_pipe_right_offset.z);
	expiperight_offset = expiperight_offset.rotate(q.getAxis(), q.getAngle());

	exhaust_pipe_right.transform.M[12] += expiperight_offset.getX();
	exhaust_pipe_right.transform.M[13] += expiperight_offset.getY();
	exhaust_pipe_right.transform.M[14] += expiperight_offset.getZ();

	//up part window
	Cube window(info.window_size.x, info.window_size.y, info.window_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&window.transform);
	btVector3 win_offset(info.window_offset.x, info.window_offset.y, info.window_offset.z);
	win_offset = win_offset.rotate(q.getAxis(), q.getAngle());

	window.transform.M[12] += win_offset.getX();
	window.transform.M[13] += win_offset.getY();
	window.transform.M[14] += win_offset.getZ();
	window.color = Black;

	Sphere leftFocus(0.2f);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&leftFocus.transform);
	btVector3 lfocus_offset(info.left_focus_offset.x, info.left_focus_offset.y, info.left_focus_offset.z);
	lfocus_offset = lfocus_offset.rotate(q.getAxis(), q.getAngle());

	leftFocus.transform.M[12] += lfocus_offset.getX();
	leftFocus.transform.M[13] += lfocus_offset.getY();
	leftFocus.transform.M[14] += lfocus_offset.getZ();

	Sphere rightFocus(0.2f);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&rightFocus.transform);
	btVector3 rfocus_offset(info.right_focus_offset.x, info.right_focus_offset.y, info.right_focus_offset.z);
	rfocus_offset = rfocus_offset.rotate(q.getAxis(), q.getAngle());

	rightFocus.transform.M[12] += rfocus_offset.getX();
	rightFocus.transform.M[13] += rfocus_offset.getY();
	rightFocus.transform.M[14] += rfocus_offset.getZ();
	if (lights_on) {
		leftFocus.color = Yellow;
		rightFocus.color = Yellow;
	}

	Cube gasParticlesLeft(0.1f, 0.1f, 0.1f);

	for (float i = 0; i < 4; i++) {
		vehicle->getChassisWorldTransform().getOpenGLMatrix(&gasParticlesLeft.transform);
		btVector3 gasParticleLeft_offset(info.exhaust_pipe_left_offset.x, info.exhaust_pipe_left_offset.y, (info.exhaust_pipe_left_offset.z - 1) - i / 6);
		gasParticleLeft_offset = gasParticleLeft_offset.rotate(q.getAxis(), q.getAngle());

		gasParticlesLeft.transform.M[12] += gasParticleLeft_offset.getX();
		gasParticlesLeft.transform.M[13] += gasParticleLeft_offset.getY();
		gasParticlesLeft.transform.M[14] += gasParticleLeft_offset.getZ();

		if (!turbo_mode) {
			gasParticlesLeft.color = Grey;
		}
		else {
			gasParticlesLeft.color = Red;
		}
		if (vehicle_running) {
			gasParticlesLeft.Render();
		}
	}

	Cube gasParticlesRight(0.1f, 0.1f, 0.1f);

	for (float i = 0; i < 4; i++) {
		vehicle->getChassisWorldTransform().getOpenGLMatrix(&gasParticlesRight.transform);
		btVector3 gasParticleRight_offset(info.exhaust_pipe_right_offset.x, info.exhaust_pipe_right_offset.y, (info.exhaust_pipe_right_offset.z - 1) - i / 6);
		gasParticleRight_offset = gasParticleRight_offset.rotate(q.getAxis(), q.getAngle());

		gasParticlesRight.transform.M[12] += gasParticleRight_offset.getX();
		gasParticlesRight.transform.M[13] += gasParticleRight_offset.getY();
		gasParticlesRight.transform.M[14] += gasParticleRight_offset.getZ();

		if (!turbo_mode) {
			gasParticlesRight.color = Grey;
		}
		else {
			gasParticlesRight.color = Red;
		}
		if (vehicle_running) {
			gasParticlesRight.Render();
		}
	}

	Cube speed_lines(0.02f, 0.02f,3);
	float speed_lines_x = info.chassis_left_offset.x - 0.8f;
	float speed_lines_y = info.chassis_left_offset.y - 0.8f;
	float speed_lines_z = info.chassis_left_offset.z - 1;

	for (int i = 0; i < 3; i++) {
		vehicle->getChassisWorldTransform().getOpenGLMatrix(&speed_lines.transform);
		btVector3 speed_lines_offset(speed_lines_x, speed_lines_y, speed_lines_z);
		speed_lines_offset = speed_lines_offset.rotate(q.getAxis(), q.getAngle());

		speed_lines.transform.M[12] += speed_lines_offset.getX();
		speed_lines.transform.M[13] += speed_lines_offset.getY();
		speed_lines.transform.M[14] += speed_lines_offset.getZ();

		if (turbo_mode) {
			speed_lines.Render();
		}

		speed_lines_x = info.chassis_left_offset.x - 0.8f;
		speed_lines_y = info.chassis_left_offset.y + i;
		speed_lines_z = info.chassis_left_offset.z;
	}

	Cube speed_lines_left(0.02f, 0.02f, 3);
	float speed_lines_left_x = info.chassis_right_offset.x + 0.8f;
	float speed_lines_left_y = info.chassis_right_offset.y - 0.8f;
	float speed_lines_left_z = info.chassis_right_offset.z - 1;

	for (int i = 0; i < 3; i++) {
		vehicle->getChassisWorldTransform().getOpenGLMatrix(&speed_lines_left.transform);
		btVector3 speed_lines_left_offset(speed_lines_left_x, speed_lines_left_y, speed_lines_left_z);
		speed_lines_left_offset = speed_lines_left_offset.rotate(q.getAxis(), q.getAngle());

		speed_lines_left.transform.M[12] += speed_lines_left_offset.getX();
		speed_lines_left.transform.M[13] += speed_lines_left_offset.getY();
		speed_lines_left.transform.M[14] += speed_lines_left_offset.getZ();

		if (turbo_mode) {
			speed_lines_left.Render();
		}

		speed_lines_left_x = info.chassis_right_offset.x + 0.8f;
		speed_lines_left_y = info.chassis_right_offset.y + i;
		speed_lines_left_z = info.chassis_right_offset.z;
	}

	chassis_mid.Render();
	chassis_left.Render();
	chassis_right.Render();
	aileron_mid.Render();
	aileron_left.Render();
	aileron_right.Render();
	upPart_mid.Render();
	upPart_left.Render();
	upPart_right.Render();
	exhaust_pipe_left.Render();
	exhaust_pipe_right.Render();
	window.Render();
	leftFocus.Render();
	rightFocus.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}
