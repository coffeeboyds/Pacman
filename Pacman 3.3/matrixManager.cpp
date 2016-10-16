#include "matrixManager.h"

matrixManager* matrixManager::matrixManager_Instance = nullptr;

//=================================================================
// Constructor
//=================================================================
matrixManager::matrixManager()
{
  ShaderManager = shaderManager::GetInstance();

  // Initialize the view and perspective matrices
  view = glm::lookAt(
                     glm::vec3(4,3,-3), // Camera is here
                     glm::vec3(0,0,0), // and looks here
                     glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                    );

  projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 200.0f);
}
//=================================================================
void matrixManager::bindModel(glm::mat4 ModelMatrix, GLuint programID)
{
  /** Send the model matrix of the object being drawn to the shader **/

  glm::mat4 mv  = view * ModelMatrix;

  // Send the matrices to the shader program
  glUniform1i(glGetUniformLocation(programID, "instanced"), false );
  glUniformMatrix4fv(glGetUniformLocation(programID, "MV"), 1, GL_FALSE, &mv[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &ModelMatrix[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(programID, "P"), 1, GL_FALSE, &projection[0][0]);
}
//=================================================================
void matrixManager::bindModel(std::vector<glm::mat4> ModelMatrices, GLuint programID, GLuint vboModelsView)
{
  /** Send the model matrices of the object being drawn to the shader, 
      to be drawn multiple times in in different positions/sizes/orientations **/

  std::vector<glm::mat4> mv;

  for( int i=0; i<ModelMatrices.size(); i++ )
  {
	mv.push_back( view * ModelMatrices.at(i) );
  }

    // Send the data
    glBindBuffer(GL_ARRAY_BUFFER, vboModelsView);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * mv.size(), &mv[0], GL_DYNAMIC_DRAW);

	// Loop over each column of the matrix... 
    for (unsigned int i = 0; i < 4 ; i++) {
      glEnableVertexAttribArray(modelsView + i);
      glVertexAttribPointer(modelsView + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(sizeof(GLfloat) * i * 4));
      glVertexAttribDivisor(modelsView + i, 1);
    }

  // Send the matrices to the shader program
  glUniform1i(glGetUniformLocation(programID, "instanced"), true );
  glUniformMatrix4fv(glGetUniformLocation(programID, "P"), 1, GL_FALSE, &projection[0][0]);
}
//=================================================================
void matrixManager::bindLights(std::vector<glm::vec3> lights)
{
  /** Send a set of lights to the shader programs 
      MAKES SURE YOU SET THE VIEW MATRIX FIRST BEFORE CALLING THIS METHOD!!! **/

  int count = lights.size();

  // Transform them to camera-space
  std::vector<glm::vec4> lightsView;
  for( int i=0; i<count; i++ )
  {
	  // Pre-condition: The view matrix is already set!
	  lightsView.push_back( view * glm::vec4( lights.at(i), 1 ) );
  }

  /** Send the location of the lights, and the number of lights to the shader program **/

  glUseProgram(ShaderManager->getMeshColorProgramID());
  glUniform1iv(glGetUniformLocation(ShaderManager->getMeshColorProgramID(), "num_lights"), 1, &count);
  if( count > 0 )
    glUniform4fv(glGetUniformLocation(ShaderManager->getMeshColorProgramID(), "LightsView"), count, &lightsView[0][0]);

  glUseProgram(ShaderManager->getMeshColorNormalMappingProgramID());
  glUniform1iv(glGetUniformLocation(ShaderManager->getMeshColorNormalMappingProgramID(), "num_lights"), 1, &count);
  if( count > 0 )
    glUniform4fv(glGetUniformLocation(ShaderManager->getMeshColorNormalMappingProgramID(), "LightsView"), count, &lightsView[0][0]);

  glUseProgram(ShaderManager->getMeshTextureProgramID());
  glUniform1iv(glGetUniformLocation(ShaderManager->getMeshTextureProgramID(), "num_lights"), 1, &count);
  if( count > 0 )
    glUniform4fv(glGetUniformLocation(ShaderManager->getMeshTextureProgramID(), "LightsView"), count, &lightsView[0][0]);

  glUseProgram(ShaderManager->getMeshTextureNormalMappingProgramID());
  glUniform1iv(glGetUniformLocation(ShaderManager->getMeshTextureNormalMappingProgramID(), "num_lights"), 1, &count);
  if( count > 0 )
    glUniform4fv(glGetUniformLocation(ShaderManager->getMeshTextureNormalMappingProgramID(), "LightsView"), count, &lightsView[0][0]);

  glUseProgram(0);
}
//=================================================================
void matrixManager::lookAt(glm::vec3 location, glm::vec3 lookAt, glm::vec3 up)
{
  view = glm::lookAt(
    location, // Camera is here
    lookAt,   // and looks here
    up        // Head is up (set to 0,-1,0 to look upside-down)
  );
}