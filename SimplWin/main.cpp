
#include <chrono>
#include <iostream>
#include <process.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Game.h"
#include "Shader.h"
#include "VerterBufferLayout.h"
#include "PhysicsComponent.h"
#include "VertexArray.h"
#include "Camera.h"
#include "Counter.h"

Counter Counter::s_Instance;
btBroadphaseInterface* BroadPhase;
btDefaultCollisionConfiguration* CollisionConfiguration;
btCollisionDispatcher* Dispatcher;
btSequentialImpulseConstraintSolver* ConstraintSolver;
btDiscreteDynamicsWorld* DynamicPhysicsWorld;

Game* game;
ImGuiIO io;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void window_size_callback(GLFWwindow* window, int width, int height);

int ScreenHeight = 800;
float aspectRatio = 4.f / 3.f;
int ScreenWidth = ScreenHeight * static_cast<int>(aspectRatio);
glm::uint depthTexture, depthBuffer, fbTexture;

// Robert Jenkins' 96 bit Mix Function
unsigned long mix(unsigned long a, unsigned long b, unsigned long c)
{
    a = a - b;  a = a - c;  a = a ^ (c >> 13);
    b = b - c;  b = b - a;  b = b ^ (a << 8);
    c = c - a;  c = c - b;  c = c ^ (b >> 13);
    a = a - b;  a = a - c;  a = a ^ (c >> 12);
    b = b - c;  b = b - a;  b = b ^ (a << 16);
    c = c - a;  c = c - b;  c = c ^ (b >> 5);
    a = a - b;  a = a - c;  a = a ^ (c >> 3);
    b = b - c;  b = b - a;  b = b ^ (a << 10);
    c = c - a;  c = c - b;  c = c ^ (b >> 15);
    return c;
}

bool SetupGLFW(GLFWwindow*& window)
{
    /* Initialize the library */
    if (!glfwInit()) {
		return false; }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 32);

    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_SAMPLES, 12);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(ScreenWidth, ScreenHeight, "SimplWin Engine x64", nullptr, nullptr);
    if (!window)
    {
        printf("Window could not be initialized \n");
        glfwTerminate();
        return false;
    }


    glfwMakeContextCurrent(window);   
    glfwSwapInterval(1); // 1 = VSync, 0 = As many frames as possible
    return true;
}

bool SetupGLEW()
{
    if (glewInit() != GLEW_OK)
        return false;

    std::cout << "OpenGL Info: " << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_VENDOR) << std::endl;
    std::cout << glGetString(GL_RENDERER) << std::endl << std::endl;

    return true;
}

bool SetupPyBullet()
{
    // create the main physics systemsw
    BroadPhase = new btDbvtBroadphase();
    CollisionConfiguration = new btDefaultCollisionConfiguration();
    Dispatcher = new btCollisionDispatcher(CollisionConfiguration);
    ConstraintSolver = new btSequentialImpulseConstraintSolver;
    DynamicPhysicsWorld = new btDiscreteDynamicsWorld(Dispatcher, BroadPhase, ConstraintSolver, CollisionConfiguration);
    // set a "normal" gravity level
    DynamicPhysicsWorld->setGravity(btVector3(0, -9.81f, 0));

    return true;
}


bool SetupImGui(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
   // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsDark();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 300 es";
    ImGui_ImplOpenGL3_Init(glsl_version);

    return true;
}

int main(void)
{
    srand(mix(clock(), static_cast<unsigned long>(time(nullptr)), getpid()));
    game = new Game();
    GLFWwindow* window;
    
    bool glfwInit = SetupGLFW(window);
    if (!glfwInit) { printf("ERROR:: Failed to setup GLFW. \n"); return -1; }
   
    bool glewInit = SetupGLEW();
    if (!glewInit) { printf("ERROR:: Failed to setup GLEW. \n"); return -1; }

    bool ImGuiInit = SetupImGui(window);
    if (!ImGuiInit) { printf("ERROR:: Failed to setup ImGui. \n"); return -1; }

    bool PyBulletInit = SetupPyBullet();
    if (!PyBulletInit) { printf("ERROR:: Failed to setup PyBullet. \n"); return -1; }

    PhysicsManager bulletHelpingClass(game->getCamera()); //OH i need to store the debug draw!
	//REMEMBER TO CHANGE THE THING IN THE PI VERSION. NO SETUP MESH REQUIRED, IN THE CONSTR


    //Init Game
    game->passPhysicsWorld(DynamicPhysicsWorld, &bulletHelpingClass);
    game->passWindowSize(ScreenWidth, ScreenHeight);
    game->OnInit();

    //main program loop
    float deltatime = 0.f;
    float totaltime = 0.0f;
    unsigned int frames = 0;

	// FRAME BUFFER: Texture
    glGenTextures(1, &fbTexture);
    glBindTexture(GL_TEXTURE_2D, fbTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, ScreenWidth, ScreenHeight, 0, GL_RGBA, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // FRAME BUFFER 
    glm::uint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbTexture, 0);
    GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawBuffers);

    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, ScreenWidth, ScreenHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL); // VERY IMPORTANT AND PI SPECIFIC

    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ScreenWidth, ScreenHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, ScreenWidth, ScreenHeight);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) { printf("FrameBuffer ERROR \n"); }

    //GL ATTRIB
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_WRITEMASK);
    glDepthFunc(GL_LESS);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    VertexArray* m_vao = new VertexArray();
    VertexBuffer* m_vb = new VertexBuffer(quadVertices,  sizeof(quadVertices));
    VertexBufferLayout* m_layout = new VertexBufferLayout();

	m_layout->PushFloat(2);
    m_layout->PushFloat(2);

    m_vao->AddBuffer(*m_vb, *m_layout);
    Shader* screenShader = new Shader("../res/shaders/screenShader");

  
	DynamicPhysicsWorld->setDebugDrawer(&bulletHelpingClass);
    DynamicPhysicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);

	int type = 0;
    float offset = 300.f;
    float gamma = 1.44f;
    float exposure = 0.44f;

    auto lastFrame = std::chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(window))
    {
        glfwSetWindowShouldClose(window, !game->gameRunning);
        srand(mix(clock(), static_cast<unsigned long>(time(NULL)), getpid()));
        glfwSetWindowSizeCallback(window, window_size_callback);

        // FRAME BUFFER : Clear and Bind
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, ScreenWidth, ScreenHeight);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
    	glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
    	glCullFace(GL_BACK);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

        // FRAME RATE : Calculate delta time and frames
        auto startFrame = std::chrono::high_resolution_clock::now();
        std::chrono::nanoseconds timeFrame = startFrame - lastFrame; // this gives the time as an int. we divide it to get it to a float
        deltatime = static_cast<float>(timeFrame.count()) / static_cast<float>(std::nano::den);
        io.Framerate = deltatime;
        game->passDeltaTime(deltatime);
        lastFrame = startFrame;
        totaltime += deltatime;
        frames++;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // GLFW : Input 
        glfwSetKeyCallback(window, key_callback);

        game->Update(deltatime);
    	game->Render();

