#pragma once

#include <memory>
#include <vector>
#include "Globals.h"

#include "Object.h"
class World
{
public:
	World() 
	{

	}
	void AddObject()
	{
		ObjectList.push_back(std::make_shared<Object>());
	}
	std::shared_ptr<Object> AddObjectAndGetPtr()
	{
		ObjectList.push_back(std::make_shared<Object>());
		return GetLastObject();
	}
	void AddObject(GLuint IDofMeshToAssignToObject)
	{
		ObjectList.push_back(std::make_shared<Object>());
		GetLastObject()->AssignMesh(GetMeshAt(IDofMeshToAssignToObject));
	}
	std::shared_ptr<Object> AddObjectAndGetPtr(GLuint ID)
	{
		ObjectList.push_back(std::make_shared<Object>());
		GetLastObject()->AssignMesh(GetMeshAt(ID));
		return GetLastObject();
	}
	std::shared_ptr<Object> GetLastObject() { return ObjectList.at(ObjectList.size() - 1); }
	std::shared_ptr<Object> GetObjectAt(GLuint Value)
	{
		if (ObjectList.size() - 1 < Value)
		{
			std::cout << "Error getting Object at " << Value << ". Last Object in list is " << ObjectList.size() - 1<< std::endl;
			return nullptr;
		}
		else
		{
			return ObjectList.at(Value);
		}

	}
	GLuint GetObjectListSize() { return ObjectList.size(); }

	void AddMesh()
	{
		MeshList.push_back(std::make_shared<Mesh>());
	}
	std::shared_ptr<Mesh> GetLastMesh() { return MeshList.at(MeshList.size() - 1); }
	std::shared_ptr<Mesh> GetMeshAt(GLuint Value)
	{
		if (MeshList.size() - 1 < Value)
		{
			std::cout << "error getting mesh at " << Value << ". Last mesh is " << MeshList.size() - 1 << std::endl;
			return nullptr;
		}
		else return MeshList.at(Value);
	}

private:

	std::vector<std::shared_ptr<Object>> ObjectList;
	std::vector<std::shared_ptr<Mesh>> MeshList;

};

