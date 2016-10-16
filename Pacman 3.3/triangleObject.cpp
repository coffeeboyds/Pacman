#include "triangleObject.h"

//=================================================================
// Constructor
//=================================================================
triangleObject::triangleObject()
{
  programID = ShaderManager->getImageColorProgramID();

  setVertices();
  vertex_count = 3;
  setMainColor( glm::vec3( 0.25, 0.25, 0.25 ) );
  makeIndicies();
}
//=================================================================
void triangleObject::setVertices()
{
  // A triangle is just 3 points
  buffers2D[position].push_back( glm::vec2( 0, 0 ) );
  buffers2D[position].push_back( glm::vec2( 1.0, 0 ) );
  buffers2D[position].push_back( glm::vec2( 0.5, 1.0 ) );
}