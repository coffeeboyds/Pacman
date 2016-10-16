#include "imageObject.h"

//=================================================================
// Constructor
//=================================================================
imageObject::imageObject()
{
  programID = ShaderManager->getImageTextureProgramID();
}
//=================================================================
int imageObject::loadTexture(char* file_path)
{
  sceneObject::loadTexture(file_path);

  setVertices();
  setUVs();

  makeIndicies();

  return 1;
}
//=================================================================
void imageObject::setVertices()
{
  // Image properties
  int width, height;

  // Get image width and height
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
  glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

  // Transform the width and height to be between 0 and 1
  int biggest = (width > height)? width:height;
  float w = (float)width/biggest;
  float h = (float)height/biggest;

  // Form a square (2 triangles) with the image's width/height ratio
  buffers2D[position].push_back( glm::vec2( -w/2, -h/2 ) ); // Bottom-left
  buffers2D[position].push_back( glm::vec2(  w/2, -h/2 ) ); // Bottom-right
  buffers2D[position].push_back( glm::vec2( -w/2,  h/2 ) ); // Top-left

  buffers2D[position].push_back( glm::vec2(  w/2, -h/2 ) ); // Bottom-right
  buffers2D[position].push_back( glm::vec2(  w/2,  h/2 ) ); // Top-right
  buffers2D[position].push_back( glm::vec2( -w/2,  h/2 ) ); // Top-left

  vertex_count = buffers2D[position].size();
}
//=================================================================
void imageObject::setUVs()
{ 
  // These must be in the same order as the vertices buffer
  buffers2D[uv].push_back( glm::vec2(0, 0) ); // Bottom-left
  buffers2D[uv].push_back( glm::vec2(1, 0) ); // Bottom-right
  buffers2D[uv].push_back( glm::vec2(0, 1) ); // Top-left

  buffers2D[uv].push_back( glm::vec2(1, 0) ); // Bottom-right
  buffers2D[uv].push_back( glm::vec2(1, 1) ); // Top-right
  buffers2D[uv].push_back( glm::vec2(0, 1) ); // Top-left
}
//=================================================================
void imageObject::setTexture(GLuint _texture)
{
  texture = _texture;
  if( buffers2D->empty() )
  {
    setVertices();
    setUVs();
    makeIndicies();
  }
}