#include "MyMesh.h"
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue,-fValue, fValue); //0
	vector3 point1( fValue,-fValue, fValue); //1
	vector3 point2( fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue,-fValue,-fValue); //4
	vector3 point5( fValue,-fValue,-fValue); //5
	vector3 point6( fValue, fValue,-fValue); //6
	vector3 point7(-fValue, fValue,-fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	float fValue = a_fHeight * 0.5f;
	vector3 center(0,-fValue,0); //center of base
	vector3 top(0, fValue, 0); //top point
	std::vector<vector3> base; //holds points at base of cone

	//Generate points on base of cone
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//Find the point on the circle (2D)
		float x = center.x + a_fRadius * glm::cos((i + 1) * glm::pi<float>() * 2 / a_nSubdivisions);
		float z = center.z + a_fRadius * glm::sin((i + 1) * glm::pi<float>() * 2 / a_nSubdivisions);

		//Add it to our list
		base.push_back(vector3(x, -fValue, z));
	}

	//Connect Shape
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//Special Case: drawing last triangle
		if (i == a_nSubdivisions - 1)
		{
			//Add triangle on Base (go counter-clockwise)
			AddTri(base[i], base[0], center);

			//Add triangle on Sides (go clockwise)
			AddTri(base[0], base[i], top);
		}
		else
		{
			//Add triangle on Base (go counter-clockwise)
			AddTri(base[i], base[i + 1], center);

			//Add triangle on Sides (go clockwise)
			AddTri(base[i + 1], base[i], top);
		}
	}


	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	float fValue = a_fHeight * 0.5f;
	vector3 centerBase(0, -fValue, 0); //center of base
	vector3 centerTop(0, fValue, 0); //center of top
	std::vector<vector3> base; //holds points at base of cylinder
	std::vector<vector3> top; //holds points at top of cylinder

	//Generate points on top and base of cylinder
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//Find the point on the circle (2D)
		float x = centerBase.x + a_fRadius * glm::cos((i + 1) * glm::pi<float>() * 2 / a_nSubdivisions);
		float z = centerBase.z + a_fRadius * glm::sin((i + 1) * glm::pi<float>() * 2 / a_nSubdivisions);

		//Add it to our list
		base.push_back(vector3(x, -fValue, z));
		top.push_back(vector3(x, fValue, z));
	}

	//Connect Shape
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//Special Case: drawing last triangle
		if (i == a_nSubdivisions - 1)
		{
			//Add triangle on Base (go counter-clockwise)
			AddTri(base[i], base[0], centerBase);

			//Add triangle on Top (go clockwise)
			AddTri(top[0], top[i], centerTop);

			//Add quad on Sides (go counter-clockwise)
			AddQuad(top[i], top[0], base[i], base[0]);

			//or

			//AddQuad(base[0], base[i], top[0], top[i]);
		}
		else
		{
			//Add triangle on Base (go counter-clockwise)
			AddTri(base[i], base[i + 1], centerBase);

			//Add triangle on Top (go clockwise)
			AddTri(top[i + 1], top[i], centerTop);

			//Add quad on Sides (go counter-clockwise)
			AddQuad(top[i], top[i + 1], base[i], base[i + 1]);

			//or

			//AddQuad(base[i + 1], base[i], top[i + 1], top[i]);
		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Replace this with your code
	float fValue = a_fHeight * 0.5f;
	float radiusScale = a_fOuterRadius / a_fInnerRadius; //scale to scale points to outer radius from inner radius
	std::vector<vector3> base; //holds points at base of tube
	std::vector<vector3> top; //holds points at top of tube

	//Generate points on top and base of tube (according to inner radius).
	//We'll multiply the x and z by the radiusScale to get the position
	//of the points on the outer radius.
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//Find the point on the circle (2D)
		float x = a_fInnerRadius * glm::cos((i + 1) * glm::pi<float>() * 2 / a_nSubdivisions);
		float z = a_fInnerRadius * glm::sin((i + 1) * glm::pi<float>() * 2 / a_nSubdivisions);

		//Add it to our list
		base.push_back(vector3(x, -fValue, z));
		top.push_back(vector3(x, fValue, z));
	}

	//Connect Shape
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//Create temps for outer radius
		vector3 outerCurrBase(base[i].x * radiusScale, base[i].y, base[i].z * radiusScale);
		vector3 outerCurrTop(top[i].x * radiusScale, top[i].y, top[i].z * radiusScale);

		//Special Case: drawing last triangle
		if (i == a_nSubdivisions - 1)
		{
			//Set extra temp vectors
			vector3 outerNextBase(base[0].x * radiusScale, base[0].y, base[0].z * radiusScale);
			vector3 outerNextTop(top[0].x * radiusScale, top[0].y, top[0].z * radiusScale);

			//Add quad on Base (go clockwise)
			AddQuad(base[0], base[i], outerNextBase, outerCurrBase);

			//Add quad on Top (go counter-clockwise)
			AddQuad(top[i], top[0], outerCurrTop, outerNextTop);

			//Add quad on Outer Sides (go clockwise)
			AddQuad(outerNextBase, outerCurrBase, outerNextTop, outerCurrTop);

			//Add quad on Inner Sides (go counter-clockwise)
			AddQuad(base[i], base[0], top[i], top[0]);
		}
		else
		{
			//Set extra temp vectors
			vector3 outerNextBase(base[i + 1].x * radiusScale, base[i + 1].y, base[i + 1].z * radiusScale);
			vector3 outerNextTop(top[i + 1].x * radiusScale, top[i + 1].y, top[i + 1].z * radiusScale);

			//Add quad on Base (go clockwise)
			AddQuad(base[i + 1], base[i], outerNextBase, outerCurrBase);

			//Add quad on Top (go counter-clockwise)
			AddQuad(top[i], top[i + 1], outerCurrTop, outerNextTop);

			//Add quad on Outer Sides (go clockwise)
			AddQuad(outerNextBase, outerCurrBase, outerNextTop, outerCurrTop);

			//Add quad on Inner Sides (go counter-clockwise)
			AddQuad(base[i], base[i + 1], top[i], top[i + 1]);
		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// Replace this with your code
	GenerateCube(a_fOuterRadius * 2.0f, a_v3Color);
	// -------------------------------

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 3)
	{
		a_nSubdivisions = 3;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	// Replace this with your code
	vector3 base(0, -a_fRadius, 0); //the bottom point of the sphere
	vector3 top(0, a_fRadius, 0); //the top point of the sphere

	int subDivisionHeight = a_nSubdivisions;
	int subDivisionAxis = a_nSubdivisions;
	std::vector<vector3> curr; //holds points at current sub division of sphere
	std::vector<vector3> next; //holds points at next sub division of sphere

	//Generate points on sphere (according to radius / subDivisions).
	//Immediately draw the geometry after the points are defined.
	for (int i = 0; i < subDivisionHeight - 1; i++)
	{
		//Get point on circle from xy-coordinates
		float xyCircle_y = a_fRadius * glm::sin((glm::pi<float>() / 2 + ((i + 1) * glm::pi<float>() / subDivisionHeight)));
		float xyCircle_x = a_fRadius * glm::cos((glm::pi<float>() / 2 + ((i + 1) * glm::pi<float>() / subDivisionHeight)));

		//Create the radius for the circle on xz-coordinates
		float xzRadius = glm::abs(xyCircle_x);

		//Special Case: Connecting to the top point of the sphere
		if (i == 0)
		{
			//Note: Y is on a circle where the radius stays the same (vertical sub divisions in image have constant radius)
			//Note: X and Z are on a circle where the radius changes (horizonal sub divisions in image have changing radius)
			for (int j = 0; j < subDivisionAxis; j++)
			{
				float xzCircle_x = xzRadius * glm::cos((j + 1) * glm::pi<float>() * 2 / subDivisionAxis);
				float xzCircle_z = xzRadius * glm::sin((j + 1) * glm::pi<float>() * 2 / subDivisionAxis);
				next.push_back(vector3(xzCircle_x, xyCircle_y, xzCircle_z));
			}

			for (int j = 0; j < subDivisionAxis; j++)
			{
				//Special Case: Drawing last triangle
				if (j == subDivisionAxis - 1)
				{
					//Add triangle on Top (go clockwise)
					AddTri(next[0], next[j], top);
				}
				else
				{
					//Add triangle on Top (go clockwise)
					AddTri(next[j + 1], next[j], top);
				}
			}
		}

		//Normal Case: Draw sides of the sphere
		if (i >= 1)
		{
			//Update the current and next vectors accordingly
			curr = next;
			next.clear();

			//Note: Y is on a circle where the radius stays the same (vertical sub divisions in image have constant radius)
			//Note: X and Z are on a circle where the radius changes (horizonal sub divisions in image have changing radius)
			for (int j = 0; j < subDivisionAxis; j++)
			{
				float xzCircle_x = xzRadius * glm::cos((j + 1) * glm::pi<float>() * 2 / subDivisionAxis);
				float xzCircle_z = xzRadius * glm::sin((j + 1) * glm::pi<float>() * 2 / subDivisionAxis);
				next.push_back(vector3(xzCircle_x, xyCircle_y, xzCircle_z));
			}

			for (int j = 0; j < subDivisionAxis; j++)
			{
				//Special Case: Drawing last triangle
				if (j == subDivisionAxis - 1)
				{
					//Add triangle on Top (go clockwise)
					AddQuad(next[0], next[j], curr[0], curr[j]);
				}
				else
				{
					//Add triangle on Top (go clockwise)
					AddQuad(next[j + 1], next[j], curr[j + 1], curr[j]);
				}
			}
		}

		
		//Special Case: Connection to the bottom point of the sphere
		if (i == a_nSubdivisions - 2)
		{
			//Note: Y is on a circle where the radius stays the same (vertical sub divisions in image have constant radius)
			//Note: X and Z are on a circle where the radius changes (horizonal sub divisions in image have changing radius)
			for (int j = 0; j < subDivisionAxis; j++)
			{
				//Special Case: Drawing last triangle
				if (j == subDivisionAxis - 1)
				{
					//Add triangle on Top (go counter-clockwise)
					AddTri(next[j], next[0], base);
				}
				else
				{
					//Add triangle on Top (go counter-clockwise)
					AddTri(next[j], next[j + 1], base);
				}
			}
		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}