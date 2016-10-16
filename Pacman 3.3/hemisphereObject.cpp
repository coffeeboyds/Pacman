#include "hemisphereObject.h"

//=================================================================
// Constructor
//=================================================================
hemisphereObject::hemisphereObject()
{
  programID = ShaderManager->getMeshTextureProgramID();

  setDrawMode( GL_TRIANGLE_STRIP );
  setVertices();

  vertex_count = buffers3D[position].size();
  setMainColor( glm::vec3( 1, 1, 0 ) );

  makeIndicies();
}
//=================================================================
void hemisphereObject::setVertices()
{
  int sections = 15;
  int stacks = 15;

  float R = 1.0;
  int p = sections;
  int q = stacks;

  // The hemisphere.
  for(int j = 0; j < q; j++)
  {
    // One latitudinal triangle strip.
    for(int i = 0; i <= p; i++)
    {
      buffers3D[normal].push_back( glm::vec3( R * cos( (float)j/q * PI/2.0 ) * cos( 2.0 * (float)i/p * PI ),
                                    R * sin( (float)j/q * PI/2.0 ),
                                    R * cos( (float)j/q * PI/2.0 ) * sin( 2.0 * (float)i/p * PI ) ) );

      buffers3D[position].push_back( glm::vec3( R * cos( (float)j/q * PI/2.0 ) * cos( 2.0 * (float)i/p * PI ),
                                    R * sin( (float)j/q * PI/2.0 ),
                                    R * cos( (float)j/q * PI/2.0 ) * sin( 2.0 * (float)i/p * PI ) ) );

      buffers3D[normal].push_back( glm::vec3( R * cos( (float)(j+1)/q * PI/2.0 ) * cos( 2.0 * (float)i/p * PI ),
                                    R * sin( (float)(j+1)/q * PI/2.0 ),
                                    R * cos( (float)(j+1)/q * PI/2.0 ) * sin( 2.0 * (float)i/p * PI ) ) );

      buffers3D[position].push_back( glm::vec3( R * cos( (float)(j+1)/q * PI/2.0 ) * cos( 2.0 * (float)i/p * PI ),
                                     R * sin( (float)(j+1)/q * PI/2.0 ),
                                     R * cos( (float)(j+1)/q * PI/2.0 ) * sin( 2.0 * (float)i/p * PI ) ) );
    }
  }

  // The base of the hemisphere (a circle)
  for (float i=0; i < 2*PI; i+=2*PI/sections)
  {
    buffers3D[normal].push_back(   glm::vec3( 0.0, -1.0, 0.0 ) );
    buffers3D[position].push_back( glm::vec3(0, 0.0, 0) );
    
    buffers3D[position].push_back( glm::vec3( cos(i), 0.0, sin(i)) );
	buffers2D[uv].push_back(       glm::vec2( cos(i)/2+0.5, sin(i)/2+0.5) );
	buffers3D[normal].push_back(   glm::vec3( 0.0, -1.0, 0.0 ) );

    buffers3D[position].push_back( glm::vec3( cos(i+2*PI/sections), 0.0, sin(i+2*PI/sections)) );
	buffers2D[uv].push_back(       glm::vec2( cos(i+2*PI/sections)/2+0.5, sin(i+2*PI/sections)/2+0.5) );
	buffers3D[normal].push_back(   glm::vec3( 0.0, -1.0, 0.0 ) );
  }
}