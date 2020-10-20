#pragma once

#include <glad/glad.h>
#include <vector>
#include "MyMaths.h"

class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat* Verts, GLuint* Inds,GLuint NumberOfVerts, GLuint NumberOfInds, bool CalcNormals);
	void CreateMesh(GLfloat* Verts, GLuint* Inds,GLuint NumberOfVerts, GLuint NumberOfInds, Vec3 NormalsDirection);

	void RenderMesh();
	void RenderPoints();

	void ClearMesh();
	~Mesh();

	void CalcAverageNormals(GLuint* IndicesIn, GLuint IndicesInCount, GLfloat* vertices,
		GLuint VertexCount);

	void CalcNormals(GLuint* IndicesIn, GLuint IndicesInCount, GLfloat* vertices,
		GLuint VertexCount, Vec3 NormalDirection);

private:

	//handles
	GLuint VertexArray, VertexBuffer, Index;
	GLuint IndexCount;
	GLuint VertexLength = 6;
	GLuint NormalOffset = 3;

};

namespace MeshFunctions {

	static void CreatePlane(Mesh& TheMesh, GLfloat Size)
	{
		GLfloat Verts[]
		{
			-Size,0.0f,-Size, 	-0.35f, 0.7f, -0.35f,
			Size,0.0f,-Size, 	0.35f, 0.7f, -0.35f,
			Size,0.0f,Size, 	0.35f, 0.7f, 0.35f,
			-Size,0.0f,Size, 	-0.35f, 0.7f, 0.35f,
		};

		GLuint Indices[]
		{
			0,1,2,
			2,3,0
		};

		TheMesh.CreateMesh(Verts, Indices, 24, 6,false);
	}

	static void CreateTriangle(Mesh& TheMesh, GLfloat Size)
	{
		GLfloat Verts[]
		{
			//Vertex			Normal 
			-Size,0.0f,0.0f, 	0.0f, 0.0f, 0.0f,
			Size,0.0f,0.0f, 	0.0f, 0.0f, 0.0f,
			0.0f,Size,0.0f,		0.0f, 0.0f, 0.0f,
		};
		GLuint Indices[]
		{
			0,1,2
		};
		TheMesh.CreateMesh(Verts, Indices, 9, 6,Vec3(0.0f,0.0f,1.0f));
	}

	static void CreateCube(Mesh& TheMesh, GLfloat SizeIn)
	{
		GLfloat Size = SizeIn * 0.5f;
		GLfloat Verts[]
		{
			-Size,-Size,-Size, 	0.0f, 0.0f, 0.0f, //0
			Size,-Size,-Size, 	0.0f, 0.0f, 0.0f, //1
			Size,-Size,Size, 	0.0f, 0.0f, 0.0f, //2
			-Size,-Size,Size, 	0.0f, 0.0f, 0.0f, //3
			-Size,Size,-Size,	0.0f, 0.0f, 0.0f, //4
			Size,Size,-Size, 	0.0f, 0.0f, 0.0f, //5
			Size,Size,Size, 	0.0f, 0.0f, 0.0f, //6
			-Size,Size,Size, 	0.0f, 0.0f, 0.0f, //7
		};
		/*GLuint Indices[]
		{
		0,1,5,
		0,5,4,
		1,2,6,
		1,6,5,
		2,3,7,
		2,7,6,
		3,0,4,
		3,4,7,
		4,5,6,
		4,6,7,
		3,2,1,
		3,1,0,			
		};*/

		GLuint Indices[]
		{
		5,1,0,
		4,5,0,
		6,2,1,
		5,6,1,
		7,3,2,
		6,7,2,
		4,0,3,
		7,4,3,
		6,5,4,
		7,6,4,
		1,2,3,
		0,1,3
		};

		TheMesh.CreateMesh(Verts, Indices, 48, 36,true);
	}

