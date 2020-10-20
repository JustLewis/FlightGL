// Flight-OpenGL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Globals.h"
#include "MyToolkit/Window.h"
#include "world.h"
#include "Player.h"

const char* FragShaderLocation = "Shaders/PhonShader.frag";
const char* VertShaderLocation = "Shaders/PhonShader.vert";

Window window;
World* world;

std::shared_ptr<Player> player;

float ProjectionChange = 0;
bool SwapProjectionChange = false;
Matrix4 AlternativeProject;

void GLAPIENTRY //This is from Kronos' wiki
MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
void Draw();

//helping Shamma with child parent relationships (matrices obviously).
//int main()
//{
//	Matrix4 ParentMatrix = Matrix4(MatrixType::Identity);
//	Matrix4 ChildMatrix = Matrix4(MatrixType::Identity);
//
//	//setting matrices.
//	Vec4 ParentPosition = Vec4(0.5,1.0,1.5,1.0);
//	MatSetTranslate(ParentMatrix,ParentPosition);
//	Vec4 ChildPosition = Vec4(1.0, 0.0, 0.0, 1.0); //to the right by 1 value
//	MatSetTranslate(ChildMatrix, ChildPosition);
//	
//	//printing to console.
//	std::cout << "ParentMatrix is \n";	ParentMatrix.printMatrix();
//	std::cout << "\n Child Matrix is\n"; ChildMatrix.printMatrix();
//
//	//Setting parentchild matrix.
//	Matrix4 ChildParentMatrix = ParentMatrix * ChildMatrix;
//	
//	//printing to console
//	std::cout << "Child matrix * parent matrix = \n";
//	ChildParentMatrix.printMatrix();
//
//	//final position of a vertex at 0 position on the xyz coordinates.
//	Vec4 ChildPositionWithParent = ChildParentMatrix * Vec4(0, 0, 0, 1);
//	
//	//printing to console
//	std::cout << "Final position is "; ChildPositionWithParent.Print();
//
//}

