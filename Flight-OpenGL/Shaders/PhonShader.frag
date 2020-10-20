#version 460 core
layout(location = 0) out vec4 FragColour;

in vec3 SpeedIntensity;
in vec3 Normal;
in vec4 CamPos;
in float SunIntensity;
in vec3 PlanetBitColour;
uniform vec3 ObjColour;
uniform bool IsSun;
uniform bool IsPlanet;

//lighting model from OpenGL 4 shader language cookbook. Translated to fragment shader instead of vertex.
uniform struct LightInfo{
vec4 Position; //Light position
vec3 Ambient; //intensity
vec3 Diffuse;
vec3 Specular;
} Light;

uniform struct MaterialInfo{
vec3 Ambient; //reflectivity
vec3 Diffuse; //intensity
vec3 Specular;
float Shine;
} Material;


void main()
{
if(IsSun)
	{
	vec3 colour = vec3(1.0,1.0,0.95f);
	colour.b *= exp(1 - SunIntensity) * 0.5f;
	FragColour = vec4(colour,1 - pow(SunIntensity,colour.b)); 
	return;
	}

vec3 LightOut = vec3(0);
if(IsPlanet)
	{

	vec3 Ambient = Light.Ambient * PlanetBitColour * 0.1;
	vec3 Specular = normalize(vec3(Light.Position - CamPos));

	float sDotN = max(dot(Specular,Normal),0.0f);
	vec3 Diffuse = Light.Diffuse * (PlanetBitColour * 0.8) * sDotN;
	vec3 Spec  = vec3(0.0f);
	if (sDotN > 0.0f)
		{
		vec3 v = normalize(-CamPos.xyz);
		vec3 r = reflect(-Specular ,Normal);
		Spec = Light.Specular * Material.Specular * pow(max(dot(r,v),0.0f),Material.Shine);
		}
	LightOut = Ambient + Diffuse + Spec;
	FragColour = vec4(LightOut,1.0f);
	return;
	}


else { //is normal object.

	vec3 Ambient = Light.Ambient * Material.Ambient;
	vec3 Specular = normalize(vec3(Light.Position - CamPos));

	float sDotN = max(dot(Specular,Normal),0.0f);
	vec3 Diffuse = Light.Diffuse * Material.Diffuse * sDotN;
	vec3 Spec  = vec3(0.0f);
	vec3 SpecRefraction  = vec3(0.0f);
	if (sDotN > 0.0f)
		{
		vec3 v = normalize(-CamPos.xyz);
		vec3 r = reflect(-Specular ,Normal);
		Spec = Light.Specular * Material.Specular * pow(max(dot(r,v),0.0f),Material.Shine);
	
		//https://asawicki.info/news_1301_reflect_and_refract_functions.html
		//More metallicy look to shiny things
		//r *= refract(Specular ,Normal,0.1);  //refraction function http://docs.gl/sl4/refract
		//SpecRefraction = vec3(0.8,0.99,0.80) * pow(max(dot(r,v),0.0f),Material.Shine);
	
		}
	LightOut = SpeedIntensity + Ambient + Diffuse + Spec + SpecRefraction;
	}

FragColour = vec4(LightOut,1.0f);
return;
}
