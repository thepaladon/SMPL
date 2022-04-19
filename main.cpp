// Credits to "Pizzaboy150" for making this single file starter template
// Source: https://forums.raspberrypi.com/viewtopic.php?t=252978
// And my thanks to Matěj Kaločai for cleaning it up and sharing it
//ssh pi@169.254.75.49
//pass raspberrry (three rs)

#include <chrono>
#include <iostream>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/XKBlib.h>

#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include <glm/glm.hpp>
#include "external/imgui/imgui_impl_opengl3.h"
#include "src/Game.h"
#include "src/Shader.h"
#include "src/VerterBufferLayout.h"
#include "src/VertexArray.h"
#include "src/PhysicsComponent.h"
#include "src/Camera.h"
#include "src/Counter.h"

// WINDOW and EGL needed things
Window win;
Display* display;
EGLDisplay eglDisplay;
EGLSurface eglSurface;
EGLContext eglContext;

// PYBULLET
btBroadphaseInterface* BroadPhase;
btDefaultCollisionConfiguration* CollisionConfiguration;
btCollisionDispatcher* Dispatcher;
btSequentialImpulseConstraintSolver* ConstraintSolver;
btDiscreteDynamicsWorld* DynamicPhysicsWorld;

Game* game;
ImGuiIO* io;
Counter Counter::s_Instance;

// FRAME BUFFER :
glm::uint depthTexture, depthBuffer, fbTexture; 

// WINDOW SIZE
int win_width = 800;
int win_height = 800;
#define ANTI_ALIASING false

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

inline int BitScanForward(int input)
{
	int i{ 0 };
	for (i = 0; input >>= 1; ++i);
	return i;
}

void* ProcessInputThread(void* arg);

void InitializeWindow()
{
	// Init multithreading
	XInitThreads();

	// Create Display
	display = XOpenDisplay(NULL);
	if (display == NULL)
	{
		std::cout << "Error creating X display!" << std::endl;
		exit(1);
	}

	//get information about out display
	int screen_num = DefaultScreen(display);

	//now lets create the window
	int win_x = 0;
	int win_y = 0;

	win = XCreateSimpleWindow(
		display,
		RootWindow(display, screen_num),
		win_x,
		win_y,
		win_width,
		win_height,
		0,
		BlackPixel(display, screen_num),
		WhitePixel(display, screen_num)
	);


	//Window name
	XStoreName(display, win, "SimplPi");

	//register for events
	XSelectInput(display, win, ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);
	//make the window visable
	XMapWindow(display, win);


}

void InitializeEGL()
{
	eglDisplay = eglGetDisplay((EGLNativeDisplayType)display);
	if (eglDisplay == EGL_NO_DISPLAY)
	{
		printf("ERROR: \"Failed to get Display\"\n"); exit(2);
	}

	// Initialising EGL (can replace NULL with major/minor version variables)
	if (eglInitialize(eglDisplay, NULL, NULL) != EGL_TRUE)
	{
		printf("ERROR: \"Failed to Initialize EGL\"\n"); return exit(3);
	}

	//Create an EGL config for what we want
	EGLConfig config;
	EGLint numConfigs;
	EGLint configs[] =
	{
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_DEPTH_SIZE, 8,
		EGL_SURFACE_TYPE,
		EGL_WINDOW_BIT,
		EGL_CONFORMANT,
		EGL_OPENGL_ES3_BIT,

#if ANTI_ALIASING
		EGL_SAMPLE_BUFFERS, 1,
		EGL_SAMPLES, 4,
#endif
		EGL_NONE
	};

	// Choosing which config to use
	if (eglChooseConfig(eglDisplay, configs, &config, 1, &numConfigs) == EGL_FALSE)
	{
		printf("ERROR: \"Failed to choose config\"\n"); exit(4);
	}

	// Creating the window surface
	eglSurface = eglCreateWindowSurface(eglDisplay, config, win, NULL);
	if (eglSurface == EGL_NO_SURFACE)
	{
		printf("ERROR: \"Failed to create Window Surface\"\n"); exit(5);
	}

	// Context Attributes
	EGLint contextAttribs[] =
	{
		EGL_CONTEXT_MAJOR_VERSION, 3,
		EGL_CONTEXT_MINOR_VERSION, 1,
		EGL_NONE
	};

	// Create rendering context
	eglContext = eglCreateContext(eglDisplay, config, EGL_NO_CONTEXT, contextAttribs);
	if (eglContext == EGL_NO_CONTEXT)
	{
		printf("ERROR: \"Failed to create Context\"\n"); exit(6);
	}

	// Set context as current
	if (eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) != EGL_TRUE)
	{
		printf("ERROR: \"Failed to make context current\"\n"); exit(7);
	}

	std::cout << "OpenGL Info: " << std::endl;
	std::cout << glGetString(GL_VERSION) << std::endl;
	std::cout << glGetString(GL_VENDOR) << std::endl;
	std::cout << glGetString(GL_RENDERER) << std::endl << std::endl;
}

