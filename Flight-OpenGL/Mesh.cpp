#include "Mesh.h"

Mesh::Mesh()
{
	VertexArray = 0;
	VertexBuffer = 0;
	Index = 0;
	IndexCount = 0;

}

///This function is great for creating smooth surfaces as the average normals affect the way the light diffuses on the surface. https://www.udemy.com/course/graphics-with-modern-opengl/learn/lecture/10016978#overview
/// Other important info https://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
void Mesh::CalcAverageNormals(GLuint* IndicesIn, GLuint IndicesInCount, GLfloat* vertices,
	GLuint VertexCount)
{

	for (size_t i = 0; i < IndicesInCount; i += VertexLength)
	{
		//assigning xyz coordiates from indices
		unsigned int in0 = IndicesIn[i] * VertexLength;
		unsigned int in1 = IndicesIn[i + 1] * VertexLength;
		unsigned int in2 = IndicesIn[i + 2] * VertexLength;

		//get verts from indices
		Vec3 verta(vertices[in0], vertices[in0 + 1], vertices[in0 + 2]);
		Vec3 vertb(vertices[in1], vertices[in1 + 1], vertices[in1 + 2]);
		Vec3 vertc(vertices[in2], vertices[in2 + 1], vertices[in2 + 2]);

		//draw lines 
		Vec3 line1 = vertb - verta;
		Vec3 line2 = vertc - verta;

		//Get the normal direction and normalize
		Vec3 Normal = CrossProduct(line1, line2);
		Normal = Normalise(Normal);

		//now to set the normal direction values in xyz for the last 3 units in each of the vertex data. 
		//First by getting the values using the offset, then by assigning the values
		in0 += NormalOffset; in1 += NormalOffset; in2 += NormalOffset;

		vertices[in0] += Normal.x; vertices[in0 + 1] += Normal.y; vertices[in0 + 2] += Normal.z;
		vertices[in1] += Normal.x; vertices[in1 + 1] += Normal.y; vertices[in1 + 2] += Normal.z;
		vertices[in2] += Normal.x; vertices[in2 + 1] += Normal.y; vertices[in2 + 2] += Normal.z;
	}

	//after adding all the directions to the normal, normalise
	for (size_t i = 0; i < VertexCount / VertexLength; i++)
	{
		unsigned int nOffset = i * VertexLength + NormalOffset;
		Vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = Normalise(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}


}

void Mesh::CalcNormals(GLuint* IndicesIn, GLuint IndicesInCount, GLfloat* vertices, GLuint VertexCount,Vec3 NormalDirection)
{

	for (size_t i = 0; i < VertexCount; i += VertexLength)
	{
		vertices[i + NormalOffset] = NormalDirection.x; 
		vertices[i + NormalOffset + 1] = NormalDirection.y;
		vertices[i + NormalOffset + 2] = NormalDirection.z;
	}
}

void Mesh::CreateMesh(GLfloat* Verts, GLuint* Inds,GLuint NumberOfVerts, GLuint NumberOfInds, bool CalcNormals)
{
	IndexCount = NumberOfInds;
	if (CalcNormals) {
		CalcAverageNormals(Inds, IndexCount, Verts, NumberOfVerts);
	}

	glGenVertexArrays(1, &VertexArray);
	glBindVertexArray(VertexArray);

	glGenBuffers(1, &VertexBuffer);
	glGenBuffers(1, &Index);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, NumberOfVerts * sizeof(Verts[0]), Verts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexCount * sizeof(Inds[0]), Inds, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Verts[0]) * 6, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Verts[0]) * 6, (void*)(sizeof(Verts[0]) * 3));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Mesh::CreateMesh(GLfloat* Verts, GLuint* Inds, GLuint NumberOfVerts, GLuint NumberOfInds, Vec3 NormalsDirection)
{
	IndexCount = NumberOfInds;

	CalcNormals(Inds, IndexCount, Verts, NumberOfVerts,NormalsDirection); 


	glGenVertexArrays(1, &VertexArray);
	glBindVertexArray(VertexArray);

	glGenBuffers(1, &VertexBuffer);
	glGenBuffers(1, &Index);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, NumberOfVerts * sizeof(Verts[0]), Verts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexCount * sizeof(Inds[0]), Inds, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Verts[0]) * 6, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Verts[0]) * 6, (void*)(sizeof(Verts[0]) * 3));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Mesh::RenderMesh()
{

	glBindVertexArray(VertexArray);
	glBindBuffer(GL_ARRAY_BUFFER,VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Index);
	glEnableVertexAttribArray(0);
	//glDrawArrays(GL_POINTS, 0, IndexCount);
	glDrawElements(GL_TRIANGLES,IndexCount,GL_UNSIGNED_INT,0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::RenderPoints()
{
	glBindVertexArray(VertexArray);
	glDrawArrays(GL_POINTS, 0, IndexCount);
	glBindVertexArray(0);
}

void Mesh::ClearMesh()
{
	if (VertexArray)
	{
		glDeleteVertexArrays(1, &VertexArray);
	}
	if (VertexBuffer)
	{
		glDeleteBuffers(1, &VertexBuffer);
	}
	if (Index)
	{
		glDeleteBuffers(1, &Index);
	}
}

Mesh::~Mesh()
{
	ClearMesh();
}
