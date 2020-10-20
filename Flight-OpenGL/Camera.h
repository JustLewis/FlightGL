#pragma once
#include "globals.h"
#include "MyMaths.h"
#include "Transform.h"
#include <memory>

class Camera
{
public:
	Camera()
	{
		Position = Vec3(0.0f, 0.0f, -1.0f);
		UpDirection = Vec3(0.0f,1.0f, 0.0f);
		Target = Vec3(0.0f, 0.0f, 0.0f);
		TargetDirection = Vec3(0.0f, 0.0f, 1.0f);
		MountedPosition = Vec3(0.0f, 0.0f, 0.0f);
		Rotation = Quat();
		CinematicCamera = false;
		transform = nullptr;
		physics = nullptr;
		
	}
	void AttachObjectTransform(std::shared_ptr<Transform> TransformIn)
	{
		transform = TransformIn;
	}
	void AttachObjectPhysics(std::shared_ptr<Physics> PhysicsIn)
	{
		physics = PhysicsIn;
	}

	void SetTarget(Vec3 TargetLoc)
	{
		Target = TargetLoc;
	}
	void AttachPosition(Vec3 PositionIn)
	{
		MountedPosition = PositionIn;
	}
	void SetOrientation(Vec3 ForwardDirectioNIn) { TargetDirection = ForwardDirectioNIn; }

	void SetQuatRotation(Quat QuatIn) { Rotation = QuatIn; }

	void SetUpDirection(Vec3 UpDirectionIn) { UpDirection = UpDirectionIn; }

	void Update()
	{
		//TODO Tidy and Add orientation
		ProcessInput();
		if (transform)
		{
			//RelativePosition = RotateVector(transform->GetOrientation(), Position);
			RelativePosition = Position.x * transform->GetRightDirection();
			RelativePosition += Position.y * transform->GetUpDirection();
			RelativePosition += Position.z * transform->GetForwardDirection();
			//Target = transform->GetPosition();

			MountedPosition = transform->GetPosition() + RelativePosition;
			TargetDirection = transform->GetForwardDirection();
			UpDirection = transform->GetUpDirection();
		}
		//std::cout << "Relative postion is "; RelativePosition.Print();
		//Render::viewMatrix = Lookat(MountedPosition, MountedPosition + TargetDirection, UpDirection);
		MatViewMatrix(Render::viewMatrix,MountedPosition /*+ physics->GetVelocity()*/, TargetDirection, UpDirection);
		Render::shader.SetUniformMatrix4F("View", Render::viewMatrix.Values, true);
	};

	void SetSpeed(float SpeedIn)
	{
		MoveSpeed = SpeedIn;
	}

private:
	Vec3 Target, Position, MountedPosition, UpDirection, TargetDirection, TargetUpDirection, TargetRightDirection;
	float MoveSpeed = 0.10f;
	Quat Rotation;
	Vec3 RelativePosition;
	bool CinematicCamera;

	std::shared_ptr<Transform> transform;
	std::shared_ptr<Physics> physics;

	void ProcessInput()
	{
		if (Input::Keyboard[GLFW_KEY_KP_8]) { Position += Vec3(0.0f,0.0f,1.0f) * MoveSpeed; }
		if (Input::Keyboard[GLFW_KEY_KP_5]) { Position -= Vec3(0.0f, 0.0f, 1.0f) * MoveSpeed; }

		if (Input::Keyboard[GLFW_KEY_KP_4]) { Position += Vec3(1.0f, 0.0f,0.0f) * MoveSpeed; }
		if (Input::Keyboard[GLFW_KEY_KP_6]) { Position -= Vec3(1.0f, 0.0f, 0.0f) * MoveSpeed; }

		if (Input::Keyboard[GLFW_KEY_KP_9]) { Position += Vec3(0.0f,1.0f,0.0f) * MoveSpeed; }
		if (Input::Keyboard[GLFW_KEY_KP_7]) { Position -= Vec3(0.0f, 1.0f, 0.0f) * MoveSpeed; }


		//if (Input::Keyboard[GLFW_KEY_KP_2]) { Position = transform->GetPosition(); }
	}

};