	static void CreateWings(Mesh& TheMesh, GLfloat Size,GLfloat HeightIn)
	{
		GLfloat Height = HeightIn * 0.5f;
		GLfloat Verts[]
		{
			-Size,-Height,-Size, 	0.0f, 0.0f, 0.0f, //0
			Size,-Height,-Size, 	0.0f, 0.0f, 0.0f, //1
			0.0f,-Height,Size, 	0.0f, 0.0f, 0.0f, //2
			-Size,Height,-Size,	0.0f, 0.0f, 0.0f, //3
			Size,Height,-Size, 	0.0f, 0.0f, 0.0f, //4 
			0.0f,Height,Size, 	0.0f, 0.0f, 0.0f, //5
		};
		GLuint Indices[]
		{
			0,2,1,
			3,4,5,
			0,1,4,
			4,3,0,
			1,2,5,
			5,4,1,
			2,0,3,
			3,5,2
		};
		TheMesh.CreateMesh(Verts, Indices, 36, 24,true);
	}

	static void CreateWingsB(Mesh& TheMesh, GLfloat Size, GLfloat HeightIn)
	{
		GLfloat Height = HeightIn * 0.5f;
		GLfloat Verts[]
		{
			-Size,-Height,-Size, 	0.0f, 0.0f, 0.0f, //0
			Size,-Height,-Size, 	0.0f, 0.0f, 0.0f, //1
			0.0f,0.0f,Size, 	0.0f, 0.0f, 0.0f, //2
			-Size,Height,-Size,	0.0f, 0.0f, 0.0f, //3
			Size,Height,-Size, 	0.0f, 0.0f, 0.0f, //4 

		};
		GLuint Indices[]
		{
			0,1,4,
			4,3,0,
			1,2,4,
			0,2,3,
			3,2,4,
			0,2,1

		};
		TheMesh.CreateMesh(Verts, Indices, 30, 18,true);
	}

	static void CreatePyramid(Mesh& TheMesh, GLfloat Size)
	{
		GLfloat Verts[]{
			-Size,-Size ,-Size,		0.0f, 0.0f, 0.0f,
			Size,-Size ,-Size,		0.0f, 0.0f, 0.0f,
			Size,-Size ,Size,		0.0f, 0.0f, 0.0f,
			-Size,-Size,Size,		0.0f, 0.0f, 0.0f,
			0.0f,Size ,0.0f,		0.0f, 0.0f, 0.0f,
		};
		GLuint Indices[]{
			0,1,2,
			2,3,0,
			0,1,4,
			1,2,4,
			2,3,4,
			3,0,4

		};
		TheMesh.CreateMesh(Verts, Indices, 30, 18,true);
	}

	static void CreateSpaceShip(Mesh& TheMesh)
	{
		GLfloat Verts[]{
			//vert				//normal
			0.0f,1.0f,-1.0f,	0.0f, 0.0f, 0.0f, //0
			-1.0f,0.5f,-1.0f,	0.0f, 0.0f, 0.0f, //1
			-3.5f,0.5f,0.5f,	0.0f, 0.0f, 0.0f, //2
			-1.0f,-0.5f,-1.0f,	0.0f, 0.0f, 0.0f, //3
			0.0f,-1.5f,-1.0f,	0.0f, 0.0f, 0.0f, //4
			1.0f,-0.5f,-1.0f,	0.0f, 0.0f, 0.0f, //5
			3.5f,0.5f,	0.5f,	0.0f, 0.0f, 0.0f, //6
			1.0f,0.5f,-1.0f,	0.0f, 0.0f, 0.0f, //7
			0.0f,0.0f,-2.0f,	0.0f, 0.0f, 0.0f, //8
			0.0f,0.5f,7.5f,		0.0f, 0.0f, 0.0f, //9
		};

		//anti clockwise
		/*GLuint Indices[]{
			0,1,8,
			8,7,0,
			1,2,3,
			3,8,1,
			8,3,4,
			4,5,8,
			7,8,5,
			5,6,7,
			0,9,1,
			1,9,2,
			2,3,9,
			3,4,9,
			4,5,9,
			5,6,9,
			6,7,9,
			7,0,9,
			};*/

		//clockwise... But anti-clockwise when transformed by projection and view.
		GLuint Indices[]{
		8,1,0,
		7,8,0,
		7,5,8,
		8,5,4,
		8,4,3,
		8,3,1,
		1,3,2,
		7,6,5,
		1,9,0,

		//front
		2,9,1,
		2,3,9,
		3,4,9,
		4,5,9,
		5,6,9,
		7,9,6,
		0,9,7,

		};

		TheMesh.CreateMesh(Verts, Indices, 60, 48,true);
	}

