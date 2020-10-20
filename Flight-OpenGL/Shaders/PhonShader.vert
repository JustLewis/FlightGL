#version 460 core
layout(location = 0) in vec3 VPos;
layout(location = 1) in vec3 Norm;

uniform mat4 Projection, Model, View;

uniform vec3 SpeedColour; 
uniform float Speed; 
uniform bool IsSun;
uniform bool IsPlanet;
uniform float Time;

out vec3 Normal;
out vec4 CamPos;
out vec3 SpeedIntensity;
out float SunIntensity;
out vec3 PlanetBitColour;

float Pi = 3.14159265359;

void main ()
{
if(IsSun)
{
	SunIntensity = length(VPos.xy) * 0.015;
	gl_Position = Projection * View * Model * vec4(VPos,1.0f);
	return;
}

vec3 PlanetOffset = vec3(0);
if(IsPlanet) //inspired by this https://www.shadertoy.com/view/XsVBWG
{
	float timeScale = .2;
    vec2 Vertexpoint = VPos.yz * 0.01;
    
    for(int i=1; i<int(7); i++)
	{
        Vertexpoint.x += 0.1 * sin(i * Vertexpoint.y + Time * timeScale);
        Vertexpoint.y += 0.3 * cos(i * Vertexpoint.x + Time * .2);
    }

	//length to make sure it's not negetive, + values at the end to desaturate the colour.
    float r = length(cos(Vertexpoint.x + Vertexpoint.y + 2.) * 0.6) + 0.1;
    float g = length(sin(Vertexpoint.x + Vertexpoint.y + 2.))+ 0.2;
    float b = length(sin(Vertexpoint.x + Vertexpoint.y + 1.)+ cos(Vertexpoint.x + Vertexpoint.y + .5) * 0.8) + 0.3;

    vec3 colour = vec3(r,g,b);
	PlanetBitColour = colour;
	PlanetOffset.xy = colour.xy * 5 * sin(Time);
	PlanetOffset.z = colour.z * 5 * cos(Time);
}

//Normal = Norm;
Normal = normalize(mat3(transpose(inverse(Model))) * Norm); // This is for if the mesh is scaled non-uniformly.  https://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/

CamPos = View * Model * vec4(VPos,1.0f);

SpeedIntensity = clamp(sin((SpeedColour * Speed)) * length(VPos.x),0.0f,1.0f);

gl_Position = Projection * View * Model * vec4(VPos + PlanetOffset,1.0f);
}