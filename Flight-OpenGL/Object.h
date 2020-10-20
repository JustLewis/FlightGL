#pragma once
#include <vector>
//#include "Shader.h"
#include "Mesh.h"
#include "Collision.h"
//TODO Put Material in struct?

class Object
{
public:
	Object()
	{
		transform = std::make_shared<Transform>();
		physics = std::make_shared<Physics>();
		BSphere = std::make_shared<BoundingSphere>();
		mesh = nullptr;
		//Colour = 0.5f, 0.5f, 0.5f;
		Shine = 100.0f;
		Ambient = Vec3(0.1f, 0.1f, 0.1f);
		Diffuse = Vec3(0.5f, 0.5f, 0.5f);
		Specular = Vec3(0.8f, 0.8f, 0.8f);
		Sun = false;

		physics->SetOwnerTransformComponent(transform);
		BSphere->SetOwnerPhysicsComponent(physics);
	}

	std::shared_ptr<BoundingSphere> GetBSphere() { return BSphere; }
	void DestroyBSphere() {	BSphere = nullptr;	} //shared pointer, will die 

	void SetMaterial(Vec3 AmbientIn, Vec3 DiffuseIn, Vec3 SpecularIn, GLfloat ShineIn)
	{
		Ambient = AmbientIn;
		Diffuse = DiffuseIn;
		Specular = SpecularIn;
		Shine = ShineIn;
		Sun = false;
	}
	//No other values needed for sun
	void SetMaterial(bool IsSun){	Sun = IsSun;	}
	void SetShine(GLfloat ShineIn) { Shine = ShineIn; }
	GLfloat GetShine() { return Shine; }

	std::shared_ptr<Physics> GetPhysics() { return physics; }
	std::shared_ptr<Transform> GetTransform() { return transform; }
	void AssignMesh(std::shared_ptr<Mesh> MeshIn) {	mesh = MeshIn;	}
	void AssignPhysics(std::shared_ptr<Physics> PhysicsIn) {	physics = PhysicsIn;	}
	
	void AssignTransform(std::shared_ptr<Transform> TheTransform) {	transform = TheTransform; physics->SetOwnerTransformComponent(transform); BSphere->SetOwnerPhysicsComponent(physics);
	}

	void Render(Matrix4 ParentMatrix)
	{
		if (physics) { physics->Update(); }
		//if (transform) { transform->Update(); }//no longer used
		if (BSphere) { BSphere->SetPoint(GetTransform()->GetPosition()); }

		transform->SetMatrix(ParentMatrix); 

		Matrix4 ThisMatrix = ParentMatrix;

		SetUniforms();

		if (mesh) 
		{
			mesh->RenderMesh();
		}
		else { std::cout << "Mesh is nullptr. Object class. (No way to ID the Object yet)." << std::endl; }

		//do same for children. Recursive
		for (size_t i = 0; i < SubObjects.size(); i++)
		{
			Render::ModelMatrix = ThisMatrix;
			SubObjects.at(i)->Render(Render::ModelMatrix); //So each child uses this objects matrix.
		}
	}
	void AddSubObject(std::shared_ptr<Mesh> MeshToAssign)
	{
		SubObjects.push_back(std::make_shared<Object>());
		SubObjects.at(SubObjects.size() - 1)->AssignMesh(MeshToAssign);
	}

	std::shared_ptr<Object> GetLastSubObject()
	{
		if (SubObjects.size() ==  0)
		{
			std::cout << "No sub objects in this obj" << std::endl;
			return nullptr;
		}
		else
		{
			return SubObjects.at(SubObjects.size() - 1);
		}
	}

	std::shared_ptr<Object> GetSubObject(GLuint SubObjectsID)
	{
		if (SubObjects.size() - 1 < SubObjectsID)
		{
			std::cout << "Error getting SubObjects in object. Last ID is " << SubObjects.size() - 1 << ". Id requested is " << SubObjectsID << std::endl;
			return nullptr;
		}
		else 
		{
			return SubObjects.at(SubObjectsID);
		}
	}

private:

	//Colour and shine
	//Vec3 Colour;

	//Material
	Vec3 Ambient, Diffuse, Specular;
	float Shine;
	bool Sun;
	std::shared_ptr<Transform> transform;
	std::shared_ptr<Physics> physics;
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Shader> shader;
	std::vector<std::shared_ptr<Object>> SubObjects;
	std::shared_ptr<BoundingSphere> BSphere;

	void SetUniforms()
	{
		Render::shader.SetUniformMatrix4F("Model", Render::ModelMatrix.Values, true);
		Render::shader.SetUniformBool("IsSun", Sun);
		Render::shader.SetUniform1F("Material.Shine", Shine);
		Render::shader.SetUniform3F("Material.Ambient", Ambient.x, Ambient.y, Ambient.z);
		Render::shader.SetUniform3F("Material.Diffuse", Diffuse.x, Diffuse.y, Diffuse.z);
		Render::shader.SetUniform3F("Material.Specular", Specular.x, Specular.y, Specular.z);
	}
};

