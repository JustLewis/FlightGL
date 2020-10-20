#pragma once
#include "MyMaths.h"
#include <memory>
#include "Transform.h"
class Physics
{
public:
	Physics()
	{
		Mass = 1.0f;
		Inertia = 1.0f;
		Velocity = Vec3();
		transform = nullptr;
	}

	Physics(float Weightin, Vec3 Velocityin, std::shared_ptr<Transform> OwnersTransform)
	{
		Mass = Weightin;
		Velocity = Velocityin;
		transform = OwnersTransform;
	}
	void SetMass(float MassIn)
	{
		Mass = MassIn;
	}
	void SetInertia(float InertiaIn)
	{
		Inertia = InertiaIn;
	}

	void SetMassAndInertia(float MassIn, float InertiaIn)
	{
		Mass = MassIn;
		Inertia = InertiaIn;
	}

	void SetOwnerTransformComponent(std::shared_ptr<Transform> TransformIn)
	{
		transform = TransformIn;
	}

	//TODO temp, make this properly
	void AddForce(Vec3 Force)
	{
		Velocity += (Force / Mass) * Utility::DeltaTime;
	}

	void AddTorque(Vec3 TorqueIn)
	{
		AngularVelocity += (TorqueIn / Inertia) * Utility::DeltaTime;
	}


	void Update()
	{
		//Gravity(); //only want gravity affecting the player for now
		//Velocity *= 0.99f; //temp friction
		transform->AddToPosition(Velocity * Utility::DeltaTime);

		//AngularVelocity *= 0.9f; //temp resistance	
		transform->AddToRotation(EulerToQuat(AngularVelocity));
	}

	float GetSpeed() { return Length(Velocity); }
	Vec3 GetVelocity() { return Velocity; }
	void SetVelocity(Vec3 VelocityIn) { Velocity = VelocityIn; }
	void SetTorque(Vec3 TorqueIn) { Torque = TorqueIn; }
	Vec3 GetVelocityDirection() { return Normalise(Velocity); }

	//angular direction
	Vec3 GetAngularVelocity() { return AngularVelocity; }
	float GetAngularMomentum(){ return Inertia * Length(AngularVelocity); }

	float GetMomentum() { return Mass * Length(Velocity); }
	Vec3 GetMomentumAndDirection() { return Mass * Velocity; }

	void Gravity()
	{
		//http://physics.weber.edu/amiri/physics1010online/WSUonline12w/OnLineCourseMovies/CircularMotion&Gravity/reviewofgravity/ReviewofGravity.html
		Vec3 PlayerToPlanet = transform->GetPosition() - Game::PlanetPosition;
		Vec3 PlayerToPlanetLength = Length(PlayerToPlanet);

		//Vec3 GravityForce = (1 * Mass * Game::PlanetMass) / (PlayerToPlanetLength * PlayerToPlanetLength); //modified for nice intro
		Vec3 GravityForce = (100 * Mass * Game::PlanetMass) / (PlayerToPlanetLength * PlayerToPlanetLength); //original
		Vec3 GravityDirection = Normalise(PlayerToPlanet);
		std::cout << "Gravity is "; GravityForce.Print();// << Gravity << std::endl;

		Velocity -= GravityDirection * GravityForce * Utility::DeltaTime;
	}
	float GetMass() { return Mass; }

private:

	float Mass, Inertia;
	Vec3 Torque;
	Vec3 AngularVelocity;
	Vec3 Velocity;
	std::shared_ptr<Transform> transform;
	//collision?


};