#if IMGUI_DEBUG
        // UI : Post-processing
        if (ImGui::TreeNode("Post-Processing:")) {
            ImGui::Checkbox("Collision Rend", &bulletHelpingClass.RenderDebug);
            ImGui::Checkbox("Collision Update", &bulletHelpingClass.UpdateDebug);
            ImGui::SliderInt("PPX:", &type, 0, 7);
            ImGui::DragFloat("Offset", &offset);
            ImGui::DragFloat("Gamma:", &gamma, 0.01f);
            ImGui::DragFloat("Exposure:", &exposure, 0.01f);

            ImGui::TreePop();
        }
#endif

        // BULLET : Collision Rendering
        if (bulletHelpingClass.RenderDebug) {
            DynamicPhysicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
            DynamicPhysicsWorld->debugDrawWorld();
            bulletHelpingClass.DrawFinal(game->getCamera()->getProjectionMatrix(), game->getCamera()->getViewMatrix());
        }

        // BULLET : Collision step through and check for new collisions
        if (bulletHelpingClass.UpdateDebug) {
            DynamicPhysicsWorld->stepSimulation(deltatime);
            bulletHelpingClass.CheckForCollision(Dispatcher);
        }

    	game->OnImGuiRender(deltatime, frames, totaltime);

        // FRAME BUFFER : Unbind and clear screen
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // FRAME BUFFER : Passing uniforms for settings
        screenShader->Bind();
        screenShader->SetUniform1i("kernelType", type);
        screenShader->SetUniform1i("screenTexture", 0);
        screenShader->SetUniform1f("offset", 1.f / offset);
        screenShader->SetUniform1f("gamma", gamma);
        screenShader->SetUniform1f("exposure", exposure);
        m_vao->Bind();

        // FRAME BUFFER : Draw to screen
        glDisable(GL_DEPTH_TEST);
        glActiveTexture(GL_TEXTURE0);
    	glBindTexture(GL_TEXTURE_2D, fbTexture);

    	glDrawArrays(GL_TRIANGLES, 0, 6);
        
    	glBindTexture(GL_TEXTURE_2D, 0);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwPollEvents();
        glfwSwapBuffers(window);

        // FRAME RATE : Display frames rendered over 1 second. 
        if (totaltime > 1.0f)
        {
            printf("%4d frames rendered in %1.4f seconds -> FPS=%3.4f\n", frames, totaltime, static_cast<float>(frames) / totaltime);  // uncomment to see frames
            totaltime -= 1.0f;
            frames = 0;
        }
    }

    glDeleteTextures(1, &fbTexture );
    glDeleteTextures(1, &depthTexture);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glDeleteFramebuffers(1, &fbo);
    delete game;
	glfwTerminate();
	return 0;
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    ScreenWidth = width;
    ScreenHeight = height;
    game->passWindowSize(ScreenWidth, ScreenHeight);

    glBindTexture(GL_TEXTURE_2D, fbTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, ScreenWidth, ScreenHeight, 0, GL_RGBA, GL_FLOAT, 0);

    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, ScreenWidth, ScreenHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL); // VERY IMPORTANT AND PI SPECIFIC

    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ScreenWidth, ScreenHeight);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //WL4 Suppression (they were unused)
    scancode = 0;
    mods = 0;

    if (key == GLFW_KEY_F4 && action == 1)
    {
        if (glfwGetWindowAttrib(window, GLFW_MAXIMIZED)) {
            glfwRestoreWindow(window);
            
        }else if (!glfwGetWindowAttrib(window, GLFW_MAXIMIZED)) {
            glfwMaximizeWindow(window);
        }
    }

	game->keyState[key] = action;
}