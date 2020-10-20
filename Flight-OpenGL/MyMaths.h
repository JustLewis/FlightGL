#pragma once
#include <iostream>

#include <glad/glad.h>
#include <vector>
#include <math.h>

struct Vec2
{
	Vec2(const float Xin, const float Yin) { x = Xin; y = Yin; }
	Vec2() { x = 0.0f; y = 0.f; }
	Vec2(const float ValueIn) { x = ValueIn; y = ValueIn; }
	float x = 0.0f;
	float y = 0.0f;

	virtual void Print() { std::cout << " = " << x << ", y = " << y << std::endl;}
};

struct Vec3 : public Vec2
{
	Vec3(const float Xin, const float Yin, const float Zin) : Vec2(Xin,Yin)
	{z = Zin;}
	Vec3() : Vec2 (){ z = 0; }
	Vec3(const float ValueIn) : Vec2(ValueIn) { z = ValueIn; }

	float z = 0.0f;

	void Print() override { std::cout << " = " << x << ", y = " << y << ", z = " << z << std::endl; }
	
	Vec3& operator+=(const Vec3& B)
	{
		this->x += B.x; this->y += B.y; this->z += B.z;
		return *this;
	}
	Vec3& operator*=(const float& B)
	{
		this->x *= B; this->y *= B; this->z *= B;
		return *this;
	}
	Vec3& operator-=(const Vec3& B)
	{
		this->x -= B.x; this->y -= B.y; this->z -= B.z;
		return *this;
	}
	Vec3& operator-()
	{
		this->x *= -1.0f; this->y *= -1.0f; this->z *= -1.0f;
		return *this;
	}

};

struct Vec4 : public Vec3
{
	Vec4(const float Xin, const float Yin, const float Zin, const float Win) : Vec3(Xin, Yin, Zin) { w = Win; }
	Vec4() : Vec3() { w = 0.0f; }
	Vec4(const float ValueIn) : Vec3(ValueIn) { w = ValueIn;}
	float w = 0.0f;

	virtual void Print() override { std::cout << "x = " << x << ", y = " << y << ", z = " << z << ", w = " << w << std::endl; }
};

struct Quat : public Vec4
{
	Quat(const float Xin, const float Yin, const float Zin, const float Win) 
	{
		x = Xin;
		y = Yin;
		z = Zin;
		w = Win;
	};

	//ONLY for axis angle to quat
	Quat(Vec4 AxisAngleIn)
	{
			float SinHalfAngle = sin(AxisAngleIn.w * 0.5f);
			x = AxisAngleIn.x * SinHalfAngle;
			y = AxisAngleIn.x * SinHalfAngle;
			z = AxisAngleIn.x * SinHalfAngle;
			w = cos(SinHalfAngle);
	};
	//Use to construct a vector to be rotated by quat, stored in the quat
	Quat(const float Xin, const float Yin, const float Zin)
	{
		x = Xin;
		y = Yin;
		z = Zin;
		w = 0.0f;
	}
	Quat() : Vec4() { w = 1.0f; };
	Quat(Vec3 VecIn) { x = VecIn.x; y = VecIn.y; z = VecIn.z; w = 0; }

	Vec3 GetAxis() { return Vec3(x, y, z); }

	Quat& operator+=(const Quat b)
	{
		this->x += b.x;
		this->y += b.y;
		this->z += b.z;
		this->w += b.w;
		return *this;
	}
	/*Quat& operator*=(const Quat b)
	{
		Vec3 av = Vec3(this->x, this->y, this->z);
		Vec3 bv = Vec3(b.x, b.y, b.z);

		float Wa;
		Vec3 SinComponent;

		Wa = b.w * this->w - Dot(bv, av);
		SinComponent = bv * this->w + av * b.w + CrossProduct(av, bv);

		this->x = SinComponent.x;
		this->y = SinComponent.y;
		this->z = SinComponent.z;
		this->w = Wa;
		return *this;
	}*/
	
};

enum MatrixType
{
	No_Values,
	Identity,
	Zero,
	ThreeByThree,
};

struct Matrix4
{
	Matrix4(MatrixType Typein);
	Matrix4();
	std::vector<GLfloat> Values;

