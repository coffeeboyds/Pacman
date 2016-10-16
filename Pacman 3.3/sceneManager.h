// Include all the scenes ONLY here, so they're included only once
#include "titleScene.h"
#include "editorScene.h"
#include "gameScene.h"

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

static scene* currentScene;

class sceneManager
{

public:

  //=================================================================
  // Constructor
  //=================================================================
  sceneManager();

  //=================================================================
  // Milliseconds: time for animation
  //=================================================================
  void DrawCurrentScene(float Milliseconds);

private:

  //=================================================================
  // Calls the current scene's control module
  //=================================================================
  static void GLFWCALL keyWrapper(int key, int action);
  static void GLFWCALL mouseWrapper(int button, int action);

  //=================================================================
  // Change the current scene to the new one
  //=================================================================
  void setScene(int newScene);

};

#endif //SCENEMANAGER_H