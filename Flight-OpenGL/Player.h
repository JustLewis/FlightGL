#pragma once
#include <memory>
#include <vector>
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
		ForwardInput = 0.0f;
		RightInput = 0.0f;
		UpInput = 0.0f;
	}
	void SetControlledObject(std::shared_ptr<Object> Obj)
	{
		ControlledObj = Obj;
	}
	void SetCamera(std::shared_ptr<Camera> CameraIn)
	{
		camera = CameraIn;
		if (ControlledObj)
		{
			camera->AttachObjectTransform(ControlledObj->GetTransform());
			camera->AttachObjectPhysics(ControlledObj->GetPhysics());
		}
	}
	void SetSpeed(float Speedin) { Speed = Speedin; }
	void SetRotationSpeed(float Rot) { RotationSpeed = Rot; }

	void AttachCameraToObject(bool attach)
	{
		CameraAttachedToObj = attach;
	}

	std::shared_ptr<Object> GetControlledObject() { return ControlledObj; }

	void Update()
	{
		Forward = 0.0f;	Right = 0.0f; Up = 0.0f;
		Pitch = 0.0f;	Yaw = 0.0f;	Roll = 0.0f;

		UpdateCamera();
		ProcessInputs();

		if (ControlledObj)//if not nullptr
		{	
			ControlledObj->GetPhysics()->Gravity();

			auto transform = ControlledObj->GetTransform();
			Vec3 RelativeThrust = Right * transform->GetRightDirection();
			RelativeThrust += Up * transform->GetUpDirection();
			RelativeThrust += Forward * transform->GetForwardDirection();
			
			ControlledObj->GetPhysics()->AddForce(RelativeThrust);

			//Make pitch roll and yaw relative to direction
			RelativeRotation = Roll * transform->GetForwardDirection();
			RelativeRotation += Pitch * transform->GetRightDirection();
			RelativeRotation += Yaw * transform->GetUpDirection();

			ControlledObj->GetPhysics()->AddTorque(RelativeRotation);

			//Child objects
			//ControlledObj->GetSubObject(0)->GetPhysics()->AddTorque(Vec3(0.0f,1.0f,0.0f));
			//ControlledObj->GetSubObject(0)->GetTransform()->SetPosition(Vec3(-1.5f, 1.0f, 5.0f));
			//ControlledObj->GetSubObject(1)->GetPhysics()->AddTorque(-Vec3(0.0f, 1.0f, 0.0f));
			//ControlledObj->GetSubObject(1)->GetTransform()->SetPosition(Vec3(1.5f, 1.0f, 5.0f));

		}
	}
private:
	std::shared_ptr<Object> ControlledObj;
	std::shared_ptr<Camera> camera;
	bool CameraAttachedToObj = false;
	float Forward = 0.0f, Right = 0.0f, Up = 0.0f, Speed = 1.0f, RotationSpeed = 0.05f, Pitch = 0.0f, Yaw = 0.0f, Roll = 0.0f;
	float ForwardInput, RightInput, UpInput;
	int Fast;
	Vec3 RelativeRotation;

	Quat Rotation;

	void UpdateCamera()
	{
		if (camera) //if not nullptr
		{
			if (ControlledObj && CameraAttachedToObj)//if not nullptr
			{
				//camera->SetTarget(ControlledObj->GetTransform()->GetPosition() + ControlledObj->GetPhysics()->GetVelocity());
				//camera->AttachPosition(ControlledObj->GetTransform()->GetPosition() + Vec3(0.0f, 2.5f, -3.5f));
				//camera->SetOrientation(ControlledObj->GetTransform()->GetForwardDirection());
				//camera->SetUpDirection(ControlledObj->GetTransform()->GetUpDirection());
				//camera->SetQuatRotation(ControlledObj->GetTransform()->GetQuatRotation());
			}
			camera->Update();
		}
	}
	void ProcessInputs()
	{
		Fast = 1 + (Input::Keyboard[GLFW_KEY_LEFT_SHIFT] * 5);

		ForwardInput *= 0.9f;
		RightInput *= 0.9f;
		UpInput *= 0.9f;

		if (Input::Keyboard[GLFW_KEY_W]) { ForwardInput += 0.05f; }
		if (Input::Keyboard[GLFW_KEY_S]) { ForwardInput -= 0.05f; }
		if (Input::Keyboard[GLFW_KEY_A]) { RightInput += 0.05f; }
		if (Input::Keyboard[GLFW_KEY_D]) { RightInput -= 0.05f; }
		if (Input::Keyboard[GLFW_KEY_Q]) { UpInput -= 0.05f; }
		if (Input::Keyboard[GLFW_KEY_E]) { UpInput += 0.05f; }

		Forward = ForwardInput * Speed * Fast;
		Right = RightInput * Speed;
		Up = UpInput * Speed;

		//slow rotation
		if (Input::Keyboard[GLFW_KEY_KP_2])
		{ 
			ControlledObj->GetPhysics()->AddTorque(-ControlledObj->GetPhysics()->GetAngularVelocity() * 3000); 
		}
		//slow translation
		if (Input::Keyboard[GLFW_KEY_KP_0])
		{
			ControlledObj->GetPhysics()->AddForce(-ControlledObj->GetPhysics()->GetVelocity() * 3000);
		}

		if (Input::Keyboard[GLFW_KEY_LEFT]) { Roll += RotationSpeed; }
		if (Input::Keyboard[GLFW_KEY_RIGHT]) { Roll -= RotationSpeed; }

		if (Input::Keyboard[GLFW_KEY_UP]) { Pitch -= RotationSpeed; }
		if (Input::Keyboard[GLFW_KEY_DOWN]) { Pitch += RotationSpeed; }

		if (Input::Keyboard[GLFW_KEY_KP_3]) { Yaw += RotationSpeed; }
		if (Input::Keyboard[GLFW_KEY_KP_1]) { Yaw -= RotationSpeed; }



	}
};

