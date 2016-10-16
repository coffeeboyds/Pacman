#include "circleObject.h"

//=================================================================
// Constructor
//=================================================================
circleObject::circleObject()
{
  programID = ShaderManager->getImageColorProgramID();

  setDrawMode(GL_LINE_LOOP);

  setVertices();
  makeIndicies();
}
//=================================================================
void circleObject::setVertices()
{
  vertex_count = 30;
  radius = 0.12;

  // MAKE A CIRCLE
  for( float i=0; i<2*PI; i+= 2*PI/vertex_count )
    buffers2D[position].push_back( glm::vec2( sin(i)*radius, cos(i)*radius ) );
}