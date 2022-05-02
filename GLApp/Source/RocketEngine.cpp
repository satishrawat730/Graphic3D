#include "RocketEngine.h"
namespace RE
{
    unsigned int RocketEngine::_WinWidth = 1366, RocketEngine::_WinHeight = 768;
    double RocketEngine::lastX = 0., RocketEngine::lastY = 0., RocketEngine::xChange = 0., RocketEngine::yChange = 0.;
    bool RocketEngine::_IsFirstMovement = true;
    int RocketEngine::_ActiveModelIndex = 0;
    float RocketEngine::_rt = 0.0f;
    bool RocketEngine::_IsNormalMappingEnabled{ true };
    vector<int> RocketEngine::keyPresses;
    
    const vector<std::string> RocketEngine::mFiles =
    {
        //Constants::Cube,
        //Constants::Teapot,
        //Constants::Sphere,
        //	Constants::pbrSphere,
        Constants::pbrBrownrock,
        // 	Constants::pbrCarpet,
        // 	Constants::pbrDentedMetal,
        // 	Constants::pbrGold,
        // 	Constants::pbrGranite,
        // 	Constants::pbrLeather,
        // 	Constants::pbrRustedIron,
        //	Constants::pbrKnife,
        //	Constants::pbrHelmet,
        //	Constants::pbrShield,
        //	Constants::pbrScar,
    };

    void RocketEngine::Intialize()
    {
        _Log.Init();
        RE_INFO("Example Layer :: Update");
        RE_TRACE("{0}", "This is trace log");
        RE_TRACE("{0}", "This is trace log34");

        // GLFW window creation
        CreateAppWindow();
        // load file

        // load other object
        
        _Shader = ShaderManager::GetInstance();
        _Shader->Initialize();
        _Scene = ElementFactory::CreateScene();
       

        CreateLight();

        CreateTerrain();

        CreateCamera();

        _Grid = ElementFactory::CreateGrid(64, 64, 0.1);
        _MainFBO = std::shared_ptr<Fbo>(new Fbo(_WinWidth, _WinHeight));

        _LightModel = glm::scale(_LightModel, glm::vec3(0.2, 0.2, 0.2));
        _LightModel = glm::translate(_LightModel, _PointLight->GetPosition());

        _AppUI = GLAppUI(_Window, _Scene, mFiles);

        _ActiveModel = _AppUI.modelPaths[_ActiveModelIndex];
        ModelLoader::LoadModel(_AppUI.modelPaths[0]);

        for (auto path : _AppUI.modelPaths)
        {
            auto model = ModelLoader::LoadModel(path);

            if (path.compare(Constants::pbrBrownrock) == 0) { model->SetName(Constants::pbrBrownrock); }
            if (path.compare(Constants::pbrCarpet) == 0) { model->SetName(Constants::pbrCarpet); }
            if (path.compare(Constants::pbrDentedMetal) == 0) { model->SetName(Constants::pbrDentedMetal); }
            if (path.compare(Constants::pbrGold) == 0) { model->SetName(Constants::pbrGold); }
            if (path.compare(Constants::pbrGranite) == 0) { model->SetName(Constants::pbrGranite); }
            if (path.compare(Constants::pbrLeather) == 0) { model->SetName(Constants::pbrLeather); }
            if (path.compare(Constants::pbrRustedIron) == 0) { model->SetName(Constants::pbrRustedIron); }
            if (path.compare(Constants::pbrKnife) == 0) { model->SetName(Constants::pbrKnife); }
            if (path.compare(Constants::pbrHelmet) == 0) { model->SetName(Constants::pbrHelmet); }
            if (path.compare(Constants::pbrShield) == 0) { model->SetName(Constants::pbrShield); }

            if (path.compare(Constants::pbrScar) == 0)
            {
                model->SetName(Constants::pbrScar);
                model->SetScale(glm::vec3(.05f, .05f, .05f));
            }

            _Scene->AddModelToScene(model);
        }
    }

    void RocketEngine::CreateCamera()
    {
        GLfloat aspectRatio = (float)_BufferWidth / (float)_BufferHeight;
        _Camera = shared_ptr<PerspectiveCamera>(new PerspectiveCamera(glm::vec3(-1.165972f, 2.022364f, 7.293371f),
            glm::vec3(0.0, 1.0, 0.0), 0.1f, 1000.0f, 45.0f,
            aspectRatio,
            281.0f, -15.0f, 15.0f, 0.5f));
    }

