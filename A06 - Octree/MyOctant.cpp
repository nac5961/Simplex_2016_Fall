#include "MyOctant.h"
using namespace Simplex;
uint Simplex::MyOctant::m_uMaxLevel = 0;
uint Simplex::MyOctant::m_uOctantCount = 0;

Simplex::MyOctant::MyOctant(uint a_uMaxLevel)
{
	//Set the max subdivision level
	m_uMaxLevel = a_uMaxLevel;

	//Set the singletons
	m_pEntityManager = MyEntityManager::GetInstance();
	m_pMeshManager = MeshManager::GetInstance();

	//Set id and level (base octant)
	m_uLevel = m_uID = 0;

	//Set size to radius of the circle used in appclass.cpp (base octant)
	m_fSize = 68.0f;

	//Set min and max points
	m_v3Max = m_v3Center + vector3(m_fSize);
	m_v3Min = m_v3Center - vector3(m_fSize);

	//Initialize all children to null pointers
	for (uint i = 0; i < 8; i++)
	{
		m_pChildren[i] = nullptr;
	}

	//Start building the octree
	ConstructTree(m_uLevel);

	//Increase total octant count
	m_uOctantCount++;
}

Simplex::MyOctant::MyOctant(MyOctant* a_pParent, vector3 a_v3Center, float a_fSize, uint a_uLevel)
{
	//Set variables
	m_pParent = a_pParent;
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;
	m_uLevel = a_uLevel;

	//Set the singletons
	m_pEntityManager = MyEntityManager::GetInstance();
	m_pMeshManager = MeshManager::GetInstance();

	//Set min and max points
	m_v3Max = m_v3Center + vector3(m_fSize);
	m_v3Min = m_v3Center - vector3(m_fSize);

	//Initialize all children to null pointers
	for (uint i = 0; i < 8; i++)
	{
		m_pChildren[i] = nullptr;
	}

	//Continue building the octree
	ConstructTree(a_uLevel);

	//Increase total octant count
	m_uOctantCount++;
}

float Simplex::MyOctant::GetSize()
{
	return m_fSize;
}

vector3 Simplex::MyOctant::GetCenterGlobal()
{
	return m_v3Center;
}

vector3 Simplex::MyOctant::GetMinGlobal()
{
	return m_v3Min;
}

vector3 Simplex::MyOctant::GetMaxGlobal()
{
	return m_v3Max;
}

bool Simplex::MyOctant::IsColliding(uint a_uIndex)
{
	//Check if the index is valid
	if (a_uIndex < 0 || a_uIndex >= m_pEntityManager->GetEntityCount())
	{
		return false;
	}

	//Get the entity
	MyEntity* entity = m_pEntityManager->GetEntity(a_uIndex);

	//Get globals for max and min points on the entity
	vector3 maxG = entity->GetRigidBody()->GetMaxGlobal();
	vector3 minG = entity->GetRigidBody()->GetMinGlobal();

	//No collisions
	if ((m_v3Min.x > maxG.x || m_v3Max.x < minG.x) && (m_v3Min.y < minG.y || m_v3Min.y > maxG.y) && (m_v3Min.z > maxG.z || m_v3Max.z < minG.z))
	{
		return false;
	}

	//Collisions
	return true;
}

void Simplex::MyOctant::Display()
{
	//Display this octant
	m_pMeshManager->AddWireCubeToRenderList(glm::scale(m_v3Max), C_YELLOW);

	//Display all child octants
	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChildren[i]->Display();
	}
}

void Simplex::MyOctant::Display(uint a_uIndex)
{
}

void Simplex::MyOctant::ClearEntityList()
{
}

void Simplex::MyOctant::Subdivide()
{
}

MyOctant * Simplex::MyOctant::GetChild(uint a_uIndex)
{
	//Check if index is valid
	if (a_uIndex < 0 || a_uIndex >= 8)
	{
		return nullptr;
	}

	return m_pChildren[a_uIndex];
}

MyOctant * Simplex::MyOctant::GetParent()
{
	return m_pParent;
}

bool Simplex::MyOctant::IsLeaf()
{
	//Check if it has children
	if (m_uChildren > 0)
	{
		return false;
	}

	return true;
}

uint Simplex::MyOctant::GetOctantCount()
{
	return m_uOctantCount;
}

void Simplex::MyOctant::ConstructTree(uint a_uLevel)
{
	//If recursively got to max level, stop going further
	if (a_uLevel >= m_uMaxLevel)
	{
		return;
	}

	vector3 offset = ZERO_V3; //offset for child position
	float size = m_fSize / 2.0f; //size of child

	//Create the 8 children recursively
	for (uint i = 0; i < 8; i++)
	{
		if (i & 1)
		{
			offset.x = size;
		}
		else
		{
			offset.x = -size;
		}

		if (i & 2)
		{
			offset.y = size;
		}
		else
		{
			offset.y = -size;
		}

		if (i & 4)
		{
			offset.z = size;
		}
		else
		{
			offset.z = -size;
		}

		//Create child (Parameters: parent, center, size, level)
		m_pChildren[i] = new MyOctant(this, m_v3Center + offset, size, a_uLevel + 1);
	}

	//Set number of children
	m_uChildren = 8;
}

void Simplex::MyOctant::AssignIDtoEntity()
{
	for (int i = 0; i < m_pEntityManager->GetEntityCount(); i++)
	{
		//Ignore entities already
		if (IsColliding(i))
		{
			
		}
	}
}