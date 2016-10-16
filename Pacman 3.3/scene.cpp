#include "scene.h"

//=================================================================
// Constructor
//=================================================================
scene::scene()
{
  MatrixManager = matrixManager::GetInstance();
  SceneMedium   = sceneMedium::GetInstance();

  // For the default camera KeyInput()
  verticalAngle = 0;
  horizontalAngle = 0;
  mouseSpeed = 0.0005f;

  changeScene = -1;  // No scene transition to begin with
}
//=================================================================
void scene::KeyInput()
{
  //** DEFAULT CONTROLS FOR FPS MOVEMENT **//

  // Get mouse position
  int xpos, ypos;
  glfwGetMousePos(&xpos, &ypos);

  // Reset mouse position for next frame
  glfwSetMousePos(WIDTH/2, HEIGHT/2);

  // Compute new orientation
  horizontalAngle += mouseSpeed * float( WIDTH/2 - xpos );
  verticalAngle   += mouseSpeed * float( HEIGHT/2 - ypos );

  // Direction : Spherical coordinates to Cartesian coordinates conversion
  glm::vec3 direction(
    cos(verticalAngle) * sin(horizontalAngle), 
    sin(verticalAngle),
    cos(verticalAngle) * cos(horizontalAngle)
  );

  // Right vector
  glm::vec3 right = glm::vec3(
    sin(horizontalAngle - 3.14f/2.0f), 
    0,
    cos(horizontalAngle - 3.14f/2.0f)
  );
	
  // Up vector
  glm::vec3 up = glm::cross( right, direction );

  // Move forward
  if (glfwGetKey( GLFW_KEY_UP ) == GLFW_PRESS)
    location += direction * deltaTime * 0.5f;

  // Move backward
  if (glfwGetKey( GLFW_KEY_DOWN ) == GLFW_PRESS)
    location -= direction * deltaTime * 0.5f;
	
  // Strafe right
  if (glfwGetKey( GLFW_KEY_RIGHT ) == GLFW_PRESS)
    location += right * deltaTime * 0.5f;

  // Strafe left
  if (glfwGetKey( GLFW_KEY_LEFT ) == GLFW_PRESS)
    location -= right * deltaTime * 0.5f;

  MatrixManager->lookAt( location, location+direction, up );
}