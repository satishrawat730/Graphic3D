#pragma once
// standard header -----------
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// 3rd Party Lib -------------
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

// Engine header files ------
#include "Common.h"
#include "ElementFactory.h"
#include "Fbo.h"
#include "GLAppUI.h"
#include "Light.h"
#include "LogManager.h"
#include "Material.h"
#include "Mesh.h"
#include "ModelLoader.h"
#include "PerspectiveCamera.h"
#include "Scene.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "Terrain.h"
#include "Texture.h"

namespace RE 
{
    class RocketEngine 
    {
        static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
        static void WindowSizeCallback(GLFWwindow* window, int width, int height);
        static void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

        static const std::vector<std::string> mFiles;

        static unsigned int _WinWidth, _WinHeight;
        static double lastX, lastY, xChange, yChange;
        static bool _IsFirstMovement;
        static int _ActiveModelIndex;
        static bool _IsNormalMappingEnabled;
        static vector<int> keyPresses;
        static float _rt;

        const unsigned int _GL_Version{ 4 };

    public:
        RocketEngine() = default;
        ~RocketEngine() = default;

        void Intialize(); // intialize and load here
        void Run(); // event , update, render loop here
        void Release(); // free, relaease memory resources here

        // getter setter 's 
        GLFWwindow* GetWindow() { return _Window; }
        unsigned int GetWinWidth() { return _WinWidth; }
        unsigned int GetWinHeight() { return _WinHeight; }
        void SetWindow(unsigned int w, unsigned int h);

        int GetBufferWidth() { return _BufferWidth; }
        int GetBufferHeight() { return _BufferHeight; }
        void SetBufferSize(unsigned int w, unsigned int h);

        shared_ptr<PerspectiveCamera> GetCamera() { return _Camera; }
        shared_ptr<ShaderManager> GetShader() { return _Shader; }
        shared_ptr<DirectionalLight> GetDirectionLight() { return _DirLight; }
        shared_ptr<PointLight> GetPointLight() { return _PointLight; }
        shared_ptr<GLScene> GetScene() { return _Scene; }
        shared_ptr<Fbo> GetFBO() { return _MainFBO; }

      private:
        void EventHandling();
        void Update();
        void Render();

        int CreateAppWindow();
        void CreateCamera();
        void CreateTerrain();
        void CreateLight();

        void UpdateCameraPosition();
        void UpdateCameraRotation();
        void UpdateCameraAspectRation();
        void UpdateLightPosition();

        void DrawTerrain();

        GLFWwindow* _Window{ nullptr };
        bool _bQuit{ true };

        
        int _BufferWidth{ 0 }, _BufferHeight{ 0 };
        
        double _DeltaTime = 0.0, _LastTime = 0.0;
        

        string _ActiveModel;

        glm::vec3 _PointLightPreviousPosition{ 0.0f, 0.0f, 0.0f };
        glm::mat4 _LightModel{ 1.0 };


        shared_ptr<Terrain> _Terrain;
        shared_ptr<PerspectiveCamera> _Camera;
        shared_ptr<ShaderManager> _Shader;

        shared_ptr<DirectionalLight> _DirLight;
        shared_ptr<PointLight> _PointLight;
        shared_ptr<GridSystem> _Grid;
        shared_ptr<GLScene> _Scene;
        shared_ptr<Fbo> _MainFBO; 
        LogManager _Log;
        GLAppUI _AppUI;
    };
}