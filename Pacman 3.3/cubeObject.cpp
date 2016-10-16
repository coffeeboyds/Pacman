#include "cubeObject.h"

//=================================================================
// Constructor
//=================================================================
cubeObject::cubeObject()
{
  programID = ShaderManager->getMeshColorProgramID();
  setVertices();
  setUVs();
  setNormals();
  computeTangentBasis();

  vertex_count = buffers3D[position].size();
  makeIndicies();
}
//=================================================================
void cubeObject::setVertices()
{
  // Vertices for a 1x1x1 cube, centered at the origin in model space
  buffers3D[position].push_back( glm::vec3(-0.5f,-0.5f,-0.5f) );
  buffers3D[position].push_back( glm::vec3(-0.5f,-0.5f, 0.5f) );
  buffers3D[position].push_back( glm::vec3(-0.5f, 0.5f, 0.5f) );
  buffers3D[position].push_back( glm::vec3( 0.5f, 0.5f,-0.5f) );
  buffers3D[position].push_back( glm::vec3(-0.5f,-0.5f,-0.5f) );
  buffers3D[position].push_back( glm::vec3(-0.5f, 0.5f,-0.5f) );
  buffers3D[position].push_back( glm::vec3( 0.5f,-0.5f, 0.5f) );
  buffers3D[position].push_back( glm::vec3(-0.5f,-0.5f,-0.5f) );
  buffers3D[position].push_back( glm::vec3( 0.5f,-0.5f,-0.5f) );
  buffers3D[position].push_back( glm::vec3( 0.5f, 0.5f,-0.5f) );
  buffers3D[position].push_back( glm::vec3( 0.5f,-0.5f,-0.5f) );
  buffers3D[position].push_back( glm::vec3(-0.5f,-0.5f,-0.5f) );
  buffers3D[position].push_back( glm::vec3(-0.5f,-0.5f,-0.5f) );
  buffers3D[position].push_back( glm::vec3(-0.5f, 0.5f, 0.5f) );
  buffers3D[position].push_back( glm::vec3(-0.5f, 0.5f,-0.5f) );
  buffers3D[position].push_back( glm::vec3( 0.5f,-0.5f, 0.5f) );
  buffers3D[position].push_back( glm::vec3(-0.5f,-0.5f, 0.5f) );
  buffers3D[position].push_back( glm::vec3(-0.5f,-0.5f,-0.5f) );
  buffers3D[position].push_back( glm::vec3(-0.5f, 0.5f, 0.5f) );
  buffers3D[position].push_back( glm::vec3(-0.5f,-0.5f, 0.5f) );
  buffers3D[position].push_back( glm::vec3( 0.5f,-0.5f, 0.5f) );
  buffers3D[position].push_back( glm::vec3( 0.5f, 0.5f, 0.5f) );
  buffers3D[position].push_back( glm::vec3( 0.5f,-0.5f,-0.5f) );
  buffers3D[position].push_back( glm::vec3( 0.5f, 0.5f,-0.5f) );
  buffers3D[position].push_back( glm::vec3( 0.5f,-0.5f,-0.5f) );
  buffers3D[position].push_back( glm::vec3( 0.5f, 0.5f, 0.5f) );
  buffers3D[position].push_back( glm::vec3( 0.5f,-0.5f, 0.5f) );
  buffers3D[position].push_back( glm::vec3( 0.5f, 0.5f, 0.5f) );
  buffers3D[position].push_back( glm::vec3( 0.5f, 0.5f,-0.5f) );
  buffers3D[position].push_back( glm::vec3(-0.5f, 0.5f,-0.5f) );
  buffers3D[position].push_back( glm::vec3( 0.5f, 0.5f, 0.5f) );
  buffers3D[position].push_back( glm::vec3(-0.5f, 0.5f,-0.5f) );
  buffers3D[position].push_back( glm::vec3(-0.5f, 0.5f, 0.5f) );
  buffers3D[position].push_back( glm::vec3( 0.5f, 0.5f, 0.5f) );
  buffers3D[position].push_back( glm::vec3(-0.5f, 0.5f, 0.5f) );
  buffers3D[position].push_back( glm::vec3( 0.5f,-0.5f, 0.5f) );
}
//=================================================================
void cubeObject::setUVs()
{
  buffers2D[uv].push_back( glm::vec2(0.000059f, 1.0f-0.000004f) );
  buffers2D[uv].push_back( glm::vec2(0.000103f, 1.0f-0.336048f) );
  buffers2D[uv].push_back( glm::vec2(0.335973f, 1.0f-0.335903f) );
  buffers2D[uv].push_back( glm::vec2(1.000023f, 1.0f-0.000013f) );
  buffers2D[uv].push_back( glm::vec2(0.667979f, 1.0f-0.335851f) );
  buffers2D[uv].push_back( glm::vec2(0.999958f, 1.0f-0.336064f) );
  buffers2D[uv].push_back( glm::vec2(0.667979f, 1.0f-0.335851f) );
  buffers2D[uv].push_back( glm::vec2(0.336024f, 1.0f-0.671877f) );
  buffers2D[uv].push_back( glm::vec2(0.667969f, 1.0f-0.671889f) );
  buffers2D[uv].push_back( glm::vec2(1.000023f, 1.0f-0.000013f) );
  buffers2D[uv].push_back( glm::vec2(0.668104f, 1.0f-0.000013f) );
  buffers2D[uv].push_back( glm::vec2(0.667979f, 1.0f-0.335851f) );
  buffers2D[uv].push_back( glm::vec2(0.000059f, 1.0f-0.000004f) );
  buffers2D[uv].push_back( glm::vec2(0.335973f, 1.0f-0.335903f) );
  buffers2D[uv].push_back( glm::vec2(0.336098f, 1.0f-0.000071f) );
  buffers2D[uv].push_back( glm::vec2(0.667979f, 1.0f-0.335851f) );
  buffers2D[uv].push_back( glm::vec2(0.335973f, 1.0f-0.335903f) );
  buffers2D[uv].push_back( glm::vec2(0.336024f, 1.0f-0.671877f) );
  buffers2D[uv].push_back( glm::vec2(1.000004f, 1.0f-0.671847f) );
  buffers2D[uv].push_back( glm::vec2(0.999958f, 1.0f-0.336064f) );
  buffers2D[uv].push_back( glm::vec2(0.667979f, 1.0f-0.335851f) );
  buffers2D[uv].push_back( glm::vec2(0.668104f, 1.0f-0.000013f) );
  buffers2D[uv].push_back( glm::vec2(0.335973f, 1.0f-0.335903f) );
  buffers2D[uv].push_back( glm::vec2(0.667979f, 1.0f-0.335851f) );
  buffers2D[uv].push_back( glm::vec2(0.335973f, 1.0f-0.335903f) );
  buffers2D[uv].push_back( glm::vec2(0.668104f, 1.0f-0.000013f) );
  buffers2D[uv].push_back( glm::vec2(0.336098f, 1.0f-0.000071f) );
  buffers2D[uv].push_back( glm::vec2(0.000103f, 1.0f-0.336048f) );
  buffers2D[uv].push_back( glm::vec2(0.000004f, 1.0f-0.671870f) );
  buffers2D[uv].push_back( glm::vec2(0.336024f, 1.0f-0.671877f) );
  buffers2D[uv].push_back( glm::vec2(0.000103f, 1.0f-0.336048f) );
  buffers2D[uv].push_back( glm::vec2(0.336024f, 1.0f-0.671877f) );
  buffers2D[uv].push_back( glm::vec2(0.335973f, 1.0f-0.335903f) );
  buffers2D[uv].push_back( glm::vec2(0.667969f, 1.0f-0.671889f) );
  buffers2D[uv].push_back( glm::vec2(1.000004f, 1.0f-0.671847f) );
  buffers2D[uv].push_back( glm::vec2(0.667979f, 1.0f-0.335851f) );
}
//=================================================================
void cubeObject::setNormals()
{
  buffers3D[normal].push_back( glm::vec3(0.0f,0.0f,-1.0f) );
  buffers3D[normal].push_back( glm::vec3(0.0f,0.0f,-1.0f) );
  buffers3D[normal].push_back( glm::vec3(0.0f,0.0f,-1.0f) );
  buffers3D[normal].push_back( glm::vec3(0.0f,0.0f,-1.0f) );
  buffers3D[normal].push_back( glm::vec3(0.0f,0.0f,1.0f) );
  buffers3D[normal].push_back( glm::vec3(0.0f,0.0f,1.0f) );
  buffers3D[normal].push_back( glm::vec3(0.0f,0.0f,1.0f) );
  buffers3D[normal].push_back( glm::vec3(0.0f,0.0f,1.0f) );
  buffers3D[normal].push_back( glm::vec3(1.0f,0.0f,0.0f) );
  buffers3D[normal].push_back( glm::vec3(1.0f,0.0f,0.0f) );
  buffers3D[normal].push_back( glm::vec3(1.0f,0.0f,0.0f) );
  buffers3D[normal].push_back( glm::vec3(1.0f,0.0f,0.0f) );
  buffers3D[normal].push_back( glm::vec3(0.0f,1.0f,0.0f) );
  buffers3D[normal].push_back( glm::vec3(0.0f,1.0f,0.0f) );
  buffers3D[normal].push_back( glm::vec3(0.0f,1.0f,0.0f) );
  buffers3D[normal].push_back( glm::vec3(0.0f,1.0f,0.0f) );
  buffers3D[normal].push_back( glm::vec3(-1.0f,0.0f,0.0f) );
  buffers3D[normal].push_back( glm::vec3(-1.0f,0.0f,0.0f) );
  buffers3D[normal].push_back( glm::vec3(-1.0f,0.0f,0.0f) );
  buffers3D[normal].push_back( glm::vec3(-1.0f,0.0f,0.0f) );
  buffers3D[normal].push_back( glm::vec3(0.0f,1.0f,0.0f) );
  buffers3D[normal].push_back( glm::vec3(0.0f,1.0f,0.0f) );
  buffers3D[normal].push_back( glm::vec3(0.0f,1.0f,0.0f) );
  buffers3D[normal].push_back( glm::vec3(0.0f,1.0f,0.0f) );
  buffers3D[normal].push_back( glm::vec3(0.0f,0.0f,-1.0f) );
  buffers3D[normal].push_back( glm::vec3(0.0f,0.0f,-1.0f) );
  buffers3D[normal].push_back( glm::vec3(0.0f,0.0f,-1.0f) );
  buffers3D[normal].push_back( glm::vec3(0.0f,0.0f,-1.0f) );
  buffers3D[normal].push_back( glm::vec3(0.0f,0.0f,1.0f) );
  buffers3D[normal].push_back( glm::vec3(0.0f,0.0f,1.0f) );
  buffers3D[normal].push_back( glm::vec3(0.0f,0.0f,1.0f) );
  buffers3D[normal].push_back( glm::vec3(0.0f,0.0f,1.0f) );
  buffers3D[normal].push_back( glm::vec3(1.0f,0.0f,0.0f) );
  buffers3D[normal].push_back( glm::vec3(1.0f,0.0f,0.0f) );
  buffers3D[normal].push_back( glm::vec3(1.0f,0.0f,0.0f) );
  buffers3D[normal].push_back( glm::vec3(1.0f,0.0f,0.0f) );
}