	void printMatrix()
	{
		for (size_t i = 0; i < 4; i++)
		{
			std::cout << "Line " << i << ": ";
			for (size_t j = 0; j < 4; j++)
			{
				if (i * 4 + j > Values.size() - 1) { break;}
				std::cout << Values.at(i * 4 + j) << ", ";
			}
			std::cout << std::endl;
		}

		for (size_t i = 16; i < Values.size(); i++)
		{
			std::cout << "Left over in array :" << Values.at(i) << std::endl;
		}
	}
	void printMatrix3()
	{
		for (size_t i = 0; i < 3; i++)
		{
			std::cout << "Line " << i << ": ";
			for (size_t j = 0; j < 3; j++)
			{
				if (i * 3 + j > Values.size() - 1) { break; }
				std::cout << Values.at(i * 3 + j) << ", ";
			}
			std::cout << std::endl;
		}

		for (size_t i = 16; i < Values.size(); i++)
		{
			std::cout << "Left over in array :" << Values.at(i) << std::endl;
		}
	}
	Matrix4& operator*=(const Matrix4 B)
	{
		Matrix4 RetM = Matrix4(MatrixType::No_Values);

		for (int Collumn = 0; Collumn < 4; Collumn++)
		{
			for (int Row = 0; Row < 4; Row++)
			{
				float Value = 0.0f;
				for (int Target = 0; Target < 4; Target++)
				{
					Value += this->Values.at(Row * 4 + Target) * B.Values.at(Target * 4 + Collumn);
				}
				RetM.Values.push_back(Value);
			}
		}
		return *this;
	}
};



	const float DegreesToRadians = 3.14159265359f / 180.0f;
	const float RadiansToDegrees = 180.0f / 3.14159265359f;

#pragma region Vec2

		static float Vector2ToAngle(Vec2 Vecin, bool ToDegrees)
		{
			/* Tan theta = Opposite over adjacent
			*
			* so inverse tan of the opposite over the adjacent = theta. That's what's happening below.
			*/
			return atan2(Vecin.x, Vecin.y) * (ToDegrees? RadiansToDegrees : 1.0f);
		}

		static Vec2 AngleToVector2(float Degrees)
		{
			//simple trig
			return Vec2(cos(Degrees), sin(Degrees));
		}

		static Vec2 operator+(Vec2 A, Vec2 B)
		{
			return Vec2(A.x + B.x, A.y + B.y);
		}
		static Vec2 operator-(Vec2 A, Vec2 B)
		{
			return Vec2(A.x - B.x, A.y - B.y);
		}
		static Vec2 operator*(Vec2 A, Vec2 B)
		{
			return Vec2(A.x * B.x, A.y * B.y);
		}
		static Vec2 operator/(Vec2 A, Vec2 B)
		{
			return Vec2(A.x / B.x, A.y / B.y);
		}
#pragma endregion

#pragma region Vec3
		static Vec3 operator+(Vec3 A, Vec3 B)
		{
			return Vec3(A.x + B.x, A.y + B.y, A.z + B.z);
		}
		static Vec3 operator-(Vec3 A, Vec3 B)
		{
			return Vec3(A.x - B.x, A.y - B.y, A.z - B.z);
		}
		static Vec3 operator*(Vec3 A, Vec3 B)
		{
			return Vec3(A.x * B.x, A.y * B.y, A.z * B.z);
		}
		static Vec3 operator/(Vec3 A, Vec3 B)
		{
			return Vec3(A.x / B.x, A.y / B.y, A.z / B.z);
		}
		
		static Vec3 EulerToDirection(Vec3 EulerAngleIn)
		{
			Vec3 ReturnVec = Vec3(0.0f, 0.0f, 0.0f);
			EulerAngleIn.x = -EulerAngleIn.x;

			ReturnVec.x = (cos(EulerAngleIn.y)) * (cos(EulerAngleIn.x));
			ReturnVec.y = sin(EulerAngleIn.x);
			ReturnVec.z = (cos(EulerAngleIn.x)) * (sin(EulerAngleIn.y));

			return ReturnVec;
		}

		static Vec3 Interpolate(Vec3 a, Vec3 b, float t)
		{
			return (1 - t) * a + t * b;
		}
		static float Interpolate(float a, float b, float t)
		{
			return (1 - t) * a + t * b;
		}

		static Vec3 CrossProduct(Vec3 A, Vec3 B)
		{
			float x = A.y * B.z - A.z * B.y;
			float y = A.z * B.x - A.x * B.z;
			float z = A.x * B.y - A.y * B.x;

			return Vec3(x, y, z);
		}

		static float Length(Vec3 A)
		{
			return sqrt(A.x * A.x + A.y * A.y + A.z * A.z);
		}
		static float Lengthsq(Vec3 A)
		{
			return A.x * A.x + A.y * A.y + A.z * A.z;
		}
		static Vec3 Normalise(Vec3 A)
		{
			float Mag = Length(A);
			return Vec3(A.x / Mag, A.y / Mag, A.z / Mag);
		}

		static float Dot(Vec3 A, Vec3 B)
		{
			return A.x * B.x + A.y * B.y + A.z * B.z;
		}