	static void CreateSpaceShipB(Mesh& TheMesh)
	{
		float Mid = 1.0f;
		GLfloat Verts[]{
			//Back
			0.0f,0.0f,-2.0f, 0.0f,0.0f,0.0f, //0
			0.0f,2.0f,1.0f,  0.0f,0.0f,0.0f, //1
			-1.0f,0.5f,0.0f, 0.0f,0.0f,0.0f, //2
			-5.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, //3
			-1.0f,-0.50f,0.0f, 0.0f,0.0f,0.0f, //4
			0.0f,-0.2f,2.0f, 0.0f,0.0f,0.0f, //5
			1.0f,-0.5f,0.0f, 0.0f,0.0f,0.0f, //6
			5.0f,0.0f,0.0f,  0.0f,0.0f,0.0f, //7
			1.0f,0.5f,0.0f, 0.0f,0.0f,0.0f, //8

			//mid
			0.0f, Mid, 1.0f, 0.0f,0.0f,0.0f, //9
			-Mid, 0.0f, 1.0f, 0.0f,0.0f,0.0f, //10
			0.0f, -Mid, 1.0f, 0.0f,0.0f,0.0f, //11
			Mid, 0.0f, 1.0f, 0.0f,0.0f,0.0f, //12
			0.0f, Mid / 2, 0.5f, 0.0f,0.0f,0.0f, //13

			//mid front
			0.0f, Mid/ 2, 2.0f, 0.0f,0.0f,0.0f, //14
			-Mid, 0.0f, 2.0f, 0.0f,0.0f,0.0f, //15
			0.0f, -Mid / 2, 2.0f, 0.0f,0.0f,0.0f, //16
			Mid, 0.0f, 2.0f, 0.0f,0.0f,0.0f, //17

			//front
			0.0f, 0.0f, 3.0f, 0.0f,0.0f,0.0f, //18
		};

		GLuint Indices[]{
			0,2,1,
			0,4,2,
			0,5,4,
			0,6,5,
			0,8,6,
			0,1,8,
			2,4,3,
			8,7,6,
			5,6,7,
			3,5,4,



		};

		TheMesh.CreateMesh(Verts, Indices, 114, 30, true);
	}
	
	//Generate procedural sphere from previous project Sucked so use Songho
	/*static void CreateProceduralSphere(Mesh& TheMesh,GLfloat RadiusIn, GLuint HfacesIn, GLuint VfacesIn)
	{
		const float ToRadians = 3.14159265359 / 180;
		const int TotalFaces = HfacesIn * VfacesIn;
		std::vector<GLfloat> Verts;
		Verts.resize(TotalFaces * 4 * 6); //4 corners to a face, 6 pieces of vertex data - 3 for xyz positions, 3 for normals
		std::vector<GLuint> Inds;
		Inds.resize(TotalFaces * 6); //6 values per index to make a quad, half of those needed for whole sphere.

		GLfloat Radius = RadiusIn;
		unsigned int VerticesCount = 0;

		//Calculate verts
		for (GLfloat i = 0; i <= 180; i += (360 / VfacesIn))
		{
			//printf("%f\n", i);
			float theta = i * ToRadians; //get current angle in radians
			float CurrentX = Radius * sinf(theta);
			float CurrentY = Radius * cosf(theta);

			for (GLfloat j = 0; j <= 360; j += (360 / HfacesIn))
			{
				//Vertices
				float theta = j * ToRadians; //get current angle
				Verts[VerticesCount] = CurrentX * sinf(theta);		//X
				Verts[VerticesCount + 1] = CurrentY;				//Y
				Verts[VerticesCount + 2] = CurrentX * cosf(theta);	//Z

				VerticesCount += 6;
			}
		}

		unsigned int EdgeCheckCount = 0;
		//calculate inds
		for (GLuint j = 0; j < (TotalFaces * 6); j += 6)
		{

			bool EdgeCheck = false;
			if (EdgeCheckCount % HfacesIn == HfacesIn - 1) { EdgeCheck = true; } //To see if we've reach the edge of the line //Using modulo to check the remained = the numvber of Hfaces - 1

			//first triangle
			Inds[j] = (EdgeCheckCount);
			Inds[j + 1] = (EdgeCheckCount + HfacesIn);
			Inds[j + 2] = (EdgeCheckCount + HfacesIn + 1);

			//second triangle
			Inds[j + 3] = (EdgeCheckCount + HfacesIn + 1);
			Inds[j + 4] = (EdgeCheckCount + 1);
			Inds[j + 5] = (EdgeCheckCount);

			if (EdgeCheck)
			{
				//update these indices if at edge
				Inds[j + 2] = EdgeCheckCount + 1;
				Inds[j + 3] = EdgeCheckCount + 1;
				Inds[j + 4] = EdgeCheckCount + 1 - HfacesIn;
			}
			//debug.. 
			//printf("%i, %i, %i,   %i,%i,%i\n", Inds[j], Inds[j + 1], Inds[j + 2], Inds[j + 3], Inds[j + 4], Inds[j + 5]);
			EdgeCheckCount++;
		}

		TheMesh.CreateMesh(Verts.data(), Inds.data(), TotalFaces * 4 * 6, TotalFaces * 6);
	}*/


