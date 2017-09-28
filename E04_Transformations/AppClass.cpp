#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "Nicholas Cato - nac5961@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));
	
	//init the mesh
	//m_pMesh = new MyMesh();
	//m_pMesh->GenerateCube(1.0f, C_WHITE);
	//m_pMesh->GenerateSphere(1.0f, 5, C_WHITE);

	//Initialize all of the meshes as cubes and store it
	for (int i = 0; i < 46; i++)
	{
		MyMesh* m_pCubeMesh = new MyMesh();
		m_pCubeMesh->GenerateCube(1.0f, C_BLACK);
		m_pMeshList.push_back(m_pCubeMesh);
	}
	
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	
	//matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(2.0f,2.0f,2.0f));
	static float value = 0.0f;
	//matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(value, 2.0f, 3.0f));
	value += 0.01f;

	//matrix4 m4Model = m4Translate * m4Scale;
	//matrix4 m4Model = m4Scale * m4Translate;
	

	//m_pMesh->Render(m4Projection, m4View, m4Model);

	int cubePosition = -5; //distance from the origin
	int vectorPosition = 0; //counter to keep track of what mesh we're using in the list

	//Render all of the meshes
	for (int i = 0; i < 8; i++) //rows
	{
		for (int j = 0; j < 11; j++) //columns
		{
			//Set the position of the model
			matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(cubePosition + value, (float)i, 0.0f));
			matrix4 m4Model = m4Translate;

			//Note: Row order starts from the bottom
			//First row (4 cubes)
			if (i == 0)
			{
				if (j == 3 || j == 4 || j == 6 || j == 7)
				{
					//Render the object
					m_pMeshList[vectorPosition]->Render(m4Projection, m4View, m4Model);
					vectorPosition++;
				}
			}

			//Second row (4 cubes)
			if (i == 1)
			{
				if (j == 0 || j == 2 || j == 8 || j == 10)
				{
					//Render the object
					m_pMeshList[vectorPosition]->Render(m4Projection, m4View, m4Model);
					vectorPosition++;
				}
			}

			//Third row (9 cubes)
			if (i == 2)
			{
				if (j == 1 || j == 9)
				{
					//do nothing
				}
				else
				{
					//Render the object
					m_pMeshList[vectorPosition]->Render(m4Projection, m4View, m4Model);
					vectorPosition++;
				}
			}

			//Fourth row (11 cubes)
			if (i == 3)
			{
				//Render the object
				m_pMeshList[vectorPosition]->Render(m4Projection, m4View, m4Model);
				vectorPosition++;
			}

			//Fifth row (7 cubes)
			if (i == 4)
			{
				if (j == 0 || j == 3 || j == 7 || j == 10)
				{
					//do nothing
				}
				else
				{
					//Render the object
					m_pMeshList[vectorPosition]->Render(m4Projection, m4View, m4Model);
					vectorPosition++;
				}
			}

			//Sixth row (7 cubes)
			if (i == 5)
			{
				if (j == 0 || j == 1 || j == 9 || j == 10)
				{
					//do nothing
				}
				else
				{
					//Render the object
					m_pMeshList[vectorPosition]->Render(m4Projection, m4View, m4Model);
					vectorPosition++;
				}
			}

			//Seventh row (2 cubes)
			if (i == 6)
			{
				if (j == 3 || j == 7)
				{
					//Render the object
					m_pMeshList[vectorPosition]->Render(m4Projection, m4View, m4Model);
					vectorPosition++;
				}
			}

			//Last row (2 cubes)
			if (i == 7)
			{
				if (j == 2 || j == 8)
				{
					//Render the object
					m_pMeshList[vectorPosition]->Render(m4Projection, m4View, m4Model);
					vectorPosition++;
				}
			}

			//Increment the column position
			cubePosition++;
		}

		//Reset to go back to the start of each row (first column)
		cubePosition = -5;
	}
	
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//SafeDelete(m_pMesh);
	for (int i = 0; i < 46; i++)
	{
		SafeDelete(m_pMeshList[i]);
	}

	//release GUI
	ShutdownGUI();
}