#pragma endregion

#pragma region Vec4
		static Vec4 operator+(Vec4 A, Vec4 B)
		{
			return Vec4(A.x + B.x, A.y + B.y, A.z + B.z, A.w + B.w);
		}
		static Vec4 operator-(Vec4 A, Vec4 B)
		{
			return Vec4(A.x - B.x, A.y - B.y, A.z - B.z, A.w - B.w);
		}
		static Vec4 operator*(Vec4 A, Vec4 B)
		{
			return Vec4(A.x * B.x, A.y * B.y, A.z * B.z, A.w * B.w);
		}
		static Vec4 operator/(Vec4 A, Vec4 B)
		{
			return Vec4(A.x / B.x, A.y / B.y, A.z / B.z, A.w / B.w);
		}

#pragma endregion

#pragma region Quaternions

		static Quat operator*(const Quat a, const Quat b)
		{
			Vec3 av = Vec3(a.x, a.y, a.z);
			Vec3 bv = Vec3(b.x, b.y, b.z);

			float w;
			Vec3 SinComponent;

			w = b.w * a.w - Dot(bv, av);
			SinComponent = bv * a.w + av * b.w + CrossProduct(av, bv);

			return Quat(SinComponent.x, SinComponent.y, SinComponent.z, w);
		}

		//wrong and not as understandable - from Antons OpenGL 4 tutorials
		//This is applying multiple axis of rotation when only one axis is applied.. why?.
		static Quat AntonsMultiplyQuat(const Quat r, const Quat s)
		{
			Quat result = Quat();
			result.w = s.x * r.x - s.y * r.y - s.z * r.z - s.w * r.w;
			result.x = s.x * r.y + s.y * r.x - s.z * r.w + s.w * r.z; //this is actually z
			result.y = s.x * r.z + s.y * r.w + s.z * r.x - s.w * r.y;
			result.z = s.x * r.w - s.y * r.z + s.z * r.y + s.w * r.x; //this is actually x
			
			//Normalise(result); //why normalise it?
			return result;
		}

		//another tutorials for quaternion multiplication. https://paroj.github.io/gltut/Positioning/Tut08%20Quaternions.html
		static Quat ParojMultiplyQuat(const Quat a, const Quat b)
		{
			Quat result = Quat();
			result.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
			result.y = a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z;
			result.z = a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x;
			result.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
			return result;
		}

		static Quat operator+(const Quat a, const Quat b)
		{
			return Quat(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
		}


		static Quat AxisAngletoQuat(Vec4 AxisAngleIn)
		{
			Quat Ret;
			float SinA = sin(AxisAngleIn.w / 2);
			Ret.w = cos(AxisAngleIn.w / 2);
			Ret.x = AxisAngleIn.x * SinA;
			Ret.y = AxisAngleIn.y * SinA;
			Ret.z = AxisAngleIn.z * SinA;
			
			return Ret;
		}
		static Quat AxisAngletoQuat(float Angle, Vec3 Axis)
		{
			Quat Ret;
			float SinA = sin(Angle / 2);
			Ret.w = cos(Angle / 2);
			Ret.x = Axis.x * SinA;
			Ret.y = Axis.y * SinA;
			Ret.z = Axis.z * SinA;
			return Ret;
		}

		static Quat Normalise(Quat& TheQuat)
		{
			float Length = sqrt(TheQuat.x * TheQuat.x + TheQuat.y * TheQuat.y + TheQuat.z * TheQuat.z + TheQuat.w * TheQuat.w);
			Quat RetQuat = Quat(TheQuat.x / Length, TheQuat.y / Length, TheQuat.z / Length, TheQuat.w / Length);
			return RetQuat;
		}

		static Vec4 QuatToAxisAngle(Quat Q)
		{//https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle/index.htm

			Quat TheQuat = Q;

			if (TheQuat.w > 1) { TheQuat = Normalise(TheQuat); }

			float angle = acosf(TheQuat.w) * 2;

			float AxisAmount = sqrt(1 - TheQuat.w * TheQuat.w);

			if (AxisAmount < 0.001)
			{
				float x = TheQuat.x;
				float y = TheQuat.y;
				float z = TheQuat.z;
				return Vec4(x, y, z, angle);
			}
			else
			{
				float x = TheQuat.x / AxisAmount;
				float y = TheQuat.y / AxisAmount;
				float z = TheQuat.z / AxisAmount;
				return Vec4(x, y, z, angle);
			}

			std::cout << "error in QuatToAxisAngle, should not have got this far in function.";
			return Vec4();
		}

		static Quat inverse(Quat q)
		{
			Quat RetQuat = Quat();
			RetQuat.x = -q.x;
			RetQuat.y = -q.y;
			RetQuat.z = -q.z;
			RetQuat.w = q.w;
			return RetQuat;
		}

		//TODO this is a bit of a mess. 
		static Quat Slerp(Quat qa, Quat qb, float t)
		{
			if (t < 0.0f) t = 0.0f;
			if (t > 1.0f) t = 1.0f;

			Quat qc = qb * inverse(qa);
			Vec4 AAngle = QuatToAxisAngle(qc);
			Quat ret = AxisAngletoQuat(AAngle);
			ret.w *= t;

			return ret;
		}

		static Vec3 RotateVector(Quat qa, Vec3 va)
		{
			Quat Ret = Quat(va);
			Ret = qa * Ret * inverse(qa);
			//Ret = Ret * inverse(qa);

			return Ret.GetAxis();
		}

		//second try from torque lecture
		static Quat EulerToQuat(Vec3 EulerIn)
		{
			float Magnitude = Length(EulerIn);
			float HalfMagnitude = Magnitude / 2;
			Quat RetQuat = Quat();
			if (Magnitude != 0.0f)
			{
				RetQuat.x = sin(HalfMagnitude) * (EulerIn.x / Magnitude);
				RetQuat.y = sin(HalfMagnitude) * (EulerIn.y / Magnitude);
				RetQuat.z = sin(HalfMagnitude) * (EulerIn.z / Magnitude);
				RetQuat.w = cos(HalfMagnitude);
				return RetQuat;
			}
			else
			{
				return Quat(0.0f, 0.0f, 0.0f, 1.0f);
			}
		}

		//First attempt is rubbish.
		//static Quat EulerToQuat(Vec3 EulerIn)
		//{
		//	Vec4 AAPitch = Vec4(1.0f, 0.0f, 0.0f, sin(EulerIn.x));
		//	Vec4 AAYaw = Vec4(0.0f, 1.0f, 0.0f, EulerIn.y);
		//	Vec4 AARoll = Vec4(0.0f, 0.0f, 1.0f, EulerIn.z);

		//	Quat QP = AxisAngletoQuat(AAPitch);
		//	Quat QY = AxisAngletoQuat(AAYaw);
		//	Quat QZ = AxisAngletoQuat(AARoll);

		//	//Quat Ret = QP * QY;
		//	return Quat((QP * QZ) * QY);
		//}
		
		//http://www.opengl-tutorial.org/assets/faq_quaternions/index.html#Q60
		//static Quat EulerToQuat(Vec3 EulerIn)
		//{
		//	//these were wrong
		//	const float fSinPitch(sin(EulerIn.y * 0.5F)); //actuall yaw
		//	const float fCosPitch(cos(EulerIn.y * 0.5F));
		//	const float fSinYaw(sin(EulerIn.z * 0.5F)); ///actually roll
		//	const float fCosYaw(cos(EulerIn.z * 0.5F));
		//	const float fSinRoll(sin(EulerIn.x * 0.5F)); // actually pitch
		//	const float fCosRoll(cos(EulerIn.x * 0.5F));
		//	const float fCosPitchCosYaw(fCosPitch * fCosYaw);
		//	const float fSinPitchSinYaw(fSinPitch * fSinYaw);

		//	const float	X = fSinRoll * fCosPitchCosYaw - fCosRoll * fSinPitchSinYaw;
		//	const float	Y = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fCosPitch * fSinYaw;
		//	const float	Z = fCosRoll * fCosPitch * fSinYaw - fSinRoll * fSinPitch * fCosYaw;
		//	const float	W = fCosRoll * fCosPitchCosYaw + fSinRoll * fSinPitchSinYaw;

		//	return Quat(X, Y, Z, W);
		//}
#pragma endregion

#pragma region Matrix functions
		

		static Matrix4 operator*(const Matrix4 A, const Matrix4 B)
		{
			Matrix4 RetM = Matrix4(MatrixType::No_Values);

			for (int Collumn = 0; Collumn < 4; Collumn++)
			{
				for (int Row = 0; Row < 4; Row++)
				{
					float Value = 0.0f;
					for (int Target = 0; Target < 4; Target++)
					{
						Value += A.Values.at(Collumn * 4 + Target) * B.Values.at(Target * 4 + Row);
					}
					RetM.Values.push_back(Value);
				}
			}
			return RetM;
		}
		static Vec4 operator* (const Matrix4 A, const Vec4 B)
		{
			Vec4 RetVec = Vec4(0.0f, 0.0f, 0.0f, 0.0f);

			RetVec.x = A.Values[0] * B.x + A.Values[1] * B.y + A.Values[2] * B.z + A.Values[3] * B.w;
			RetVec.y = A.Values[4] * B.x + A.Values[5] * B.y + A.Values[6] * B.z + A.Values[7] * B.w;
			RetVec.z = A.Values[8] * B.x + A.Values[9] * B.y + A.Values[10] * B.z + A.Values[11] * B.w;
			RetVec.w = A.Values[12] * B.x + A.Values[13] * B.y + A.Values[14] * B.z + A.Values[15] * B.w;

			return RetVec;

		}

		static Matrix4 operator+ (const Matrix4 A, const Matrix4 B)
		{
			Matrix4 RetMat = Matrix4();
			for (size_t i = 0; i < 16; i++)
			{
				RetMat.Values.at(i) = A.Values.at(i) + B.Values.at(i);
			}
			return RetMat;
		}

		static Matrix4 operator* (const float B, const Matrix4 A)
		{
			Matrix4 RetMat = A;

			for (size_t i = 0; i < 16; i++)
			{
				RetMat.Values.at(i) *= B;
			}

			return RetMat;
		}

		static Matrix4 Interpolate(Matrix4 a, Matrix4 b, float t)
		{
			return (1 - t) * a + t * b;
		}

		static Matrix4 ThreeByThreeMult(const Matrix4 A, const Matrix4 B)
		{
			Matrix4 RetM = Matrix4(MatrixType::No_Values);

			for (int Collumn = 0; Collumn < 3; Collumn++)
			{
				for (int Row = 0; Row < 3; Row++)
				{
					float Value = 0.0f;
					for (int Target = 0; Target < 3; Target++)
					{
						Value += A.Values.at(Row * 3 + Target) * B.Values.at(Target * 3 + Collumn);
					}
					RetM.Values.push_back(Value);
				}
			}
			return RetM;
		}
		
		static Matrix4 Transpose(Matrix4 A)
		{
			Matrix4 RetMat = Matrix4(MatrixType::Identity);

			for (size_t Column = 0; Column < 4; Column++)
			{
				for (size_t Row = 0; Row < 4; Row++)
				{
					RetMat.Values.at(Row * 4 + Column) = A.Values.at(Column * 4 + Row);
				}

			}
			return RetMat;
		}

		static Matrix4 ThreeByThreeTranspose(const Matrix4 A)
		{
			Matrix4 RetMat = Matrix4(MatrixType::ThreeByThree);

			for (size_t Column = 0; Column < 3; Column++)
			{
				for (size_t Row = 0; Row < 3; Row++)
				{
					RetMat.Values.at(Row * 3 + Column) = A.Values.at(Column * 3 + Row);
				}
				
			}
			return RetMat;
		}
		
		//works but not used.
		//Antons OpenGL4 tutorials chapter :quaternion quckstart //https://capnramses.itch.io/antons-opengl-4-tutorials
		//static Matrix4 QuatToMatrix(Quat qa)
		//{
		//	Normalise(qa);
		//	float x = qa.x;
		//	float y = qa.y;
		//	float z = qa.z;
		//	float w = qa.w;
		//	Matrix4 RetMat = Matrix4(MatrixType::Identity);

		//	RetMat.Values.at(0) = 1.0f - 2.0f * y * y - 2.0f * z * z;
		//	RetMat.Values.at(1) = 2.0f * x * y + 2.0f * w * z;
		//	RetMat.Values.at(2) = 2.0f * x * z - 2.0f * w * y;
		//	//RetMat.Values.at(3) = 0.0f; //Identity part

		//	RetMat.Values.at(4) = 2.0f * x * y - 2.0f * w * z;
		//	RetMat.Values.at(5) = 1.0f - 2.0f * x * x - 2.0f * z * z;
		//	RetMat.Values.at(6) = 2.0f * y * z + 2.0f * w * x;
		//	//RetMat.Values.at(7) = 0.0f; //identity part

		//	RetMat.Values.at(8) = 2.0f * x * z + 2.0f * w * y;
		//	RetMat.Values.at(9) = 2.0f * y * z - 2.0f * w * x;
		//	RetMat.Values.at(10) = 1.0f - 2.0f * x * x - 2.0f * y * y;
		//	
		//	//rest is Identity matrix 
		//	return RetMat;
		//}

		//http://www.opengl-tutorial.org/assets/faq_quaternions/index.html#Q54
		static void MatRotate(Matrix4& Mat, Quat q)
		{

			float xx = q.x * q.x;
			float xy = q.x * q.y;
			float xz = q.x * q.z;
			float xw = q.x * q.w;

			float yy = q.y * q.y;
			float yz = q.y * q.z;
			float yw = q.y * q.w;

			float zz = q.z * q.z;
			float zw = q.z * q.w;

			//transposed
			Mat.Values.at(0) = 1 - 2 * (yy + zz);
			Mat.Values.at(4) = 2 * (xy - zw);
			Mat.Values.at(8) = 2 * (xz + yw);

			Mat.Values.at(1) = 2 * (xy + zw);
			Mat.Values.at(5) = 1 - 2 * (xx + zz);
			Mat.Values.at(9) = 2 * (yz - xw);

			Mat.Values.at(2) = 2 * (xz - yw);
			Mat.Values.at(6) = 2 * (yz + xw);
			Mat.Values.at(10) = 1 - 2 * (xx + yy);


			//untransposed
			/*Mat.Values.at(0) = 1 - 2 * (yy + zz);
			Mat.Values.at(1) = 2 * (xy - zw);
			Mat.Values.at(2) = 2 * (xz + yw);

			Mat.Values.at(4) = 2 * (xy + zw);
			Mat.Values.at(5) = 1 - 2 * (xx + zz);
			Mat.Values.at(6) = 2 * (yz - xw);

			Mat.Values.at(8) = 2 * (xz - yw);
			Mat.Values.at(9) = 2 * (yz + xw);
			Mat.Values.at(10) = 1 - 2 * (xx + yy);*/

		}

		//set rotation really
		static void MatRotate(Matrix4& Mat, Vec4 AxisAngle)
		{
			float Pitch = AxisAngle.w * AxisAngle.x; //*Mat.Values.at(0);	
			float Yaw = AxisAngle.w * AxisAngle.y;// *Mat.Values.at(5);
			float Roll = AxisAngle.w * AxisAngle.z;// *Mat.Values.at(10);

			float CosYaw = cos(Yaw);
			float SinYaw = sin(Yaw);
			float CosPitch = cos(Pitch);
			float SinPitch = sin(Pitch);
			float CosRoll = cos(Roll);
			float SinRoll = sin(Roll);

			Mat.Values.at(0) = (CosYaw * CosRoll) + (SinYaw * (SinPitch * SinRoll));
			Mat.Values.at(1) = (CosYaw * -SinRoll) + (SinYaw * (SinPitch * CosRoll));	
			Mat.Values.at(2) = SinYaw * CosPitch;
			Mat.Values.at(4) = CosPitch * SinRoll;
			Mat.Values.at(5) = CosPitch * CosRoll;
			Mat.Values.at(6) = -SinPitch;
			Mat.Values.at(8) = (CosRoll * -SinYaw) + (CosYaw * (SinPitch * SinRoll));
			Mat.Values.at(9) = (-SinYaw * -SinRoll) + (CosYaw * (SinPitch * CosRoll));
			Mat.Values.at(10) = CosPitch * CosYaw;

		}

		//set rotation really
		static void MatRotate(Matrix4& Mat, const float Angle, const Vec3 Axis)
		{
			float Pitch = Angle * Axis.x;
			float Yaw = Angle * Axis.y;
			float Roll = Angle * Axis.z;

			float CosYaw = cos(Yaw);
			float SinYaw = sin(Yaw);
			float CosPitch = cos(Pitch);
			float SinPitch = sin(Pitch);
			float CosRoll = cos(Roll);
			float SinRoll = sin(Roll);

			Mat.Values.at(0) = (CosYaw * CosRoll) + (SinYaw * (SinPitch * SinRoll));
			Mat.Values.at(1) = (CosYaw * -SinRoll) + (SinYaw * (SinPitch * CosRoll));
			Mat.Values.at(2) = SinYaw * CosPitch;

			Mat.Values.at(4) = CosPitch * SinRoll;
			Mat.Values.at(5) = CosPitch * CosRoll;
			Mat.Values.at(6) = -SinPitch;

			Mat.Values.at(8) = (CosRoll * -SinYaw) + (CosYaw * (SinPitch * SinRoll));
			Mat.Values.at(9) = (-SinYaw * -SinRoll) + (CosYaw * (SinPitch * CosRoll));
			Mat.Values.at(10) = CosPitch * CosYaw;

		}
		//set rotation really
		static void MatRotateLong(Matrix4& Mat,const float Angle, const Vec3 Axis)
		{
			float Pitch = Angle * Axis.x;
			float Yaw = Angle * Axis.y;
			float Roll = Angle * Axis.z;

			Matrix4 PitchMatrix = Matrix4(MatrixType::ThreeByThree);
			Matrix4 RollMatrix = Matrix4(MatrixType::ThreeByThree);

			PitchMatrix.Values.at(4) = cos(Pitch);
			PitchMatrix.Values.at(5) = -sin(Pitch);
			PitchMatrix.Values.at(7) = sin(Pitch);
			PitchMatrix.Values.at(8) = cos(Pitch);

			RollMatrix.Values.at(0) = cos(Roll);
			RollMatrix.Values.at(1) = -sin(Roll);
			RollMatrix.Values.at(3) = sin(Roll);
			RollMatrix.Values.at(4) = cos(Roll);
			
			
			Matrix4 PitchRollMatrix = ThreeByThreeMult(PitchMatrix, RollMatrix);

			Matrix4 YawMatrix = Matrix4(MatrixType::ThreeByThree);

			YawMatrix.Values.at(0) = cos(Yaw);
			YawMatrix.Values.at(2) = sin(Yaw);
			YawMatrix.Values.at(6) = -sin(Yaw);
			YawMatrix.Values.at(8) = cos(Yaw);

			
			Matrix4 ResultMatrix = ThreeByThreeMult(YawMatrix, PitchRollMatrix);

			//assigning new 3x3 rotation matrix values to 4x4 matrix.
			for (size_t i = 0; i < 3; i++)
			{
				Mat.Values.at(i) = ResultMatrix.Values.at(i);
				Mat.Values.at(i + 4) = ResultMatrix.Values.at(i + 3);
				Mat.Values.at(i + 8) = ResultMatrix.Values.at(i + 6);
			}

		}

		static void MatScale(Matrix4& Mat, const Vec3 ScaleAmount)
		{
			Mat.Values.at(0) *= ScaleAmount.x;
			Mat.Values.at(5) *= ScaleAmount.y;
			Mat.Values.at(10) *= ScaleAmount.z;
		}

		static void MatSetScale(Matrix4& Mat, const Vec3 ScaleAmount)
		{
			Mat.Values.at(0) = ScaleAmount.x;
			Mat.Values.at(5) = ScaleAmount.y;
			Mat.Values.at(10) = ScaleAmount.z;
		}

		static void MatTranslate(Matrix4& Mat, const Vec3 Translate)
		{
			Mat.Values.at(3) += Translate.x;
			Mat.Values.at(7) += Translate.y;
			Mat.Values.at(11) += Translate.z;
			Mat.Values.at(15) = 1.0f;
		}
		static void MatTranslateTransposed(Matrix4& Mat, const Vec3 Translate)
		{
			Mat.Values.at(12) += Translate.x;
			Mat.Values.at(13) += Translate.y;
			Mat.Values.at(14) += Translate.z;
			Mat.Values.at(15) = 1.0f;
		}
		static void MatSetTranslate(Matrix4& Mat, const Vec3 Translate)
		{
			Mat.Values.at(3) = Translate.x;
			Mat.Values.at(7) = Translate.y;
			Mat.Values.at(11) = Translate.z;
			Mat.Values.at(15) = 1.0f;
		}
		
		//https://www.3dgep.com/understanding-the-view-matrix/#The_View_Matrix
		//Using this, no longer using the GLM lookat copy
		static void MatViewMatrix(Matrix4& Mat, const Vec3 Position, const Vec3 ForwardDirection, const Vec3 UpDirection)
		{
			Vec3 Forward = Normalise(ForwardDirection);
			Vec3 Up = Normalise(UpDirection);
			Vec3 Right = CrossProduct(Forward, Up);

			Up = CrossProduct(Right,Forward); //have to redo this cross product to stop it warping.
			Right = Normalise(Right);

			Mat.Values.at(0) = Right.x;
			Mat.Values.at(1) = Right.y;
			Mat.Values.at(2) = Right.z;

			Mat.Values.at(4) = Up.x;
			Mat.Values.at(5) = Up.y;
			Mat.Values.at(6) = Up.z;

			Mat.Values.at(8) = -Forward.x;
			Mat.Values.at(9) = -Forward.y;
			Mat.Values.at(10) = -Forward.z;

			//ruined by rotation
			//Mat.Values.at(3) = -Position.x;
			//Mat.Values.at(7) = -Position.y;
			//Mat.Values.at(11) = Position.z;

			Mat.Values.at(3) = -Dot(Right, Position);
			Mat.Values.at(7) = -Dot(Up, Position);
			Mat.Values.at(11) = Dot(Forward, Position);

			Mat.Values.at(15) = 1.0f;
		}

		//https://stackoverflow.com/questions/21830340/understanding-glmlookat
		static Matrix4 Lookat(const Vec3 EyePosition, const Vec3 Direction, const Vec3 Up)
		{
			Matrix4 RetMat = Matrix4(MatrixType::No_Values);
			Vec3 x, y, z;
			z = EyePosition - Direction;
			z = Normalise(z);

			y = Up;
			x = CrossProduct(y, z);
			y = CrossProduct(z, x);
			
			x = Normalise(x);
			y = Normalise(y);

			RetMat.Values.push_back(x.x);
			RetMat.Values.push_back(x.y);
			RetMat.Values.push_back(x.z);
			RetMat.Values.push_back(Dot(-x, EyePosition));
			RetMat.Values.push_back(y.x);
			RetMat.Values.push_back(y.y);
			RetMat.Values.push_back(y.z);
			RetMat.Values.push_back(Dot(-y, EyePosition));
			RetMat.Values.push_back(z.x);
			RetMat.Values.push_back(z.y);
			RetMat.Values.push_back(z.z);
			RetMat.Values.push_back(Dot(-z, EyePosition));
			RetMat.Values.push_back(0.0f);
			RetMat.Values.push_back(0.0f);
			RetMat.Values.push_back(0.0f);
			RetMat.Values.push_back(1.0f);

			return RetMat;

		}

		static void SetTranslationMatrix(GLfloat* Containerin, const Vec3 TranslationIn)
		{
			GLfloat TheVector[4] = { TranslationIn.x,TranslationIn.y,TranslationIn.z,1.0f };

			for (size_t i = 0; i < 15; i += 4)
			{
				Containerin[i + 0] *= TheVector[0];
				Containerin[i + 1] *= TheVector[1];
				Containerin[i + 2] *= TheVector[2];
				Containerin[i + 3] *= TheVector[3];
			}
		}

		Matrix4 SetProjectionMatrixB(GLfloat const FOV, GLfloat const AspectRation, GLfloat const nearPlane, GLfloat const FarPlane);


		Matrix4 ResetIdentity();
#pragma endregion