#pragma once
#include <memory>
#include "MyMaths.h"
#include "Physics.h"
class Collision
{
};

class BoundingSphere
{
public:
	BoundingSphere()
	{
		Point = Vec3();
		Radius = 1.0f;
	}
	BoundingSphere(Vec3 PointIn, float RadiusIn)
	{
		Point = PointIn;
		Radius = RadiusIn;
	}

	void SetOwnerPhysicsComponent(std::shared_ptr<Physics> PhysicsIn) { physics = PhysicsIn; }

	void SetPoint(Vec3 PointIn) { Point = PointIn; }
	Vec3 GetPoint() { return Point; }
	void SetRadius(float RadiusIn) { Radius = RadiusIn; }
	float GetRadius() { return Radius; }

	bool Colliding(BoundingSphere SphereIn) 
	{
		return Lengthsq(SphereIn.GetPoint() - Point) <= (Radius + SphereIn.GetRadius()) * (Radius + SphereIn.GetRadius());
	}

	void ApplyLinearForce(Vec3 Momentum)
	{
		//std::cout << "Force applied is "; Momentum.Print();
		physics->AddForce(Momentum);
	}

	//should momentum be something else?
	void ApplyAngularForce(Vec3 TorqueDirection, float Momentum)
	{
		physics->AddForce(TorqueDirection * Momentum);
	}

private:
	
	Vec3 Point;
	float Radius;

	std::shared_ptr<Physics> physics;

};


static bool BSpheresColliding(Vec3 Point, float Size, Vec3 OtherPoint, float OtherSize)
{
	return Lengthsq(OtherPoint - Point) <= ((Size + OtherSize) * (Size + OtherSize));
}