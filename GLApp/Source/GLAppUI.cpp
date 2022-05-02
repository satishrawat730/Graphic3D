#include "GLAppUI.h"
#include "Common.h"

std::vector<std::string> GLAppUI::str_vec = {};
bool GLAppUI::display_popup_once = false;
GLAppUI::GLAppUI(GLFWwindow* pWind, std::shared_ptr<GLScene> pSc)
{
	pScene = pSc;
	pWindow = pWind;
	initialize();
}

GLAppUI::GLAppUI(GLFWwindow* pWind, std::shared_ptr<GLScene> pSc, std::vector <std::string> models)
{
	pScene = pSc;
	pWindow = pWind;
	initialize();
	modelPaths = models;
}

void GLAppUI::initialize()
{
	const char* glsl_version = "#version 100";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(pWindow, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Our state
	bool show_demo_window = false;
	bool show_another_window = false;
	bool show_instancing_of_model = false;
	clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
	selectedModel = 0;
	selectedRenderingModel = 0;
}

void GLAppUI::DrawElements()
{
	int display_w, display_h;
	glfwGetFramebufferSize(pWindow, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GLAppUI::CreateMainUIWindow()
{
	static float f = 0.0f;
	static int counter = 0;

	ImGui::Begin("CONTROLS", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);  // Create a window called "CONTROLS" and append into it.

	float curr_window_x = 0;
	float curr_window_y = /*main_window_pos_height - main_window_pos.y +*/0;
	ImVec2 curr_window_pos(curr_window_x, curr_window_y);
	ImGui::SetWindowPos(curr_window_pos);
   /* if (ImGui::BeginMenu("Menu"))
    {
        if (ImGui::MenuItem("Open Popup"))
        {
            display_popup_once = true;
        }
        ImGui::EndMenu();
    }
    if (display_popup_once)
    {
        CreatePopup("This is sample popup");
    }*/
	ImGui::Checkbox("Demo Window", &show_demo_window);  
    
    
	ImGui::SameLine();
	ImGui::Checkbox("Console Window", &show_console_window);    // Edit bools storing our window open/close state	

	ImGui::Checkbox("Instancing", &show_instancing_of_model);

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
	ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;

	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);	
}

struct AppLog
{
    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
    bool                AutoScroll;  // Keep scrolling if already at the bottom.

    AppLog()
    {
        AutoScroll = true;
        Clear();
    }

    void Clear()
    {
        Buf.clear();
        LineOffsets.clear();
        LineOffsets.push_back(0);
    }

    void AddLog(const char* fmt, ...) IM_FMTARGS(2)
    {
        int old_size = Buf.size();
        va_list args;
        va_start(args, fmt);
        Buf.appendfv(fmt, args);
        va_end(args);
        for (int new_size = Buf.size(); old_size < new_size; old_size++)
            if (Buf[old_size] == '\n')
                LineOffsets.push_back(old_size + 1);
    }

    void Draw(const char* title, bool* p_open = NULL)
    {
        if (ImGui::BeginPopup("Options"))
        {
            ImGui::Checkbox("Auto-scroll", &AutoScroll);
            ImGui::EndPopup();
        }

        // Main window
        if (ImGui::Button("Options"))
            ImGui::OpenPopup("Options");
        ImGui::SameLine();
        bool clear = ImGui::Button("Clear");
        ImGui::SameLine();
        bool copy = ImGui::Button("Copy");
        ImGui::SameLine();
        Filter.Draw("Filter", -100.0f);

        ImGui::Separator();
        ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        if (clear)
            Clear();
        if (copy)
            ImGui::LogToClipboard();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        const char* buf = Buf.begin();
        const char* buf_end = Buf.end();
        if (Filter.IsActive())
        {
            for (int line_no = 0; line_no < LineOffsets.Size; line_no++)
            {
                const char* line_start = buf + LineOffsets[line_no];
                const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                if (Filter.PassFilter(line_start, line_end))
                    ImGui::TextUnformatted(line_start, line_end);
            }
        }
        else
        {
            ImGuiListClipper clipper;
            clipper.Begin(LineOffsets.Size);
            while (clipper.Step())
            {
                for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                {
                    const char* line_start = buf + LineOffsets[line_no];
                    const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
                    ImGui::TextUnformatted(line_start, line_end);
                }
            }
            clipper.End();
        }
        ImGui::PopStyleVar();

        if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();
    }
};

void GLAppUI::CreateConsoleUIWindow(bool* p_open)
{
    std::string str;
    int display_w, display_h;
    glfwGetFramebufferSize(pWindow, &display_w, &display_h);
   //glfwGetWindowSize(pWindow,&display_w,&display_h);
     
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    //console_height_value = ImGui::GetWindowHeight();
    float window_height = (float(display_h) / 4);
    float y_value = (display_h - window_height);
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, y_value));
    ImGui::SetNextWindowSize(ImVec2(float(display_w), window_height));
   
    ImGui::Begin("Console", p_open, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize);     // Create a window called "Console" and append into it.

    ImGui::SetWindowSize("Console", ImVec2(float(display_w), window_height));
    //printf("main_viewport->WorkPos.y =%d \n", main_viewport->WorkPos.y);
    ImGui::SetWindowPos(ImVec2(main_viewport->WorkPos.x, y_value));

    static AppLog log;

    if (add_logs_to_ui)
    {
        //std::cout << "Size of vector =" << GLAppUI::str_vec.size() << std::endl;
        for (auto n = GLAppUI::str_vec.begin(); n != str_vec.end(); ++n)
        {
            log.AddLog((*n).c_str());
        }
        add_logs_to_ui = false;
    }
    log.Draw("Console", p_open);
    ImGui::End();
}