void InitializeBullet()
{
	// create the main physics systemsw
	BroadPhase = new btDbvtBroadphase();
	CollisionConfiguration = new btDefaultCollisionConfiguration();
	Dispatcher = new btCollisionDispatcher(CollisionConfiguration);
	ConstraintSolver = new btSequentialImpulseConstraintSolver;
	DynamicPhysicsWorld = new btDiscreteDynamicsWorld(Dispatcher, BroadPhase, ConstraintSolver, CollisionConfiguration);
	// set a "normal" gravity level
	DynamicPhysicsWorld->setGravity(btVector3(0, -9.81f, 0));

}

void InitializeImGui()
{
	const char* glsl_version = "#version 300 es";
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplOpenGL3_Init(glsl_version);
	
	io = &ImGui::GetIO();
	io->DisplaySize = ImVec2(win_width, win_height);
}

int main(int argc, char* argv[])
{
	//events
	game = new Game();

	InitializeWindow();
	InitializeEGL();
	InitializeBullet();
	InitializeImGui();



	pthread_t inputThread;
	int threadFailed = pthread_create(&inputThread, NULL, &ProcessInputThread, nullptr);
	if (threadFailed)
		printf("ERROR: \"Failed to create Input Thread\"\n");

	//main program loop
	float deltatime = 0.f;
	float totaltime = 0.0f;
	unsigned int frames = 0;

	PhysicsManager bulletHelpingClass(game->getCamera()); //OH i need to store the debug draw!
	bulletHelpingClass.SetupMesh();

	game->passPhysicsWorld(DynamicPhysicsWorld, &bulletHelpingClass);
	game->passWindowSize(win_width, win_height);
	game->OnInit();

	//texture
	glGenTextures(1, &fbTexture);
	glBindTexture(GL_TEXTURE_2D, fbTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, win_width, win_height, 0, GL_RGBA, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//fb
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, win_width, win_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL); // VERY IMPORTANT AND PI SPECIFIC

	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, win_width, win_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, win_width, win_height);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("FrameBuffer ERROR \n");
	}

	
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
	VertexBuffer* m_vb = new VertexBuffer(quadVertices, sizeof(quadVertices));
	VertexBufferLayout* m_layout = new VertexBufferLayout();

	m_layout->PushFloat(2);
	m_layout->PushFloat(2);

	m_vao->AddBuffer(*m_vb, *m_layout);
	Shader* screenShader = new Shader("../../../res/shaders/screenShader");


	DynamicPhysicsWorld->setDebugDrawer(&bulletHelpingClass);
	DynamicPhysicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_WRITEMASK);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	eglSwapInterval(eglDisplay, 0);


	int type = 0;
	float offset = 300.f;
	float gamma = 1.44f;
	float exposure = 0.44f;


	std::chrono::time_point<std::chrono::system_clock> lastFrame = std::chrono::high_resolution_clock::now();
	while (game->gameRunning)
	{
		srand(mix(clock(), time(NULL), getpid()));

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glViewport(0, 0, win_width, win_width);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
		glEnable(GL_DEPTH_TEST);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

		srand(std::chrono::high_resolution_clock::now().time_since_epoch().count());
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		std::chrono::time_point<std::chrono::system_clock> startFrame = std::chrono::high_resolution_clock::now();
		std::chrono::nanoseconds timeFrame = startFrame - lastFrame; // this gives the time as an int. we divide it to get it to a float
		deltatime = static_cast<float>(timeFrame.count()) / static_cast<float>(std::nano::den);
		io->Framerate = deltatime;
		lastFrame = startFrame;
		totaltime += deltatime;
		frames++;

		//ImGui::SetNextWindowPos(ImVec2(0, 0));


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

		
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		screenShader->Bind();
		m_vao->Bind();
		screenShader->SetUniform1i("kernelType", type);
		screenShader->SetUniform1i("screenTexture", 0);
		screenShader->SetUniform1f("offset", 1.f / offset);
		screenShader->SetUniform1f("gamma", gamma);
		screenShader->SetUniform1f("exposure", exposure);

		glDisable(GL_DEPTH_TEST);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fbTexture);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindTexture(GL_TEXTURE_2D, 0);


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (totaltime > 1.0f)
		{
			printf("%4d frames rendered in %1.4f seconds -> FPS=%3.4f\n", frames, totaltime, static_cast<float>(frames) / totaltime);  // uncomment to see frames
			totaltime -= 1.0f;
			frames = 0;
		}

		//swap the buffers
		eglSwapBuffers(eglDisplay, eglSurface);
	}

	game->OnShutdown();
	pthread_join(inputThread, nullptr);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
	eglDestroySurface(eglDisplay, eglSurface);
	eglDestroyContext(eglDisplay, eglContext);
	eglTerminate(eglDisplay);
	XDestroyWindow(display, win);
	XCloseDisplay(display);

	return 0;
}

