// Objects
#include "imageObject.h"
#include "textImageObject.h"
#include "meshObject.h"
#include "pacman.h"

#include "sceneMedium.h"  // To share data between scenes

#ifndef SCENE_H
#define SCENE_H

// SCREEN RESOLUTION
// Placed here for mouse input controls,
// and main() needs it for glfwOpenWindow()
static const int WIDTH  = 1024;
static const int HEIGHT = 768;

// List of scenes identified when doing a scene transition
static enum SCENES { TITLE, EDITOR, GAME };

class scene
{

public:

  //=================================================================
  // Constructor
  //=================================================================
  scene();

  //=================================================================
  // Milliseconds: Time for animation
  // return value: An int representing a scene to transition to
  //               Return -1 to keep the current scene
  //=================================================================
  virtual int Draw(float Milliseconds) = 0;

  //=================================================================
  // Handle input
  //=================================================================
  virtual void KeyInput(int key, int action) = 0;
  virtual void MouseInput(int button, int action) = 0;
  void KeyInput();  // Default FPS controls

protected:

  matrixManager* MatrixManager;
  sceneMedium*   SceneMedium;

  float deltaTime;     // Milliseconds since the last draw() call

  // For the default camera KeyInput() (no parameters)
  float horizontalAngle;
  float verticalAngle;
  float mouseSpeed;
  glm::vec3 location;

  int changeScene;  // A number above -1 signals a scene transition

};

#endif //SCENE_H