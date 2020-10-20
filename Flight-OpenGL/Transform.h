#pragma once
#include "MyMaths.h"
#include "Globals.h"
class Transform
{
public:
	Transform() {
		Position = Vec3();
		Scale = Vec3(1.0f);
		TransformMatrix = Matrix4();
	};
	Transform(Vec3 Posin, Vec4 AxisAngleIn, Vec3 Scalein)
	{
		Position = Posin;
		Scale = Scalein;
		TransformMatrix = Matrix4();
		MatRotateLong(TransformMatrix, AxisAngleIn.w, AxisAngleIn);
		//TransformMatrix.printMatrix();
	}

	Matrix4 GetMatrix()
	{
		return TransformMatrix;
	}
	//not used
	void SetRotation(Vec3 EulerIn)
	{
		Orientation = EulerToQuat(EulerIn);
		MatRotate(TransformMatrix, Orientation);
	}

	void AddToRotation(Quat QuatIn) 
	{
		//Debugging antons weird rotation.
		/*Quat temp = Orientation;
		std::cout << "Orientation is. "; Orientation.Print();
		Orientation = Orientation * QuatIn;
		std::cout << "Orientation after multiplication is "; Orientation.Print();
		Quat Weirdquat = AntonsMultiplyQuat(temp, QuatIn);
		std::cout << "Orientation after Antons multiplication"; Weirdquat.Print();
		Weirdquat = ParojMultiplyQuat(temp, QuatIn);
		std::cout << "Orientation after Paroj Multiplication"; Weirdquat.Print();
		std::cout << std::endl;*/
		
		Orientation = Orientation * QuatIn;
		//Orientation = AntonsMultiplyQuat(Orientation,QuatIn);
		MatRotate(TransformMatrix, Orientation);
		MatScale(TransformMatrix, Scale);
		//TransformMatrix.printMatrix();
	};

private:
	Vec3 Position, Scale;// , EulerRotation;

	Matrix4 TransformMatrix;
	
	Quat Orientation;

	//Things that work
public:

	Vec3 GetPosition() { return Position; }
	void SetPosition(Vec3 PosIn) { Position = PosIn; }
	void AddToPosition(Vec3 Posin) { 
		Position += Posin;
	}
	Quat GetOrientation() { return Orientation; }

	void SetScale(float ScaleIn)
	{
		Scale.x = ScaleIn;
		Scale.y = ScaleIn;
		Scale.z = ScaleIn;
	}

	void SetScale(Vec3 ScaleIn) { Scale = ScaleIn;	}

	Quat GetQuatRotation() { return Orientation;}

	Vec3 GetRightDirection()
	{
		return Vec3(TransformMatrix.Values.at(0), TransformMatrix.Values.at(4), TransformMatrix.Values.at(8));
	}
	Vec3 GetUpDirection()
	{
		return Vec3(TransformMatrix.Values.at(1), TransformMatrix.Values.at(5), TransformMatrix.Values.at(9));
	}
	Vec3 GetForwardDirection()
	{
		return Vec3(TransformMatrix.Values.at(2), TransformMatrix.Values.at(6), TransformMatrix.Values.at(10));
	}

	//updates passed in matrix
	void SetMatrix(Matrix4& Model)
	{
		//MatScale(Model, Scale);
		MatSetTranslate(TransformMatrix, Position);

		Model = Model * TransformMatrix; //this transform only contains rotation,
		//MatTranslate(Model, Position);
		Render::ModelMatrix = Model;
	}

};