void GLAppUI::SetLogVector(std::vector<std::string> vec)
{
    GLAppUI::str_vec = vec;
}

bool GLAppUI::CreateModelDropDown()
{

	short int prevSelectedModel = selectedModel;

	//static const char* models[]{ "SELECT MODEL:", "Teapot", "car", "Sphere", "Cube", "Stylized_Bucket", "ship", "Multicolored", "tree_blender_file" };
	static const char* models[]{ "Brown rock", "Carpet", "Dented metal", "Gold", "Granite", "Leather", "Rusted Iron", "Knife"};
	ImGui::Combo("Subject Model", &selectedModel, models, IM_ARRAYSIZE(models));	
	return selectedModel != prevSelectedModel ? true: false;
}

void GLAppUI::CreateRenderingModelDropDown()
{	
	static const char* renderingTypes[]{ "SELECT RENDERING MODEL:", "Regular Diffuse", "Normal Maps", "Multi Texture"};
	ImGui::Combo("Render Type", &selectedRenderingModel, renderingTypes, IM_ARRAYSIZE(renderingTypes));
}

void GLAppUI::CreatePopup(const char *msg) //Sample popup. To be replaced as required
{
    int display_w, display_h;
    glfwGetFramebufferSize(pWindow, &display_w, &display_h);
    
    if (display_popup_once)
    {
        ImGui::OpenPopup("Error", ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);
    }
    float popup_w = float(display_w/4);
    float popup_h = float(display_h/5);
    auto text_w = ImGui::CalcTextSize(msg).x;
    ImGui::SetNextWindowSize(ImVec2(popup_w,popup_h));
    float text_x = (popup_w/2) - (text_w/2);
    
        if (ImGui::BeginPopupModal("Error", NULL,ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings))
        { 
            ImGui::SetCursorPosX(text_x);
            ImGui::Text(msg);
            ImGui::Separator();
            ImGui::SetCursorPos(ImVec2((popup_w/2)- ((popup_w/5)/2), popup_h-(popup_h/4)));
            if (ImGui::Button("Close",ImVec2(popup_w/5, popup_h/5)))
            {
                display_popup_once = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
}

void GLAppUI::CreateTerrainUI()
{
	ImGui::Text("");
	ImGui::Text(" Terrain Settings ");
	ImGui::Checkbox("Terrain Wireframe Mode", &bTerrainWireframe);
	ImGui::SliderFloat(" Terrain Height Scale ", &terrainHeight, 0.0f, 200.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
	ImGui::SliderFloat(" Terrain Grid Size ", &terrainGridScale, 0.1f, 2.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
	ImGui::Text("");
}

bool GLAppUI::DisplayElements()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
         ImGui::ShowDemoWindow(&show_demo_window);

    if (show_console_window)
         CreateConsoleUIWindow(&show_console_window);

	// Show a simple window that we create ourselves. We use a Begin/End pair to created a named window
	CreateMainUIWindow();

	ImGui::Text("");
	// Drop down list for model	
	bool modelChanged = CreateModelDropDown();
	
	//Drop down list for Rendering model type
	CreateRenderingModelDropDown();

	// Terrain UI
	CreateTerrainUI();

	//Sample popup creation. Uncomment to create popup on the first model change
	/*if (selectedModel != 0)
		CreatePopup();*/
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();
	return modelChanged;
}

void GLAppUI::Render()
{
	// Rendering
	ImGui::Render();
}

bool GLAppUI::show_instancing()
{
	return show_instancing_of_model;
}
