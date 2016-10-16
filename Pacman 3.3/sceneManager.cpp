#include "sceneManager.h"

//=================================================================
// Constructor
//=================================================================
sceneManager::sceneManager()
{
  // The title screen is the first scene
  currentScene = new titleScene();

  // Bind functions to handle input
  glfwSetKeyCallback(keyWrapper);
  glfwSetMouseButtonCallback(mouseWrapper);
}
//=================================================================
void sceneManager::DrawCurrentScene(float Milliseconds)
{
  // Draw the current scene
  int nextScene = currentScene->Draw(Milliseconds);

  if( nextScene != -1 )
  {
    // The scene is over, prepare the next one
    delete currentScene;
    setScene(nextScene);
  }
}
//=================================================================
void sceneManager::setScene(int newScene)
{
  switch( newScene )
  {
    case TITLE:  currentScene = new titleScene();  break;
	case EDITOR: currentScene = new editorScene(); break;
	case GAME:   currentScene = new gameScene();  break;
  }
}
//=================================================================
void GLFWCALL sceneManager::keyWrapper(int key, int action)
{
  currentScene->KeyInput(key, action);
}
//=================================================================
void GLFWCALL sceneManager::mouseWrapper(int key, int action)
{
  currentScene->MouseInput(key, action);
}