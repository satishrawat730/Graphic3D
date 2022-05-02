#pragma once
//imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <stdio.h>
#include <memory>
#include <vector>

#include "Scene.h"

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

class GLAppUI
{
public:
	GLAppUI() = default;
	GLAppUI(GLFWwindow* pWind, std::shared_ptr<GLScene> pSc);	
	GLAppUI(GLFWwindow* pWind, std::shared_ptr<GLScene> pSc, std::vector<std::string> models);
	bool DisplayElements();	
	void DrawElements();
	void Render();

	// terrain properties 
	float GetTerrainHeightScale() { return terrainHeight; }
	float GetTerrainGridScale() { return terrainGridScale; }
	bool GetTerrainWireMode() { return bTerrainWireframe; }	
	
	int selectedModel;
	int selectedRenderingModel;
	std::vector <std::string> modelPaths;
	void SetLogVector(std::vector<std::string> vec);
	bool show_instancing();

private:	
	static bool display_popup_once;
	static std::vector<std::string> str_vec;
	bool show_console_window = false;
	bool add_logs_to_ui = true;
	bool show_instancing_of_model = false;
	void initialize();
	void CreateMainUIWindow();
	bool CreateModelDropDown(); //returns true if selection has changed
	void CreateRenderingModelDropDown();
	void CreatePopup(const char *msg);
	void CreateTerrainUI();
	void CreateConsoleUIWindow(bool* p_open);

	bool show_demo_window = false;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);	
	GLFWwindow* pWindow;
	std::shared_ptr<GLScene> pScene;
	
	// Terrain related values
	bool bTerrainWireframe{ false };
	float terrainHeight{ 30.f };
	float terrainGridScale{ 1.f };
};