int main()
{
	world = new World();
	window = Window(Render::WindowWidth, Render::WindowHeight);
	window.Initialise();

	Input::Keyboard = window.GetKeyIDArray();
	Input::MousePosChange = window.GetMousePositionChange();
	Utility::DeltaTime = 0.01674;

	Render::shader = Shader();
	Render::shader.CreateFromFiles(VertShaderLocation, FragShaderLocation);

	glEnable(GL_DEPTH_TEST);
	//blending for the sun to have opacity
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//culling faces
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	
	//glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	world->AddMesh();
	MeshFunctions::CreateCube(*world->GetLastMesh(), 1.0f); //cube ID: 0

	world->AddMesh();
	MeshFunctions::CreatePlane(*world->GetLastMesh(), 100.0f); //floor ID: 1

	world->AddMesh();
	MeshFunctions::CreateSpaceShip(*world->GetLastMesh()); //space ship ID: 2

	world->AddMesh();
	MeshFunctions::CreateProceduralSphere(*world->GetLastMesh(),200,100,100); //Sphere ID: 3

	//Player object to control.
	world->AddObject(2); //Object ID: 0

	//Sphere to represent world
	auto Obj = world->AddObjectAndGetPtr(3); //Object ID: 1
	Obj->AssignTransform(std::make_shared<Transform>(Transform(Vec3(0.0f, -20000.0f, 0.0f), Vec4(0.0f, 0.0f, 0.0f, 0.0f * DegreesToRadians), Vec3(100.0f,100.0f,100.0f))));
	Obj->SetMaterial(Vec3(0.1f, 0.25f, 0.15f), Vec3(0.35f, 0.85f, 0.35f), Vec3(0.1f, 0.1f, 0.6f), 100.0f);
	Obj->GetPhysics()->SetMassAndInertia(2000000.0f, 2000000.0f); //is a planet I guess
	Game::PlanetMass = Obj->GetPhysics()->GetMass();
	Obj->GetBSphere()->SetRadius(20000.0f);//Scale * Radius of the sphere.

	Obj->GetPhysics()->SetVelocity(Vec3(1.0f, 0.0f, 0.0f));
	Obj->GetPhysics()->SetTorque(Vec3(0.0f, 1.0f, 0.0f));
	
	//Sphere to represent sun
	Obj = world->AddObjectAndGetPtr(3); //Object ID: 2
	Obj->AssignTransform(std::make_shared<Transform>(Transform(Vec3(60000.0f, 100000.0f, 1000000.0f), Vec4(0.0f, 0.0f, 0.0f, 90.0f * DegreesToRadians), Vec3(1000.0f, 1000.0f, 1000.0f))));
	Obj->SetMaterial(true); //true is to set shader to only do colour of sun.
	Obj->GetPhysics()->SetMassAndInertia(900000.0f * 900000.0f, 900000.0f * 900000.0f); //is a star
	Obj->GetBSphere()->SetRadius(200000.0f);//Scale * Radius of the sphere.

	Obj = world->GetObjectAt(0);
	Obj->SetMaterial(Vec3(0.1f), Vec3(0.3f, 0.35f, 0.32f), Vec3(1.0f), 1.0f);

	//Attaching child objects. 
	Obj->AddSubObject(world->GetMeshAt(0));
	Obj->GetLastSubObject()->AssignTransform(std::make_shared<Transform>(Transform(Vec3(-1.5f, 1.0f, 5.0f), Vec4(0.0f), Vec3(1.f))));
	
	Obj->AddSubObject(world->GetMeshAt(0));
	Obj->GetLastSubObject()->AssignTransform(std::make_shared<Transform>(Transform(Vec3(1.5f, 1.0f, 5.0f), Vec4(0.0f), Vec3(1.f))));

/*	Obj->GetLastSubObject()->AddSubObject(world->GetMeshAt(0));
	Obj->GetLastSubObject()->GetLastSubObject()->AssignTransform(std::make_shared<Transform>(Transform(Vec3(0.0, 1.0, -1.0), Vec4(0.0), Vec3(1.0f))));
	
	Obj->GetLastSubObject()->GetLastSubObject()->AddSubObject(world->GetMeshAt(0));
	Obj->GetLastSubObject()->GetLastSubObject()->GetLastSubObject()->AssignTransform(std::make_shared<Transform>(Transform(Vec3(0.0, 1.0, -1.0), Vec4(0.0), Vec3(1.0f))));*/

	Obj = world->AddObjectAndGetPtr(0);
	Obj->GetTransform()->SetPosition(Vec3(100000.0f - 20.0f, 0.0f, 20.0f)); //cause player is falling to planet
	Obj->GetTransform()->SetScale(Vec3(20.0f));

	//setting up player
	player = std::make_shared<Player>(Player());
	player->SetControlledObject(world->GetObjectAt(0));
	player->SetCamera(std::make_shared<Camera>());
	player->SetSpeed(1000000.1f);
	player->SetRotationSpeed(15.0f);
	player->AttachCameraToObject(true);
	//player->GetControlledObject()->GetTransform()->SetPosition(Vec3(0.0f,10000.0f,0.0f)); 
	player->GetControlledObject()->GetTransform()->SetPosition(Vec3(100000.0f,1.0f,0.0f));
	player->GetControlledObject()->GetBSphere()->SetRadius(5.0f);//just a round numbers
	player->GetControlledObject()->GetPhysics()->SetMassAndInertia(1000.0f, 1000.0f); //low numbers because why not.


	//Setting up shader projection
	Render::ProjectionMat = ResetIdentity();
	Render::ProjectionMat = SetProjectionMatrixB(45.0f, (GLfloat)Render::WindowWidth / Render::WindowHeight, 1.0f, 100.0f);
	Render::shader.UseShader();
	Render::shader.SetUniformMatrix4F("Projection", Render::ProjectionMat.Values, false);
	std::cout << "Projection matrix: \n";
	Render::ProjectionMat.printMatrix();
	AlternativeProject = Render::ProjectionMat;

	for (size_t i = 0; i < 3; i++) {
		std::cout << std::endl;
		Vec4 TheVertex = Vec4(0.25f, -0.5f, 1.5f + i, 1.0f);
		std::cout << "The vertex before projection. "; TheVertex.Print();
		TheVertex = Render::ProjectionMat * TheVertex;
		std::cout << "The vertex after projection. "; TheVertex.Print();
	}
	//TheVertex.x /= TheVertex.w;
	//TheVertex.y /= TheVertex.w;
	//TheVertex.z /= TheVertex.w;
	//std::cout << "Actual position in normalised device coordinates with this projection = "; ((Vec3)TheVertex).Print();
	

	//AlternativeProject.Values.at(14) = 1.0f; //opposite of depth
	AlternativeProject.Values.at(10) = -2.75f; //How the z affects the scale of other values in the vertex
	AlternativeProject.Values.at(11) = -5.5f; //How the z affects the scale of other values in the vertex

	//TODO Make Light Class ?
	Vec3 LightPos = Vec3(6000.0f, 10000.0f, -100000.0f);
	Render::shader.SetUniform3F("Light.Position", LightPos.x, LightPos.y, LightPos.z);
	Render::shader.SetUniform3F("Light.Ambient", 0.1f, 0.1f, 0.1f);
	Render::shader.SetUniform3F("Light.Diffuse", 0.87f, 0.87f, 0.87f);
	Render::shader.SetUniform3F("Light.Specular", 1.0f, 1.0f, 1.0f);


	while (!window.GetWindowShouldClose())
	{
	Draw();
	}
	delete world;
	world = nullptr;
}