    void RocketEngine::CreateTerrain()
    {
        _Terrain = ElementFactory::CreateTerrain(Constants::TerrainHeightmapFilePath, Constants::TerrainBaseTexFilePath, _Shader->GetShader(ShaderType::TERRAIN));
        _Terrain->SetLight(_DirLight, _PointLight);
    }

    void RocketEngine::CreateLight()
    {
        glm::vec3 lightposition = glm::vec3(10.0, 10.0, -20.0);
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        glm::vec3 directionOfLight = glm::vec3(1.0f, 1.0f, 1.0f);

        _DirLight = ElementFactory::CreateDirectionalLight(lightColor, 0.1f, directionOfLight, 0.7f);
        _PointLight = ElementFactory::CreatePointLight(lightColor, 0.001f, lightposition, 0.7f, 0.0032f, 0.009f, 1.0f);

        _Scene->AddLightToScene(_PointLight);
        _Scene->AddLightToScene(_DirLight);
    }

    void RocketEngine::Run()
    {
        while (!glfwWindowShouldClose(_Window)) // app loop 
        {
            // event handling
            EventHandling();
            // update
            Update();
            // render
            Render();
        }
    }

    void RocketEngine::Release()
    {
        // free or Release	
        glfwTerminate();
    }

    void RocketEngine::Update()
    {
        auto newTime = glfwGetTime();
        _DeltaTime = newTime - _LastTime;
        _LastTime = newTime;

        if (_PointLight->GetPosition() != _PointLightPreviousPosition)
        {
            auto deltaLightPosition = _PointLight->GetPosition() - _PointLightPreviousPosition;
            _LightModel = glm::translate(_PointLight->GetLightModelMatrix(), deltaLightPosition);
            _PointLight->SetLightModelMatrix(_LightModel);
        }

        UpdateCameraPosition();
        UpdateCameraRotation();
        UpdateCameraAspectRation();
        UpdateLightPosition();
    }

