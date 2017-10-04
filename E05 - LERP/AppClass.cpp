#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	//m_sProgrammer = "Alberto Bobadilla - labigm@rit.edu";

	////Alberto needed this at this position for software recording.
	//m_pWindow->setPosition(sf::Vector2i(710, 0));

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUp(vector3(5.0f,3.0f,15.0f), ZERO_V3, AXIS_Y);

	m_pModel = new Simplex::Model();
	m_pModel->Load("Sorted\\WallEye.bto");
	
	m_stopsList.push_back(vector3(-4.0f, -2.0f, 5.0f));
	m_stopsList.push_back(vector3(1.0f, -2.0f, 5.0f));

	m_stopsList.push_back(vector3(-3.0f, -1.0f, 3.0f));
	m_stopsList.push_back(vector3(2.0f, -1.0f, 3.0f));

	m_stopsList.push_back(vector3(-2.0f, 0.0f, 0.0f));
	m_stopsList.push_back(vector3(3.0f, 0.0f, 0.0f));

	m_stopsList.push_back(vector3(-1.0f, 1.0f, -3.0f));
	m_stopsList.push_back(vector3(4.0f, 1.0f, -3.0f));

	m_stopsList.push_back(vector3(0.0f, 2.0f, -5.0f));
	m_stopsList.push_back(vector3(5.0f, 2.0f, -5.0f));

	m_stopsList.push_back(vector3(1.0f, 3.0f, -5.0f));

	
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

	//Set the view and projection matrix
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();

	// Draw the model
	m_pModel->PlaySequence();

	//Get a timer
	static float fTimer = 0;	//store the new timer
	static uint uClock = m_pSystem->GenClock(); //generate a new clock for that timer
	fTimer += m_pSystem->GetDeltaTime(uClock); //get the delta time for that timer

	//calculate the current position
	vector3 v3CurrentPos;

	//your code goes here

	//float to set the max value of fTimer
	float fMax = 1.0f;

	//float to keep track of current position in vector
	static unsigned int m_currentPos = 0;

	//bool to check if the lerping has initially started
	static bool start = true;

	//Lerp to beginning position at start
	if (start == true)
	{
		v3CurrentPos = vector3(0, 0, 0);
		v3CurrentPos = glm::lerp(v3CurrentPos, m_stopsList[0], 1.0f);
		start = false;
	}

	//Clamp fTimer
	if (fTimer >= fMax)
	{
		fTimer = fMax;
	}

	//Lerp from one stop to the next
	if (m_currentPos < m_stopsList.size() - 1)
	{
		v3CurrentPos = glm::lerp(m_stopsList[m_currentPos], m_stopsList[m_currentPos + 1], fTimer);
	}

	//Special Case: Lerp from last stop to beginning stop
	else
	{
		v3CurrentPos = glm::lerp(m_stopsList[m_stopsList.size() - 1], m_stopsList[0], fTimer);
	}

	//Reset fTimer to lerp again
	//Update the current stop
	if (fTimer >= fMax)
	{
		fTimer = 0.0f;
		m_currentPos++;

		if (m_currentPos >= m_stopsList.size())
		{
			m_currentPos = 0;
		}
	}


	
	matrix4 m4Model = glm::translate(IDENTITY_M4, v3CurrentPos);
	m_pModel->SetModelMatrix(m4Model);

	m_pMeshMngr->Print("\nTimer: ");//Add a line on top
	m_pMeshMngr->PrintLine(std::to_string(fTimer), C_YELLOW);

	//m_pMesh->Render(m4Projection, m4View, m4Model);

	// Draw stops
	for (uint i = 0; i < m_stopsList.size(); ++i)
	{
		m_pMeshMngr->AddSphereToRenderList(glm::translate(m_stopsList[i]) * glm::scale(vector3(0.05f)), C_GREEN, RENDER_WIRE);
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
	SafeDelete(m_pModel);
	//release GUI
	ShutdownGUI();
}