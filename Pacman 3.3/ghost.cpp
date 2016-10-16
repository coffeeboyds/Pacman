#include "ghost.h"

//=================================================================
// Constructor
//=================================================================
ghost::ghost()
{
  size = 1;
  xPos = 0;
  yPos = 0;
  zPos = 0;
  angle = 0;
}
//=================================================================
float ghost::getX()
{
  return xPos;
}
//=================================================================
float ghost::getY()
{
  return yPos;
}
//=================================================================
float ghost::getZ()
{
  return zPos;
}
//=================================================================
glm::mat4 ghost::getModelMatrix()
{
  glm::mat4 modelMatrix;

  modelMatrix = glm::translate( modelMatrix, glm::vec3( xPos, yPos, zPos ) );
  modelMatrix = glm::rotate(    modelMatrix, angle, glm::vec3( 0, 1, 0 ) );
  modelMatrix = glm::rotate(    modelMatrix, -90.0f, glm::vec3( 1, 0, 0 ) );
  modelMatrix = glm::scale(     modelMatrix, glm::vec3( size, size, size ) );

  return modelMatrix;
}
//=================================================================
void ghost::loadIdentity()
{
  xPos = 0;
  yPos = 0;
  zPos = 0;
}
//=================================================================
void ghost::translate(float x, float y, float z)
{
  xPos += x;
  yPos += y;
  zPos += z;
}
//=================================================================
void ghost::setAngle(float degrees)
{
  angle = degrees;
}
//=================================================================
void ghost::setSize(float factor)
{
  size = factor;
}