    void RocketEngine::Render()
    {
        // start recording every draw call on the mainFBO.
        _MainFBO->enableFramebuffer();
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (_AppUI.DisplayElements())
        {
            //ModelLoader::LoadModel(files[_AppUI.selectedModel - 1]);
            _ActiveModelIndex = _AppUI.selectedModel;
        }

        _AppUI.Render();
        _ActiveModel = _AppUI.modelPaths[_ActiveModelIndex];

        DrawTerrain();
        _Grid->DrawGrid(glm::mat4(1.0), _Camera->GetViewMatrix(), _Camera->GetProjectionMatrix());

        glm::mat4 model(1.0);
        model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));

        auto projection = _Camera->GetProjectionMatrix();
        auto lightProjection = _Camera->GetProjectionMatrix();
        auto view = _Camera->GetViewMatrix();
        auto lightView = _Camera->GetViewMatrix();
        auto eyePosition = _Camera->GetCameraPosition();

        //_AppUI.DrawElements();

        std::shared_ptr<Shader> activeShader = _Shader->GetShader(ShaderType::NORMAL_MAP);

        for (auto light : _Scene->GetAllLightsFromScene())
        {
            light->DrawLight(projection, view);
        }

        try
        {
            std::vector<std::shared_ptr<Model>>& entities = _Scene->GetAllModelsFromScene();
            for (int i = 0; i < entities.size(); ++i)
            {
                glm::mat4 model(1.0);
                std::shared_ptr<Model> mdl = entities.at(i);
                glm::vec3 position = mdl->GetPosition();
                glm::vec3 scale = mdl->GetScale();
                model = glm::scale(model, scale);
                model = glm::translate(model, glm::vec3(position));

                std::vector<std::shared_ptr<Mesh>>& meshes = mdl->GetAllMeshesFromScene();
                std::vector<std::shared_ptr<Material>>& mtls = mdl->GetAllMtlsFromScene();

                for (int j = 0; j < meshes.size(); ++j)
                {
                    std::shared_ptr <Mesh> pmsh = meshes.at(j);
                    int matidx = pmsh->GetMaterialIdx();
                    std::shared_ptr <Material> mat = mtls.at(matidx);

                    activeShader = _Shader->GetShader(mat->GetShaderName());

                    if (mdl->GetName().compare(_ActiveModel) == 0)
                    {
                        activeShader = _Shader->GetShader(ShaderType::PBR);

                        glm::vec3 albedo = glm::vec3(1.00, 0.25, 0.10);

                        glm::mat4 m1 = glm::mat4(1.0f);
                        m1 = glm::scale(m1, mdl->GetScale());
                        m1 = glm::rotate(m1, _rt += .00049f, glm::vec3(0.f, 1.0f, 0.f));

                        activeShader->SetValMat4("model", m1);
                        activeShader->SetValMat4("projection", projection);
                        activeShader->SetValMat4("view", view);
                        activeShader->SetValVec3("eyePosition", _Camera->GetCameraPosition());
                        mat->UseMaterial(activeShader.get());

                        // if the object needs to be tested with point and directional lights
                        // comment if not needed
                        _DirLight->UseLight(activeShader.get());
                        _PointLight->UseLight(activeShader.get());
                        activeShader->SetValVec3("lightPosition", _PointLight->GetPosition());

                        glBindVertexArray(pmsh->GetVAO());
                        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pmsh->GetEBO());

                        if (_AppUI.show_instancing())
                        {
                            glDrawElementsInstanced(GL_TRIANGLES, pmsh->GetTriangleCount(), GL_UNSIGNED_INT, 0, 5);
                        }
                        else
                        {
                            glDrawElements(GL_TRIANGLES, pmsh->GetTriangleCount(), GL_UNSIGNED_INT, 0);
                        }

                    }
                }
            }
        }
        catch (const std::exception& e)
        {
            std::cout << e.what();
        }

        _AppUI.DrawElements();

        //stop recording framebuffer
        _MainFBO->disableFramebuffer();

        //set color filter color.
        _Shader->GetShader(ShaderType::COLOR_FILTER)->SetValVec3("colorFilter", glm::vec3(1.38f, 1.0f, .8f));

        //draw the recorded framebuffer
        _MainFBO->drawFramebuffer(ShaderType::SCREEN_SPACE);

        glfwSwapBuffers(_Window);
        glfwPollEvents();
    }

    void RocketEngine::DrawTerrain()
    {
        // Draw Terrain
        _Terrain->SetHeightScale(_AppUI.GetTerrainHeightScale());
        _Terrain->SetGridScale(_AppUI.GetTerrainGridScale());
        _Terrain->SetWireframeMode(_AppUI.GetTerrainWireMode());
        _Terrain->Render( _Camera->GetViewMatrix(), _Camera->GetProjectionMatrix(), _Camera->GetCameraPosition());
    }

    void RocketEngine::SetWindow(unsigned int w, unsigned int h)
    {
        _WinWidth = w;
        _WinHeight = h;
    }

    void RocketEngine::SetBufferSize(unsigned int w, unsigned int h)
    {
        _BufferWidth = w; 
        _BufferHeight = h;
    }

    int RocketEngine::CreateAppWindow()
    {
        if (!glfwInit())
        {
            std::cout << Constants::Errors.at(0);
            return -1;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _GL_Version);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _GL_Version);

        //GLFW_OPENGL_CORE_PROFILE = No Backward compatibility
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        //GLFW_OPENGL_FORWARD_COMPAT = Allow forward compatibility
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        _Window = glfwCreateWindow(_WinWidth, _WinHeight, Constants::AppName.c_str(), nullptr, nullptr);

        if (_Window == nullptr)
        {
            std::cout << Constants::Errors.at(1);
            glfwTerminate();
            return -1;
        }

        glfwMakeContextCurrent(_Window);		

        glfwGetFramebufferSize(_Window, &_BufferWidth, &_BufferHeight);

        glfwSetInputMode(_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        if (!(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)))
        {
            std::cout << Constants::Errors.at(2);
            return -1;
        }

        glViewport(0, 0, _WinWidth, _WinHeight);
        glEnable(GL_DEPTH_TEST);

        glfwSetFramebufferSizeCallback(_Window, FrameBufferSizeCallback);        
        glfwSetWindowSizeCallback(_Window, WindowSizeCallback);
        glfwSetCursorPosCallback(_Window, CursorPositionCallback);
        glfwSetKeyCallback(_Window, KeyCallback);

        return 1;
    }

    void RocketEngine::EventHandling()
    {
    }

    void RocketEngine::WindowSizeCallback(GLFWwindow* window, int width, int height)
    {
        _WinWidth = width;
        _WinHeight = height;
    }

    void RocketEngine::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    void RocketEngine::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (key == GLFW_KEY_N && action == GLFW_PRESS)
            _IsNormalMappingEnabled = !_IsNormalMappingEnabled;

        if (key == GLFW_KEY_R && action == GLFW_REPEAT)
        {
            _rt += .05f;
        }
        if (key == GLFW_KEY_T && action == GLFW_REPEAT)
        {
            _rt -= .05f;
        }
        if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        {
            if (++_ActiveModelIndex > mFiles.size()-1)_ActiveModelIndex = 1;
        }
        if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        {
            if (--_ActiveModelIndex < 1) _ActiveModelIndex = (int)mFiles.size()-1;
        }
        if (action == GLFW_PRESS)
        {
            keyPresses.push_back(key);
        }

        if (action == GLFW_RELEASE)
        {
            auto itr = std::find(keyPresses.begin(), keyPresses.end(), key);
            if (itr != keyPresses.end())
            {
                keyPresses.erase(itr);
            }
        }
    }

    void RocketEngine::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
        if (xpos > 0 && xpos < 350)
            if (ypos > 0 && ypos < 430)
                return;

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS)
        {
            lastX = xpos;
            lastY = ypos;
            return;
        }

        if (_IsFirstMovement == true)
        {
            lastX = xpos;
            lastY = ypos;
            _IsFirstMovement = false;
        }

        xChange = xpos - lastX;
        yChange = lastY - ypos;

        lastX = xpos;
        lastY = ypos;
    }



    void RocketEngine::UpdateCameraPosition()
    {
        auto position = _Camera->GetCameraPosition();
        auto front = _Camera->GetCameraFrontVector();
        auto right = _Camera->GetCameraRightVector();
        auto movementSpeed = _Camera->GetMoveSpeed();
        float velocity = _DeltaTime * movementSpeed;

        auto itrW = std::find(keyPresses.begin(), keyPresses.end(), GLFW_KEY_W);

        if (itrW != keyPresses.end())
            position += front * velocity;

        auto itrA = std::find(keyPresses.begin(), keyPresses.end(), GLFW_KEY_A);

        if (itrA != keyPresses.end())
            position -= right * velocity;

        auto itrS = std::find(keyPresses.begin(), keyPresses.end(), GLFW_KEY_S);

        if (itrS != keyPresses.end())
            position -= front * velocity;

        auto itrD = std::find(keyPresses.begin(), keyPresses.end(), GLFW_KEY_D);

        if (itrD != keyPresses.end())
            position += right * velocity;

        _Camera->SetCameraPosition(position);
    }

    // move the light based on arrow keys
    void RocketEngine::UpdateLightPosition()
    {
        auto lightPosition = _PointLight->GetPosition();
        auto velocity = _DeltaTime * 0.1f;
        _PointLightPreviousPosition = lightPosition;

        auto itrUp = std::find(keyPresses.begin(), keyPresses.end(), GLFW_KEY_UP);

        if (itrUp != keyPresses.end())
            lightPosition.y += 100.0f * velocity;

        auto itrDown = std::find(keyPresses.begin(), keyPresses.end(), GLFW_KEY_DOWN);

        if (itrDown != keyPresses.end())
            lightPosition.y -= 100.0f * velocity;

        auto itrLeft = std::find(keyPresses.begin(), keyPresses.end(), GLFW_KEY_LEFT);

        if (itrLeft != keyPresses.end())
            lightPosition.x += 100.0f * velocity;

        auto itrRight = std::find(keyPresses.begin(), keyPresses.end(), GLFW_KEY_RIGHT);

        if (itrRight != keyPresses.end())
            lightPosition.x -= 100.0f * velocity;

        auto itrPageUp = std::find(keyPresses.begin(), keyPresses.end(), GLFW_KEY_PAGE_UP);

        if (itrPageUp != keyPresses.end())
            lightPosition.z += 100.0f * velocity;

        auto itrPageDown = std::find(keyPresses.begin(), keyPresses.end(), GLFW_KEY_PAGE_DOWN);

        if (itrPageDown != keyPresses.end())
            lightPosition.z -= 100.0f * velocity;

        _PointLight->SetPosition(lightPosition);
    }

    void RocketEngine::UpdateCameraRotation()
    {
        float turnSpeed = _Camera->GetTurnSpeed();
       
        xChange *= turnSpeed;
        yChange *= turnSpeed;

        double pitch = _Camera->GetPitch() + yChange;
        double yaw = _Camera->GetYaw() + xChange;

        if (pitch >= 89.0f)
            pitch = 89.0f;

        if (pitch <= -89.0f)
            pitch = -89.0f;

        _Camera->SetPitch((float)pitch);
        _Camera->SetYaw((float)yaw);

        _Camera->Update();

        xChange = 0.0f;
        yChange = 0.0f;
    }

    void RocketEngine::UpdateCameraAspectRation()
    {
        GLfloat aspectRatio = (float)_WinWidth / (float) _WinHeight;
        _Camera->SetAspectRatio(aspectRatio);
    }

}