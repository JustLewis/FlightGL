#pragma once
#include "Globals.h"

//not used
class Material
{
public:

	Material()
	{
		Colour = Vec3(1.0f, 1.0f, 1.0f);
		Shine = 100.0f;
	}
	void SetShine(float ShineIn) { Shine = ShineIn; }
	GLfloat GetShine() { return Shine; }
	void SetColour(Vec3 ColourIn) { Colour = ColourIn; }
	Vec3 GetColour() { return Colour; }


private:

	Vec3 Colour;
	GLfloat Shine;
};