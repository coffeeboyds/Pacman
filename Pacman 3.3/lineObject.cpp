#include "lineObject.h"

//=================================================================
// Constructor
//=================================================================
lineObject::lineObject()
{
  programID = ShaderManager->getImageColorProgramID();

  setDrawMode(GL_LINE_STRIP);
  setVertices();
  vertex_count = 2;
  setMainColor( glm::vec3( 0.25, 0.25, 0.25 ) );
  makeIndicies();
}
//=================================================================
void lineObject::setVertices()
{
  // A line is just 2 points
  buffers2D[position].push_back( glm::vec2( 0, 0 ) );
  buffers2D[position].push_back( glm::vec2( 1.0, 0 ) );
}