#include "screenObject.h"

//=================================================================
// Constructor
//=================================================================
screenObject::screenObject()
{
}
//=================================================================
float screenObject::getX()
{
  return modelMatrix[3][0];  // X translation component
}
//=================================================================
float screenObject::getY()
{
  return modelMatrix[3][1];  // Y translation component
}
//=================================================================
void screenObject::draw()
{
  if( !drawEnabled )
    return;

  // Use the shader program and bind the object's VAO
  glUseProgram(programID);
  glBindVertexArray( VAO );

  if( texture != -1 )  // If a texture was loaded already...
  {
    // Bind the texture and set the "textureSampler" (in the GLSL code) to Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(programID, "textureSampler"), 0);
  }
  
  MatrixManager->bindModel(modelMatrix, programID);

  glEnable(GL_BLEND);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
  glDrawElements(
                  getDrawMode(),     // mode
			      indices_count,    // count
			      GL_UNSIGNED_SHORT, // type
			      (void*)0           // element array buffer offset
		);
  glDisable(GL_BLEND);

  glBindVertexArray(0);    // Unbind
  glUseProgram(0);         // Turn the shader off
}