void SkySet()
{
	float MidnightDistance = 50000.0f;
	float SkyDistance = 23000.f;
	float PlayerDistance = Length(world->GetObjectAt(1)->GetTransform()->GetPosition() - player->GetControlledObject()->GetTransform()->GetPosition());
	float t = 0.0f;

	if (PlayerDistance > SkyDistance&& PlayerDistance < MidnightDistance)
	{
		t = (PlayerDistance - SkyDistance) / (MidnightDistance - SkyDistance);
	}
	else
	{
		if (PlayerDistance > MidnightDistance) { t = 1.0f; }
		if (PlayerDistance < SkyDistance) { t = 0.0f; }
	}
	Render::ClearColour = Interpolate(Colours::PurpleSky, Colours::MidnightBlue, t);
}
void RecreateShader()
{
	while (Input::Keyboard[GLFW_KEY_P]) //p pressed
	{
		glfwPollEvents();
		if (!Input::Keyboard[GLFW_KEY_P]) // P released
		{
			Render::shader.ClearShader();
			Render::shader.CreateFromFiles(VertShaderLocation, FragShaderLocation);

			Render::shader.UseShader();
			Render::shader.SetUniformMatrix4F("Projection", Render::ProjectionMat.Values, false);

			//TODO Make Light Class ?
			Vec3 LightPos = Vec3(6000.0f, 10000.0f, -100000.0f);
			Render::shader.SetUniform3F("Light.Position", LightPos.x, LightPos.y, LightPos.z);
			Render::shader.SetUniform3F("Light.Ambient", 0.1f, 0.1f, 0.1f);
			Render::shader.SetUniform3F("Light.Diffuse", 0.87f, 0.87f, 0.87f);
			Render::shader.SetUniform3F("Light.Specular", 1.0f, 1.0f, 1.0f);
		}
	}
}

