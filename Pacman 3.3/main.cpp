// Include OpenGL stuff
#include <glew.h>
#include <glfw.h>

// local classes
#include "sceneManager.h"

void init();  // Initialize both GLEW and GLFW


int main(void)
{
	init(); //setup GLEW and GLFW
	sceneManager* SceneManager = new sceneManager();

	// For speed computation
	double lastTime = glfwGetTime();
	double seconds;
	int nbFrames = 0;

	// Time computation for drawing
	double deltaTime;
	double lastDrawTime = glfwGetTime();

	do{
	    // Measure speed
		seconds = glfwGetTime() - lastTime;
	    nbFrames++;
        if ( seconds >= 1.0 ){ // If last prinf() was more than 1 sec ago
          // printf and reset timer
          printf("%f ms/frame\n", 1000.0/double(nbFrames));
          nbFrames = 0;
          lastTime += 1.0;
	    }

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Get the time since the last draw
		deltaTime = glfwGetTime() - lastDrawTime;
		lastDrawTime = glfwGetTime();

		// Draw the scene
		SceneManager->DrawCurrentScene(deltaTime*1000);

		// Swap buffers
		glfwSwapBuffers();
	}
	while( glfwGetWindowParam( GLFW_OPENED ) );
}

void init()
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );	
	}

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	if( !glfwOpenWindow( WIDTH, HEIGHT, 0,0,0,0, 32,0, GLFW_WINDOW ) )
	{
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
	}

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
	}

	glfwSetWindowTitle( "Pacman 3.3" );

	glfwSetMousePos(WIDTH/2, HEIGHT/2);

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	// Blend function for transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}