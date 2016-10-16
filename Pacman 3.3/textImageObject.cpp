#include "textImageObject.h"

//=================================================================
// Constructor
//=================================================================
textImageObject::textImageObject()
{
  programID = ShaderManager->getImageTextureProgramID();
  text = "";
}
//=================================================================
void textImageObject::setVertices()
{
  if( text == "" )
    return;

  unsigned int length = strlen(text);

  buffers2D[position].clear();
  // Form squares (2 triangles each) for each letter of size 1x1
  for ( unsigned int i=0 ; i<length; i++ )
  {
    buffers2D[position].push_back( glm::vec2( i-0.5, -0.5 ) ); // Bottom-left
	buffers2D[position].push_back( glm::vec2( i+0.5, -0.5 ) ); // Bottom-right
	buffers2D[position].push_back( glm::vec2( i-0.5,  0.5 ) ); // Top-left

	buffers2D[position].push_back( glm::vec2( i+0.5, -0.5 ) ); // Bottom-right
	buffers2D[position].push_back( glm::vec2( i+0.5,  0.5 ) ); // Top-right
	buffers2D[position].push_back( glm::vec2( i-0.5,  0.5 ) ); // Top-left
  }
}
//=================================================================
void textImageObject::setUVs()
{ 
  // Set the UV coordinates around the letters in ascii.tga

  unsigned int length = strlen(text);
  int rows = 6;
  int cols = 16;

  float x_unit = 1.0/cols;
  float y_unit = 1.0/rows;

  buffers2D[uv].clear();
  for ( unsigned int i=0 ; i<length ; i++ )
  {
    char character = text[i];
    float uv_x = (character%cols)*x_unit;
    float uv_y = 1 - ((character-32)/cols)*y_unit - y_unit + 0.01;

    buffers2D[uv].push_back( glm::vec2( uv_x       , uv_y ) );        // Bottom-left
    buffers2D[uv].push_back( glm::vec2( uv_x+x_unit, uv_y ) );        // Bottom-right
    buffers2D[uv].push_back( glm::vec2( uv_x       , uv_y+y_unit ) ); // Top-left

    buffers2D[uv].push_back( glm::vec2( uv_x+x_unit, uv_y ) );        // Bottom-right
    buffers2D[uv].push_back( glm::vec2( uv_x+x_unit, uv_y+y_unit ) ); // Top-right
    buffers2D[uv].push_back( glm::vec2( uv_x       , uv_y+y_unit ) ); // Top-left
  }
}
//=================================================================
void textImageObject::setText(char* _text)
{
  text = _text;
  setVertices();
  setUVs();

  vertex_count = buffers2D[position].size();
  makeIndicies();
}