	//http://www.songho.ca/opengl/gl_sphere.html
	static void CreateProceduralSphere(Mesh& TheMesh, GLfloat RadiusIn, GLuint HfacesIn, GLuint VfacesIn)
	{
		const float ToRadians = 3.14159265359 / 180;
		const int TotalFaces = HfacesIn * VfacesIn;
		std::vector<GLfloat> Verts;
		std::vector<GLuint> Inds;

		GLfloat Radius = RadiusIn;
		unsigned int VerticesCount = 0;

		float x, y, z, xy;                              // vertex position
		float nx, ny, nz, lengthInv = 1.0f / Radius;    // vertex normal
		float s, t;                                     // vertex texCoord

		float sectorStep = 2 * 3.14159265359 / HfacesIn;
		float stackStep = 3.14159265359 / VfacesIn;
		float sectorAngle, stackAngle;

		for (int i = 0; i <= VfacesIn; ++i)
		{
			stackAngle = 3.14159265359 / 2 - i * stackStep;        // starting from pi/2 to -pi/2
			xy = Radius * cosf(stackAngle);             // r * cos(u)
			z = Radius * sinf(stackAngle);              // r * sin(u)

			// add (sectorCount+1) vertices per stack
			// the first and last vertices have same position and normal, but different tex coords
			for (int j = 0; j <= HfacesIn; ++j)
			{
				sectorAngle = j * sectorStep;           // starting from 0 to 2pi

				// vertex position (x, y, z)
				x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
				y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
				Verts.push_back(x);
				Verts.push_back(y);
				Verts.push_back(z);

				//normals for if calculating them.
				//Verts.push_back(0.0f);
				//Verts.push_back(0.0f);
				//Verts.push_back(0.0f);

				// normalized vertex normal (nx, ny, nz)
				nx = x * lengthInv;
				ny = y * lengthInv;
				nz = z * lengthInv;
				Verts.push_back(nx);
				Verts.push_back(ny);
				Verts.push_back(nz);

				VerticesCount += 6;
			}
		}

		int k1, k2, IndsCount = 0;
		for (int i = 0; i < VfacesIn; ++i)
		{
			k1 = i * (VfacesIn + 1);     // beginning of current stack
			k2 = k1 + HfacesIn + 1;      // beginning of next stack

			for (int j = 0; j < HfacesIn; ++j, ++k1, ++k2)
			{
				// 2 triangles per sector excluding first and last stacks
				// k1 => k2 => k1+1
				if (i != 0)
				{
					Inds.push_back(k1);
					Inds.push_back(k2);
					Inds.push_back(k1 + 1);
					IndsCount += 3;
				}

				// k1+1 => k2 => k2+1
				if (i != (HfacesIn - 1))
				{
					Inds.push_back(k1 + 1);
					Inds.push_back(k2);
					Inds.push_back(k2 + 1);
					IndsCount += 3;
				}
			}
		}

		TheMesh.CreateMesh(Verts.data(), Inds.data(), VerticesCount, IndsCount, true);
	}
}