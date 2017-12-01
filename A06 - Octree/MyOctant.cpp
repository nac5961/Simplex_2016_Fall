#include "MyOctant.h"
using namespace Simplex;

Simplex::MyOctant::MyOctant(uint a_uMaxLevel)
{
}

Simplex::MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
}

float Simplex::MyOctant::GetSize()
{
	return 0.0f;
}

vector3 Simplex::MyOctant::GetCenterGlobal()
{
	return vector3();
}

vector3 Simplex::MyOctant::GetMinGlobal()
{
	return vector3();
}

vector3 Simplex::MyOctant::GetMaxGlobal()
{
	return vector3();
}

bool Simplex::MyOctant::IsColliding(uint a_uIndex)
{
	return false;
}

void Simplex::MyOctant::Display()
{
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
	return nullptr;
}

MyOctant * Simplex::MyOctant::GetParent()
{
	return nullptr;
}

bool Simplex::MyOctant::IsLeaf()
{
	return false;
}

uint Simplex::MyOctant::GetOctantCount()
{
	return uint();
}

void Simplex::MyOctant::ContructTree(uint a_uMaxLevel)
{
}

void Simplex::MyOctant::AssignIDtoEntity()
{
}
