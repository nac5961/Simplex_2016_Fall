#include "MyEntityManager.h"
using namespace Simplex;
//  MyEntityManager
MyEntityManager* MyEntityManager::m_pInstance = nullptr;
void MyEntityManager::Init(void)
{
	//Initialize variables
	m_uEntityCount = 0;
}
void MyEntityManager::Release(void)
{
	//Release all of the entities
	for (uint i = 0; i < m_uEntityCount; i++)
	{
		SafeDelete(m_entityList[i]);
	}

	//Clear the list
	m_entityList.clear();

	//Set the entity count to 0
	m_uEntityCount = 0;
}
MyEntityManager* MyEntityManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new MyEntityManager();
	}
	return m_pInstance;
}
void MyEntityManager::ReleaseInstance()
{
	//Release the entity manager
	SafeDelete(m_pInstance);
}
int Simplex::MyEntityManager::GetEntityIndex(String a_sUniqueID)
{
	for (uint i = 0; i < m_uEntityCount; i++)
	{
		//Check each id for a match
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID)
		{
			//Return the index
			return i;
		}
	}
	return -1;
}
//Accessors
Model* Simplex::MyEntityManager::GetModel(uint a_uIndex)
{
	//Invalid index
	if (a_uIndex < 0 || a_uIndex >= m_uEntityCount)
	{
		return nullptr;
	}

	//Return the model
	return m_entityList[a_uIndex]->GetModel();
	
	//return nullptr;
}
Model* Simplex::MyEntityManager::GetModel(String a_sUniqueID)
{
	for (uint i = 0; i < m_uEntityCount; i++)
	{
		//Check each id for a match
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID)
		{
			//Return the model
			return m_entityList[i]->GetModel();
		}
	}
	return nullptr;
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(uint a_uIndex)
{
	//Invalid index
	if (a_uIndex < 0 || a_uIndex >= m_uEntityCount)
	{
		return nullptr;
	}

	//Return the rigidbody
	return m_entityList[a_uIndex]->GetRigidBody();
	
	//return nullptr;
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(String a_sUniqueID)
{
	for (uint i = 0; i < m_uEntityCount; i++)
	{
		//Check each id for a match
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID)
		{
			//Return the rigidbody
			return m_entityList[i]->GetRigidBody();
		}
	}
	return nullptr;
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(uint a_uIndex)
{
	//Invalid index
	if (a_uIndex < 0 || a_uIndex >= m_uEntityCount)
	{
		return IDENTITY_M4;
	}

	return m_entityList[a_uIndex]->GetModelMatrix();

	//return IDENTITY_M4;
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(String a_sUniqueID)
{
	for (uint i = 0; i < m_uEntityCount; i++)
	{
		//Check each id for a match
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID)
		{
			//Return the model matrix
			return m_entityList[i]->GetModelMatrix();
		}
	}

	return IDENTITY_M4;
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, String a_sUniqueID)
{
	for (uint i = 0; i < m_uEntityCount; i++)
	{
		//Check each id for a match
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID)
		{
			//Set the model matrix
			m_entityList[i]->SetModelMatrix(a_m4ToWorld);

			//Stop searching
			break;
		}
	}
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, uint a_uIndex)
{
	//Invalid index
	if (a_uIndex < 0 || a_uIndex >= m_uEntityCount)
	{
		return;
	}

	//Set model matrix
	m_entityList[a_uIndex]->SetModelMatrix(a_m4ToWorld);
}
uint Simplex::MyEntityManager::GetEntityCount() { return m_uEntityCount; }
//The big 3
MyEntityManager::MyEntityManager(){Init();}
MyEntityManager::MyEntityManager(MyEntityManager const& other){ }
MyEntityManager& MyEntityManager::operator=(MyEntityManager const& other) { return *this; }
MyEntityManager::~MyEntityManager(){Release();};
// other methods
void Simplex::MyEntityManager::Update(void)
{
	//Check for collisions
	for (uint i = 0; i < m_uEntityCount; i++)
	{
		for (uint j = 0; j < m_uEntityCount; j++)
		{
			//Ignore collisions with self
			if (i == j)
			{
				continue;
			}

			//Check collisions between objects
			m_entityList[i]->IsColliding(m_entityList[j]);
		}
	}
}
void Simplex::MyEntityManager::AddEntity(String a_sFileName, String a_sUniqueID)
{
	//Create entity
	MyEntity* entity = new MyEntity(a_sFileName, a_sUniqueID);

	//Add it to the list
	m_entityList.push_back(entity);

	//Update the entity count
	m_uEntityCount++;
}
void Simplex::MyEntityManager::RemoveEntity(uint a_uIndex)
{
	//Invalid index
	if (a_uIndex < 0 || a_uIndex >= m_uEntityCount)
	{
		return;
	}

	//Delete the pointer at the index
	SafeDelete(m_entityList[a_uIndex]);

	//Remove the pointer from the list
	m_entityList.erase(m_entityList.begin() + a_uIndex);

	//Update the entity count
	m_uEntityCount--;
}
void Simplex::MyEntityManager::RemoveEntity(String a_sUniqueID)
{
	for (uint i = 0; i < m_uEntityCount; i++)
	{
		//Check each id for a match
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID)
		{
			//Delete the pointer at the index
			SafeDelete(m_entityList[i]);

			//Remove the pointer from the list
			m_entityList.erase(m_entityList.begin() + i);

			//Update the entity count
			m_uEntityCount--;

			//Stop searching
			break;
		}
	}
}
String Simplex::MyEntityManager::GetUniqueID(uint a_uIndex)
{
	//Invalid index
	if (a_uIndex < 0 || a_uIndex >= m_uEntityCount)
	{
		return "";
	}

	//Return the id
	return m_entityList[a_uIndex]->GetUniqueID();

	//return "";
}
MyEntity* Simplex::MyEntityManager::GetEntity(uint a_uIndex)
{
	//Invalid index
	if (a_uIndex < 0 || a_uIndex >= m_uEntityCount)
	{
		return nullptr;
	}

	//Return the entity
	return m_entityList[a_uIndex];

	//return nullptr;
}
void Simplex::MyEntityManager::AddEntityToRenderList(uint a_uIndex, bool a_bRigidBody)
{
	//Invalid index
	if (a_uIndex < 0 || a_uIndex >= m_uEntityCount)
	{
		return;
	}

	//Add to render list
	m_entityList[a_uIndex]->AddToRenderList(a_bRigidBody);
}
void Simplex::MyEntityManager::AddEntityToRenderList(String a_sUniqueID, bool a_bRigidBody)
{
	for (uint i = 0; i < m_uEntityCount; i++)
	{
		//Check each id for a match
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID)
		{
			//Add to render list
			m_entityList[i]->AddToRenderList(a_bRigidBody);

			//Stop searching
			break;
		}
	}
}