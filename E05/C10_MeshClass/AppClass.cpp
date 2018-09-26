#include "AppClass.h"
void Application::InitVariables(void)
{
	// fill up invader vector with appropriate num of blocks
	for (size_t i = 0; i < 46; i++)
	{
		invader.push_back(new MyMesh());
		invader[i]->GenerateCube(1.0f, C_BLACK);
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

	static float value = -14.0f;

	std::vector<matrix4> translate;
	
	// comments are every 5 to help keep track of blocks
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value, 0.0f, 0.0f))); // 0
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value, 1.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value, 2.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 1.0f, 2.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 1.0f, 3.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 2.0f, 4.0f, 0.0f))); // 5
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 2.0f, 3.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 2.0f, 2.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 2.0f, 1.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 2.0f, 0.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 2.0f, 6.0f, 0.0f))); // 10
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 3.0f, 5.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 3.0f, 4.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 3.0f, 2.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 3.0f, 1.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 3.0f, -1.0f, 0.0f))); // 15
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 4.0f, -1.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 4.0f, 1.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 4.0f, 2.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 4.0f, 3.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 4.0f, 4.0f, 0.0f))); // 20
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 5.0f, 4.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 5.0f, 3.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 5.0f, 2.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 5.0f, 1.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 6.0f, -1.0f, 0.0f))); // 25
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 6.0f, 1.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 6.0f, 2.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 6.0f, 3.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 6.0f, 4.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 7.0f, 5.0f, 0.0f))); // 30
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 7.0f, 4.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 7.0f, 2.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 7.0f, 1.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 7.0f, -1.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 8.0f, 0.0f, 0.0f))); // 35
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 8.0f, 1.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 8.0f, 2.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 8.0f, 3.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 8.0f, 4.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 8.0f, 6.0f, 0.0f))); // 40
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 9.0f, 2.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 9.0f, 3.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 10.0f, 2.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 10.0f, 1.0f, 0.0f)));
	translate.push_back(glm::translate(IDENTITY_M4, vector3(value + 10.0f, 0.0f, 0.0f))); // 45

	// increment value to make invader move
	value += 0.01f;

	// apply corresponding translation to each block
	for (size_t i = 0; i < 46; i++)
		invader[i]->Render(m4Projection, m4View, translate[i]);
		
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
	// delete every mesh pointer
	for each (MyMesh* var in invader)
		SafeDelete(var);

	//release GUI
	ShutdownGUI();
}