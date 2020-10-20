#pragma once
#include <memory>
//#include <GLFW\glfw3.h> //No idea why I need this all ofa sudden fo the enums... Camera works fine
//#include "Globals.h"
#include "Camera.h"
#include "Object.h"

class Player
{
public:
	Player() 
	{
		ControlledObj = nullptr;
		camera = nullptr;
		Rotation = Quat(0.0f, 0.0f, 0.0f, 1.0f);
	}
	void SetControlledObject(std::shared_ptr<Object> Obj)
	{
		ControlledObj = Obj;
	}
	void SetCamera(std::shared_ptr<Camera> CameraIn)
	{
		camera = CameraIn;
	}
	void SetSpeed(float Speedin){Speed = Speedin;}
	void SetRotationSpeed(float Rot) {RotationSpeed = Rot;}

	void AttachCameraToObject(bool attach)
	{
		CameraAttachedToObj = attach;
	}

	std::shared_ptr<Object> GetControlledObject() { return ControlledObj; }

	void Update()
	{
		if (camera) //if not nullptr
		{
			if (ControlledObj && CameraAttachedToObj)//if not nullptr
			{
				camera->SetTarget(ControlledObj->GetTransform()->GetPosition() + ControlledObj->GetTransform()->GetForwardDirection() * 1.5);
				camera->AttachPosition(ControlledObj->GetTransform()->GetPosition() + Vec3(0.0f, 2.5f, -3.5f));
				camera->SetOrientation(ControlledObj->GetTransform()->GetForwardDirection());
				camera->SetUpDirection(ControlledObj->GetTransform()->GetUpDirection());
			}
			camera->Update();
		}
		Forward = 0.0f;
		Right = 0.0f;
		Up = 0.0f;

		int Fast = 1 + (Input::Keyboard[GLFW_KEY_LEFT_SHIFT] * 6);
		
		//absolute values from GLFW keyboard enum. Had to hard code because compiler was being weird.
		if (Input::Keyboard[GLFW_KEY_W]) { Forward = Speed * Fast; } //W
		if (Input::Keyboard[GLFW_KEY_S]) { Forward = -Speed * Fast; } //S
		if (Input::Keyboard[GLFW_KEY_A]) { Right = -Speed; } //A
		if (Input::Keyboard[GLFW_KEY_D]) { Right = Speed; } //D
		if (Input::Keyboard[GLFW_KEY_Q]) { Up = -Speed; } //Q
		if (Input::Keyboard[GLFW_KEY_E]) { Up = Speed; } //E

		//Pitch = 0.0f;
		//Yaw = 0.0f;
		//Roll = 0.0f;

		if (Input::Keyboard[GLFW_KEY_LEFT]) { Roll -= RotationSpeed; }
		if (Input::Keyboard[GLFW_KEY_RIGHT]) { Roll += RotationSpeed; }

		//std::cout << "Roll is " << Roll << std::endl;

		if (Input::Keyboard[GLFW_KEY_UP]) { Pitch += RotationSpeed; }
		if (Input::Keyboard[GLFW_KEY_DOWN]) { Pitch -= RotationSpeed; }

		if (Input::Keyboard[GLFW_KEY_KP_3]) { Yaw += RotationSpeed; }
		if (Input::Keyboard[GLFW_KEY_KP_1]) { Yaw -= RotationSpeed; }
		
		float Max = 1.0f;
		if (Pitch > Max) { Pitch = -Max; }
		if (Yaw > Max) { Yaw = -Max; }
		if (Roll > Max) { Roll = -Max; }

		if (Pitch < -Max) { Pitch = Max; }
		if (Yaw < -Max) { Yaw = Max; }
		if (Roll < -Max) { Roll = Max; }

		//Rotation = Rotation * Quat(Pitch, Yaw, Roll, Pitch * Yaw * Roll * RotationSpeed);// *inverse(Rotation);

		std::cout << "Rotation = "; Rotation.Print();

		if (ControlledObj)//if not nullptr
		{
			ControlledObj->GetPhysics()->AddForce(Vec3(Right, Up, Forward)); ///TODO, this is not right
			//ControlledObj->GetPhysics()->AddForce(Forward); ///TODO, this is not right
			ControlledObj->GetTransform()->SetRotation(Vec4(Pitch, Yaw, Roll,180.0f * DegreesToRadians)); ///TODO, this is not right
			
			//ControlledObj->GetTransform()->SetRotation(Vec3(Pitch, Yaw, Roll)); ///TODO, this is not right EULER
			//ControlledObj->GetTransform()->SetRotation(Rotation); ///TODO, this is not right QUATERION
		}
		Forward = 0.0f;
		Right = 0.0f;
		Up = 0.0f;
	}
private:
	std::shared_ptr<Object> ControlledObj;
	std::shared_ptr<Camera> camera;
	bool CameraAttachedToObj = false;
	float Forward = 0.0f, Right = 0.0f, Up = 0.0f, Pitch = 0.0f, Yaw = 0.0f, Roll = 0.0f, Speed = 1.0f, RotationSpeed = 0.05f;
	Quat Rotation;
};