void TimeUtilities() 
{
	//Setting to 60fps delta time in main.
	//Utility::DeltaTime = Utility::Time - glfwGetTime(); //This makes it stuttery
	Utility::Time = glfwGetTime();
	Render::shader.SetUniform1F("Time", Utility::Time);
	if (Utility::Time - 1 > Utility::OneSecondAgo)
	{
		char Name[64];
		sprintf_s(Name, "OpenGL 4.6 @%i FPS", Utility::FrameCount);
		window.WindowName(Name);
		Utility::OneSecondAgo = Utility::Time;
		Utility::FrameCount = 0;
	}
	else
	{
		Utility::FrameCount++;
	}
}

void Draw()
{
	glfwPollEvents();
	TimeUtilities();
	RecreateShader();
	SkySet();

	glClearColor(Render::ClearColour.x, Render::ClearColour.y, Render::ClearColour.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//for demonstrating projection thing.
	/*if (ProjectionChange > 1.0f) { SwapProjectionChange = true; }
	if (ProjectionChange < 0.0f) { SwapProjectionChange = false; }

	if (SwapProjectionChange) { ProjectionChange -= 0.01f; }
	if (!SwapProjectionChange) { ProjectionChange += 0.01f; }
	
	Render::shader.SetUniformMatrix4F("Projection", Interpolate(Render::ProjectionMat, AlternativeProject, ProjectionChange).Values, false);
	*/

	player->Update();

	if (Input::Keyboard[GLFW_KEY_T]) { Render::RenderPlanetEffects = !Input::Keyboard[GLFW_KEY_LEFT_SHIFT]; }
	
	//TODO put this in world class?
	for (size_t i = 0; i < world->GetObjectListSize(); i++)
	{
		//check for collisions
		auto Obj = world->GetObjectAt(i);

		if(Obj->GetBSphere() && i) 
		{
			for (size_t j = i; j < world->GetObjectListSize(); j++) //j = i here to make sure not duplicating collision checks.
			{
				auto Objb = world->GetObjectAt(j);
				if (Obj == Objb || !Objb->GetBSphere()) { continue; } //break out of iteration if same or bsphere is null ptr

				//if objects colliding
				if (BSpheresColliding(
					Obj->GetBSphere()->GetPoint(), Obj->GetBSphere()->GetRadius(),
					Objb->GetBSphere()->GetPoint(), Objb->GetBSphere()->GetRadius()))
				{
					//TODO add angular collision response and improve linear collision response.
					Vec3 ObjMomentum = Obj->GetPhysics()->GetMomentumAndDirection() + Objb->GetPhysics()->GetMomentumAndDirection();
					Obj->GetBSphere()->ApplyLinearForce(ObjMomentum);
				}

			}
		}

		//Update all physics and render.
		Render::ModelMatrix = Matrix4(MatrixType::Identity);
		if (Obj == player->GetControlledObject())
		{
			float Speed = player->GetControlledObject()->GetPhysics()->GetSpeed();
			//std::cout << "Speed is " << Speed << std::endl;
			Render::shader.SetUniform3F("SpeedColour", 1.0f, 0.2f, 0.2f);
			Render::shader.SetUniform1F("Speed", Speed * 0.001f);
		}
		else
		{
			Render::shader.SetUniform3F("SpeedColour", 0.0f, 0.0f, 0.0f);
			Render::shader.SetUniform1F("Speed", 0.0f);
		}
		if (i == 1)
		{
			Game::PlanetPosition = Obj->GetTransform()->GetPosition();
			Render::shader.SetUniformBool("IsPlanet", Render::RenderPlanetEffects);
		}
		else 
		{
			Render::shader.SetUniformBool("IsPlanet", false);
		}

		Obj->Render(Render::ModelMatrix);
	}

	window.SwapBuffers();
	glDisable(GL_DEBUG_OUTPUT);
	return;
}

void GLAPIENTRY
MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	//This started corrupting weirdly. Using STD::COUT instead
	/*fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);*/

	std::cout << "GL CALLBACK : " << (type == GL_DEBUG_TYPE_ERROR ? " ** GL ERROR ** " : "") << "type = 0x" << severity << ", message = " << message << "\n\n";
}