void* ProcessInputThread(void* arg)
{
	XEvent x_event;

	while (game->gameRunning)
	{
		while (XPending(display))
		{
			XNextEvent(display, &x_event);
		}

		switch (x_event.type)
		{
		case ButtonPress:
		case ButtonRelease:
		{
			int button = BitScanForward(x_event.xbutton.button);
			int pressed = ButtonRelease - x_event.type;

			game->MouseButtonEvent((MouseButton)button, pressed);
			game->mouseState[button] = pressed;

			break;
		}
		case MotionNotify:
		{
			int mx = x_event.xmotion.x;
			int my = x_event.xmotion.y;

			// Check for movement
			if ((game->mouseX != mx) || (game->mouseY != my))
				game->MouseMove(mx - game->mouseX, my - game->mouseY);

			// Save position
			game->mouseX = mx;
			game->mouseY = my;
			break;
		}
		case KeyPress:
		case KeyRelease:
		{
			bool keyPressed = KeyRelease - x_event.type;
			KeySym key = XkbKeycodeToKeysym(display, x_event.xkey.keycode, 0, 0);

			game->keyState[key] = keyPressed;
			//io->KeysDown[key] = keyPressed;
			game->KeyEvent(key, keyPressed);
			break;
		}
		case ConfigureNotify:
		{
		eglQuerySurface(eglDisplay, eglSurface, EGL_WIDTH, &win_width);
		eglQuerySurface(eglDisplay, eglSurface, EGL_HEIGHT, &win_height);

		game->passWindowSize(win_width, win_height);

		glBindTexture(GL_TEXTURE_2D, fbTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, win_width, win_height, 0, GL_RGBA, GL_FLOAT, 0);

		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, win_width, win_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL); // VERY IMPORTANT AND PI SPECIFIC

		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, win_width, win_height);
		break;
		}

		}

		io->MousePos = ImVec2(static_cast<float>(game->mouseX), static_cast<float>(game->mouseY));
		io->MouseDown[0] = game->mouseState[0]; // left
		io->MouseDown[1] = game->mouseState[2]; // right
	}

	return 0;
}