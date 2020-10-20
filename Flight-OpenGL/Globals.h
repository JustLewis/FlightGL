#pragma once
#include "Shader.h"
#include "MyMaths.h"

namespace Colours
{
	static const Vec3 SkyBlue = Vec3(135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f);
	static const Vec3 PurpleSky = Vec3(95.0f / 255.0f, 85.0f / 255.0f, 127.0f / 255.0f);
	static const Vec3 MidnightBlue = Vec3(5.0f / 255.0f, 18.0f / 255.0f, 36.0f / 255.0f);
}
namespace Light
{
	//static Vec3 LightDirection = Normalise(Vec3(1.0f, 1.0f, 1.0f));
	static Vec3 LightPosition = Vec3(0.0f, 1.0f,0.0f);
	static Vec3 LightColour = Vec3(0.5f, 0.1f, 1.0f);
}

namespace Debug
{
	bool UseFloor = false;
	bool RenderPyramid = false;
	bool RenderDemoTri = false;
	bool RenderCube = true;
	bool UseFragLight = false;

}

namespace Input
{
	bool* Keyboard = nullptr;
	double* MousePosChange = nullptr;
}

namespace Render
{
	Shader shader;
	Matrix4 ProjectionMat = Matrix4(MatrixType::No_Values);
	Matrix4 ModelMatrix = Matrix4(MatrixType::Identity);
	Matrix4 viewMatrix = Matrix4(MatrixType::Identity);

	//1080p
	//unsigned int WindowWidth = 1920;
	//unsigned int WindowHeight = 1080;

	//unsigned int WindowWidth = 1920 / 2;
	//unsigned int WindowHeight = 1080 / 2;

	//4k
	unsigned int WindowWidth = 3840;
	unsigned int WindowHeight = 2160;

	Vec3 ClearColour = Vec3();
	bool RenderPlanetEffects = false;

}

namespace Utility
{
	double Time;
	double DeltaTime;
	double OneSecondAgo;
	unsigned int FrameCount;

}

namespace Game

{
	float PlanetMass;
	Vec3 PlanetPosition;
}