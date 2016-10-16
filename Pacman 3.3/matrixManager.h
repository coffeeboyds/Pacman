#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp> // For glm::lookat()

#include "shaderManager.h"

#ifndef MATRIXMANAGER_H
#define MATRIXMANAGER_H

// Buffer data for VBO. The order must match the layout in the GLSL code.
#define position       0
#define uv             1
#define color          2
#define normal         3
#define tangent        4
#define bitangent      5
#define modelsView     6  // 4x4 matrices take 4 positions!
#define num_buffers    9

class matrixManager
{

public:

  //=================================================================
  // Singleton Getter
  //=================================================================
  static matrixManager* GetInstance()
  {
    if(matrixManager_Instance == nullptr)
    {
      matrixManager_Instance = new matrixManager();
    }

    return matrixManager_Instance;
  }

  //=================================================================
  // Send the model matrix of the object being drawn to the shader
  //=================================================================
  void bindModel(glm::mat4 ModelMatrix, GLuint programID);
  void bindModel(std::vector<glm::mat4> ModelMatrices, GLuint programID, GLuint vboModelsView);

  //=================================================================
  // Send a set of lights to the shader programs
  //=================================================================
  void bindLights(std::vector<glm::vec3> lights);

  //=================================================================
  // Set the view matrix
  //=================================================================
  void lookAt(glm::vec3 location, glm::vec3 lookAt, glm::vec3 up);

private:

  //=================================================================
  // Constructor
  //=================================================================
  matrixManager();

  // Don't forget to declare these two. You want to make sure they
  // are unaccessible otherwise you may accidentally get copies of
  // this singleton appearing.
  matrixManager(matrixManager const&);  // Don't Implement
  void operator=(matrixManager const&); // Don't implement

  //singleton
  static matrixManager* matrixManager_Instance;

  shaderManager* ShaderManager;

  glm::mat4 projection;
  glm::mat4 view;

};

#endif //MATRIXMANAGER_H