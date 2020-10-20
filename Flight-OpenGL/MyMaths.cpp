#include "MyMaths.h"



Matrix4::Matrix4(MatrixType Typein)
{
	switch (Typein)
	{
	case No_Values:
		break;
	
	case Identity:
		for (size_t i = 0; i < 16; i++)
		{
			Values.push_back(0.0f);
		}
		Values.at(0) = 1.0f;
		Values.at(5) = 1.0f;
		Values.at(10) = 1.0f;
		Values.at(15) = 1.0f;
		break;
	
	case Zero:
		for (size_t i = 0; i < 16; i++)
		{
			Values.push_back(0.0f);
		}
		break;

	case ThreeByThree:
		for (size_t i = 0; i < 12; i++)
		{
			Values.push_back(0.0f);
		}
		Values.at(0) = 1.0f;
		Values.at(4) = 1.0f;
		Values.at(8) = 1.0f;
		break;

	default:
		for (size_t i = 0; i < 16; i++)
		{
			Values.push_back(0.0f);
		}
		break;
	};

}

Matrix4::Matrix4()
{
	for (size_t i = 0; i < 16; i++)
	{
		Values.push_back(0.0f);
	}
	Values.at(0) = 1.0f;
	Values.at(5) = 1.0f;
	Values.at(10) = 1.0f;
	Values.at(15) = 1.0f;
}

Matrix4 ResetIdentity()
{
	return Matrix4(MatrixType::Identity);
}

//http://www.songho.ca/opengl/gl_projectionmatrix.html 
//https://www.scratchapixel.com/lessons/3d-basic-rendering/perspective-and-orthographic-projection-matrix/opengl-perspective-projection-matrix
Matrix4 SetProjectionMatrixB(GLfloat FOV, GLfloat AspectRation, GLfloat NP, GLfloat FP)//np - near plane, fp - far plane
{
	float Scale = tan(FOV * 0.5f * (3.14 / 180)) * NP;
	float r = AspectRation * Scale; //right 
	float l = -r; //left
	float t = Scale; //top
	float b = -t; //bottom
	//bit of a mess
	Matrix4 RetMat = Matrix4(MatrixType::No_Values);

	//From the tutorial
	/*RetMat.Values.insert(RetMat.Values.begin(),
	{
	2.0f * NP / (r - l),	0.0f, 0.0f, 0.0f,
	0.0f, 2 * NP / (t - b), 0.0f, 0.0f,
	(r + l) / (r - l),(t + b) / (t - b),-(FP + NP) / (FP - NP),-2.0f * (FP * NP) / (FP - NP), //First 2 values are 0, no idea why we need to calculate it.
	0.0f, 0.0f, -1.0f, 0.0f,
	});*/

	RetMat.Values.insert(RetMat.Values.begin(),
	{
	2.0f * NP / (r - l),	0.0f, 0.0f, 0.0f,
	0.0f, 2 * NP / (t - b), 0.0f, 0.0f,
	0.0f,0.0f,-(FP + NP) / (FP - NP),-2.0f * (FP * NP) / (FP - NP),
	0.0f, 0.0f, -1.0f, 0.0f,
	});

	//transpoased projection matrix
	//RetMat.Values.insert(RetMat.Values.begin(),
	//	{
	//	2.0f * NP / (r - l),0.0f, 0.0f, 0.0f,
	//	0.0f, 2 * NP / (t - b),0.0f, 0.0f,
	//	(r + l) / (r - l),(t + b) / (t - b),-(FP + NP) / (FP - NP),-1.0f,
	//	0.0f,0.0f, -2 * FP * NP / (FP - NP), 0.0f,
	//	});

	return RetMat;
};

